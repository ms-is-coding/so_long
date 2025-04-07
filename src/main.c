/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smamalig <smamalig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 10:34:52 by smamalig          #+#    #+#             */
/*   Updated: 2025/04/08 01:31:04 by smamalig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mlx_int.h"
#include "so_long.h"
#include "mlx.h"

#include <X11/keysym.h>
#include <X11/X.h>
#include <signal.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>

static inline uint32_t blend_pixel_fast(uint32_t fg, uint32_t bg) {
	uint32_t	inv_a;
	uint32_t	a;
	uint32_t	r;
	uint32_t	g;
	uint32_t	b;

	inv_a = ((fg >> 24) & 0xff) + 1;
	if (inv_a == 0)
		return bg;
	if (inv_a == 255)
		return fg;
	a = 256 - inv_a;
	r = (((fg >> 16) & 0xff) * a + ((bg >> 16) & 0xff) * inv_a) >> 8;
	g = (((fg >> 8) & 0xff) * a + ((bg >> 8) & 0xff) * inv_a) >> 8;
	b = ((fg & 0xff) * a + (bg & 0xff) * inv_a) >> 8;
	return (a << 24) | (r << 16) | (g << 8) | b;
}

bool is_wall(t_game *g, int tx, int ty)
{
	return (tx < 0 || tx >= g->opt.map_width || ty < 0
		|| ty >= g->opt.map_height || g->map_matrix[ty][tx] == '1');
}

int	cleanup(t_game *g)
{
	int	tex_idx;

	tex_idx = 0;
	g->state.is_running = 0;
	if (g->threads.counter)
		pthread_kill(g->threads.counter, 0);
	if (g->threads.render)
		pthread_kill(g->threads.render, 0);
	while (tex_idx < TEX_COUNT)
	{
		if (g->textures[tex_idx]) {
			mlx_destroy_image(g->mlx, g->textures[tex_idx]);
			g->textures[tex_idx] = 0;
		}
		tex_idx++;
	}
	if (g->frame)
		mlx_destroy_image(g->mlx, g->frame);
	if (g->win)
		mlx_destroy_window(g->mlx, g->win);
	if (g->mlx)
	{
		mlx_destroy_display(g->mlx);
		free(g->mlx);
	}
	return (0);
}

int	on_destroy(t_game *g)
{
	cleanup(g);
	exit(0);
	return (0);
}

t_vector translate(t_game *g, float x, float y)
{
	return (t_vector){
		.x = x - g->window.x,
		.y = y - g->window.y
	};
}

void	set_player_position(t_game *g, int tx, int ty)
{
	g->player.x = tx * TILE_SIZE + .25 * (TILE_SIZE - g->player.w);
	g->player.y = ty * TILE_SIZE + .25 * (TILE_SIZE - g->player.h);
	g->player.px = g->player.x;
	g->player.py = g->player.y;
}

void	player_hitbox(t_game *g)
{
	t_vector t0 = translate(g, g->player.x, g->player.y);
	t_vector t1 = translate(g, g->player.x + g->player.w, g->player.y + g->player.h);

	ft_line(g, (t_point){ t0.x, t0.y }, (t_point){ t1.x, t0.y });
	ft_line(g, (t_point){ t1.x, t0.y }, (t_point){ t1.x, t1.y });
	ft_line(g, (t_point){ t1.x, t1.y }, (t_point){ t0.x, t1.y });
	ft_line(g, (t_point){ t0.x, t1.y }, (t_point){ t0.x, t0.y });
	ft_line(g, (t_point){ t0.x, t0.y }, (t_point){ t1.x, t1.y });
	ft_line(g, (t_point){ t0.x, t1.y }, (t_point){ t1.x, t0.y });
}

bool	is_out_of_bounds(t_game *g, int tx, int ty);

void	render_hitboxes(t_game *g)
{
	if (!g->debug_mode)
		return;
	int	x0 = g->window.x / TILE_SIZE - 1;
	int	x1 = (g->window.x + g->window.w) / TILE_SIZE + 1;
	int	y0 = g->window.y / TILE_SIZE - 1;
	int	y1 = (g->window.y + g->window.h) / TILE_SIZE + 1;

	for (int y = y0; y < y1; y++) {
		for (int x = x0; x < x1; x++) {
			if (is_out_of_bounds(g, x, y))
			{
				render_hitbox(g, TEX_EMPTY, x, y);
				continue ;
			}
			int mask = compute_texture_mask(g, x, y);
			int tex_idx = get_texture_index(mask);
			if (g->map_matrix[y][x] == 'C')
				render_hitbox(g, TEX_COLLECTIBLE, x, y);
			else if (g->map_matrix[y][x] == '1')
				render_hitbox(g, tex_idx, x, y);
		}
	}
	player_hitbox(g);
}

void	ft_image_to_vbuffer(t_game *g, void *img, t_rect pos);

void	ft_render_tile(t_game *g, int tx, int ty)
{
	int mask = compute_texture_mask(g, tx, ty);
	int tex_idx = get_texture_index(mask);
	t_vector t = translate(g, tx * TILE_SIZE, ty * TILE_SIZE);
	ft_image_to_vbuffer(g, g->textures[tex_idx], (t_rect){
		t.x, t.y, TILE_SIZE, TILE_SIZE
	});
}

int horizontal_overlap(struct s_player *player, t_hitbox box) {
	return (player->x < box.r) && (player->x + player->w > box.l);
}

int vertical_overlap(struct s_player *player, t_hitbox box) {
	return (player->y < box.b) && (player->y + player->h > box.t);
}

void move_down(struct s_player *p, t_hitbox box) {
	float overlap = p->y + p->h - box.t;
	float prev_overlap = p->py + p->h - box.t;
	if (prev_overlap <= 0 && overlap > 0 && horizontal_overlap(p, box)) {
		p->vy = 0;
		p->y = box.t - p->h;
	}
}

void move_up(struct s_player *p, t_hitbox box) {
	float overlap = box.b - p->y;
	float prev_overlap = box.b - p->py;
	if (prev_overlap <= 0 && overlap > 0 && horizontal_overlap(p, box)) {
		p->vy = 0;
		p->y = box.b;
	}
}

void move_right(struct s_player *p, t_hitbox box) {
	float overlap = p->x + p->w - box.l;
	float prev_overlap = p->px + p->w - box.l;
	if (prev_overlap <= 0 && overlap > 0 && vertical_overlap(p, box)) {
		p->vx = 0;
		p->x = box.l - p->w;
	}
}

void move_left(struct s_player *p, t_hitbox box) {
	float overlap = box.r - p->x;
	float prev_overlap = box.r - p->px;
	if (prev_overlap <= 0 && overlap > 0 && vertical_overlap(p, box)) {
		p->vx = -1e-10;
		p->x = box.r;
	}
}

void	ft_collision_x(t_game *g, struct s_player *p)
{
	t_hitbox	box;
	int			y0 = p->y / TILE_SIZE;
	int			y1 = (p->y + p->h) / TILE_SIZE;

	if (p->vx > 0) {
		for (int y = y0; y <= y1; y++) {
			int	tx = (p->x + p->w) / TILE_SIZE;
			if (is_solid(g, tx, y, &box) == 1)
				move_right(p, box);
		}
	}
	else if (p->vx < 0) {
		for (int y = y0; y <= y1; y++) {
			int	tx = p->x / TILE_SIZE;
			if (is_solid(g, tx, y, &box))
				move_left(p, box);
		}
	}
}

void	ft_collision_y(t_game *g, struct s_player *p)
{
	t_hitbox	box;
	int x0 = p->x / TILE_SIZE;
	int x1 = (p->x + p->w) / TILE_SIZE;
	if (p->vy > 0) {
		for (int x = x0; x <= x1; x++) {
			int	ty = (p->y + p->h) / TILE_SIZE;
			if (is_solid(g, x, ty, &box))
				move_down(p, box);
		}
	}
	else if (p->vy < 0) {
		for (int x = x0; x <= x1; x++) {
			int	ty = p->y / TILE_SIZE;
			if (ty < 0)
				return ;
			if (is_solid(g, x, ty, &box))
				move_up(p, box);
		}
	}
}

void	ft_player_update(t_game *g)
{
	struct s_player	*p;

	p = &g->player;
	p->vx *= g->opt.friction;
	p->vy += g->opt.gravity;
	if (g->input.keys.left)
		p->vx = -g->opt.velocity;
	else if (g->input.keys.right)
		p->vx = g->opt.velocity;
	if (g->state.should_dash) {
		if (g->input.keys.left || g->input.keys.right)
			p->vx *= g->opt.dash_multiplier;
		g->state.should_dash--;
	}
	if (g->state.should_jump)
	{
		p->vy = -g->opt.jump_force;
		g->state.should_jump = 0;
	}
	p->px = p->x;
	p->x += p->vx;
	ft_collision_x(g, p);
	p->py = p->y;
	p->y += p->vy;
	ft_collision_y(g, p);
}

void	ft_image_transform(t_game *g, void *img, t_rect p,
	uint32_t (*transform)(t_game *g, uint32_t *buf, t_rect p))
{

	int	_;
	void	*bg = mlx_get_data_addr(g->frame, &_, &_, &_);
	void	*fg = mlx_get_data_addr(img, &_, &_, &_);

	int x0 = (p.x < 0) ? -p.x : 0;
	int x1 = (p.x + p.w > g->window.w) ? g->window.w - p.x : p.w;
	int y0 = (p.y < 0) ? -p.y : 0;
	int y1 = (p.y + p.h > g->window.h) ? g->window.h - p.y : p.h;

	uint32_t *bg_ptr = (uint32_t *)bg + ((p.y + y0) * g->window.w) + (p.x + x0);

	for (int j = y0; j < y1; j++) {
		uint32_t *bg_row = bg_ptr;

		for (int i = x0; i < x1; i++) {
			uint32_t bg_px = *bg_row;

			*bg_row = blend_pixel_fast(transform(g, fg, (t_rect){
				.x = i, .y = j, .w = p.w, .h = p.h }), bg_px);
			bg_row++;
		}

		bg_ptr += g->window.w;
	}
}

void	ft_image_to_vbuffer(t_game *g, void *img, t_rect p)
{
	int	_;
	void	*bg = mlx_get_data_addr(g->frame, &_, &_, &_);
	void	*fg = mlx_get_data_addr(img, &_, &_, &_);

	int x0 = (p.x < 0) ? -p.x : 0;
	int x1 = (p.x + p.w > g->window.w) ? g->window.w - p.x : p.w;
	int y0 = (p.y < 0) ? -p.y : 0;
	int y1 = (p.y + p.h > g->window.h) ? g->window.h - p.y : p.h;

	uint32_t *fg_ptr = (uint32_t *)fg + (y0 * p.w) + x0;
	uint32_t *bg_ptr = (uint32_t *)bg + ((p.y + y0) * g->window.w) + (p.x + x0);

	for (int j = y0; j < y1; j++) {
		uint32_t *fg_row = fg_ptr;
		uint32_t *bg_row = bg_ptr;

		for (int i = x0; i < x1; i++) {
			uint32_t fg_px = *fg_row++;
			uint32_t bg_px = *bg_row;

			*bg_row = blend_pixel_fast(fg_px, bg_px);
			bg_row++;
		}

		fg_ptr += p.w;
		bg_ptr += g->window.w;
	}
}

void	ft_camera_calibrate(t_game *g)
{
	(void)g;
	// if (g->window.w < g->map.w)
	// {
	// 	if (g->window.x < 0)
	// 		g->window.x = 0;
	// 	if (g->window.x > g->map.w - g->window.w)
	// 		g->window.x = g->map.w - g->window.w;
	// }
	// else
	// 	g->window.x = (g->map.w - g->window.w) / 2;
	// if (g->window.h < g->map.h) {
	// 	if (g->window.y < 0)
	// 		g->window.y = 0;
	// 	if (g->window.y > g->map.h - g->window.h)
	// 		g->window.y = g->map.h - g->window.h;
	// }
	// else
	// 	g->window.y = (g->window.h - g->map.h) / 2;
}

void	ft_camera_update(t_game *g)
{
	const int	x_offset = g->player.x - .5 * (g->window.w - g->player.w);
	const int	y_offset = g->player.y - .5 * (g->window.h - g->player.h);

	g->window.x = x_offset;
	g->window.y = y_offset;
	//if (g->player.x > g->window.x + g->window.w / 2.
	//	&& g->window.x + g->window.w < g->map.w)
	//	g->window.x = x_offset;
	//else if (g->player.x < g->window.x + g->window.w / 2.
	//	&& g->window.x > 0)
	//	g->window.x = x_offset;
	//if (g->player.y > g->window.y + g->window.h / 2.)
	//	g->window.y = y_offset;
	//else if (g->player.y < g->window.y + g->window.h / 2.)
	//	g->window.y = y_offset;
	ft_camera_calibrate(g);
}

uint32_t	ft_transform_mirror(t_game *g, uint32_t *buf, t_rect p)
{
	if (g->player.vx < 0)
		return buf[(p.y + 1) * p.w - p.x];
	return buf[p.y * p.w + p.x];
}

void	ft_render_player(t_game *g)
{
	t_vector t = translate(g,
		g->player.x - .5 * (TILE_SIZE - g->player.w),
		g->player.y - .5 * (TILE_SIZE - g->player.h));
	ft_image_transform(g, g->textures[TEX_PLAYER], (t_rect){
		t.x, t.y, TILE_SIZE, TILE_SIZE
	}, ft_transform_mirror);
}

void	render_button(t_game *g, t_rect btn, int color)
{
	for (int y = btn.y; y < btn.y + btn.h; y++)
		for (int x = btn.x; x < btn.x + btn.w; x++)
			mlx_pixel_put(g->mlx, g->win, x, y, color);
}

int	render_menu(t_game *g)
{
	t_rect btn = { 100, 150, 135, 33 };
	struct s_mouse mouse = g->input.mouse;
	bool is_inside = mouse.x > btn.x && mouse.x <= btn.x + btn.w
		&& mouse.y >= btn.y && mouse.y <= btn.y + btn.h;
	render_button(g, btn, 0x808080);
	if (is_inside && mouse.left)
		g->state.scene = SCENE_LEVEL;
	mlx_string_put(g->mlx, g->win, btn.x + 10, btn.y + 20, 0xffffff, "S T A R T   G A M E");
	mlx_string_put(g->mlx, g->win, btn.x + 11, btn.y + 21, 0xffffff, "S T A R T   G A M E");
	mlx_string_put(g->mlx, g->win, btn.x + 11, btn.y + 20, 0xffffff, "S T A R T   G A M E");
	mlx_string_put(g->mlx, g->win, btn.x + 10, btn.y + 21, 0xffffff, "S T A R T   G A M E");
	return (0);
}

int	render_pause_menu(t_game *g)
{
	t_rect btn = { 100, 150, 88, 33 };
	struct s_mouse mouse = g->input.mouse;
	bool is_inside = mouse.x > btn.x && mouse.x <= btn.x + btn.w
		&& mouse.y >= btn.y && mouse.y <= btn.y + btn.h;
	if (is_inside && mouse.left)
		g->state.scene = SCENE_LEVEL;
	render_button(g, btn, 0x808080);
	mlx_string_put(g->mlx, g->win, btn.x + 10, btn.y + 20, 0xffffff, "R E S U M E");
	mlx_string_put(g->mlx, g->win, btn.x + 11, btn.y + 21, 0xffffff, "R E S U M E");
	mlx_string_put(g->mlx, g->win, btn.x + 11, btn.y + 20, 0xffffff, "R E S U M E");
	mlx_string_put(g->mlx, g->win, btn.x + 10, btn.y + 21, 0xffffff, "R E S U M E");
	return (0);
}

int	render(t_game *g)
{
	if (!g->state.should_render)
		return (0);
	g->state.frame_count++;
	g->state.should_render = 0;
	if (g->state.scene == SCENE_MAIN_MENU)
		return (render_menu(g));
	if (g->state.scene == SCENE_PAUSE_MENU)
		return (render_pause_menu(g));
	ft_player_update(g);
	ft_camera_update(g);
	t_rect p = {
		-g->window.x * PARALLAX_CONSTANT,
		-g->window.y * PARALLAX_CONSTANT,
		g->map.w * PARALLAX_CONSTANT + g->window.w * PARALLAX_CONSTANT * 0.25,
		g->map.h * PARALLAX_CONSTANT + g->window.h * PARALLAX_CONSTANT * 0.25,
	};
	ft_image_to_vbuffer(g, g->parallaxes[0], p);
	ft_render_map(g);
	ft_render_player(g);
	if (g->debug_mode)
		render_hitboxes(g);
	mlx_put_image_to_window(g->mlx, g->win, g->frame, 0, 0);
	return (0);
}



int	ft_init_player(t_game *g)
{
	g->player.w = 32;
	g->player.h = 56;
	g->window.w = WINDOW_W;
	g->window.h = WINDOW_H;
	g->map.w = g->opt.map_width * TILE_SIZE;
	g->map.h = g->opt.map_height * TILE_SIZE;
	return (0);
}

bool	is_out_of_bounds(t_game *g, int tx, int ty)
{
	return (tx < 0 || ty < 0 || tx >= g->opt.map_width || ty >= g->opt.map_height);
}

int	ft_render_map(t_game *g)
{
	int	x0 = g->window.x / TILE_SIZE - 1;
	int	x1 = (g->window.x + g->window.w) / TILE_SIZE + 1;
	int	y0 = g->window.y / TILE_SIZE - 1;
	int	y1 = (g->window.y + g->window.h) / TILE_SIZE + 1;

	for (int j = y0; j < y1; j++) {
		for (int i = x0; i < x1; i++) {
			t_vector t = translate(g, i * TILE_SIZE, j * TILE_SIZE);
			if (is_out_of_bounds(g, i, j))
			{
				ft_image_to_vbuffer(g, g->textures[TEX_EMPTY], (t_rect){
					t.x, t.y, TILE_SIZE, TILE_SIZE });
				continue;
			}
			if (g->map_matrix[j][i] == 'E')
				ft_image_to_vbuffer(g, g->textures[TEX_EXIT], (t_rect){
					t.x, t.y, TILE_SIZE, TILE_SIZE });
			if (g->map_matrix[j][i] == 'C')
				ft_image_to_vbuffer(g, g->textures[TEX_COLLECTIBLE], (t_rect){
					t.x + 16, t.y + 32, 32, 32 });
			if (!is_wall(g, i, j)) continue ;
			int mask = compute_texture_mask(g, i, j);
			int tex_idx = get_texture_index(mask);
			ft_image_to_vbuffer(g, g->textures[tex_idx], (t_rect){
				t.x, t.y, TILE_SIZE, TILE_SIZE
			});
		}
	}
	return (0);
}

void	ft_debug(char *info)
{
	ft_printf("\e[95m[DBG]\e[m %s\n", info);
}

int	ft_init_parallax(t_game *g)
{
	float parallax = 1;
	int _;
	for (int k = 0; k < PARALLAX_LAYERS; k++)
	{
		parallax *= PARALLAX_CONSTANT;
		int width = g->map.w * parallax + PARALLAX_CONSTANT * g->window.w * 0.25;
		int height = g->map.h * parallax + PARALLAX_CONSTANT * g->window.h * 0.25;
		printf("Trying to init %ix%i parallax\n", width, height);
		g->parallaxes[k] = mlx_new_image(g->mlx, width, height);
		if (!g->parallaxes[k])
			return (1);
		uint32_t *buf = (uint32_t *)mlx_get_data_addr(g->parallaxes[k], &_, &_, &_);
		if (!buf)
			return (1);
		for (int j = 0; j < height; j++) {
			for (int i = 0; i < width; i++) {
				uint8_t r = 255. * i / width;
				uint8_t g = 255. * j / height;
				uint8_t b = 128.;
				uint32_t color = (r << 16) | (g << 8) | b;
				buf[j * width + i] = color;
			}
		}
	}
	return (0);
}

int	ft_init_renderer(t_game *g)
{
	ft_printf("sizeof(t_game): %i\n", sizeof(t_game));
	g->mlx = mlx_init();
	if (!g->mlx)
		return (1);
	ft_debug("Initialized MLX");
	if (ft_load_textures(g))
		return (1);
	ft_debug("Loaded textures");
	ft_init_player(g);
	ft_debug("Initialized player");
	g->win = mlx_new_window(g->mlx, WINDOW_W, WINDOW_H,
		"So Long");
	if (!g->win)
	{
		ft_printf("\e[91m[ERR]\e[m Failed to create a window\n");
		free(g->mlx);
		return (1);
	}
	ft_debug("Created window");
	g->frame = mlx_new_image(g->mlx, WINDOW_W, WINDOW_H);
	if (!g->frame)
	{
		ft_printf("\e[91m[ERR]\e[m Failed to generate frame\n");
	}
	ft_debug("Prepared frame");
	if (ft_init_parallax(g))
	{
		ft_printf("\e[91m[ERR]\e[m Failed to initialize parallax: %m\n");
		return (1);
	}
	ft_debug("Initialized parallax");
	launch_threads(g);
	ft_debug("Launched threads");
	ft_register_hooks(g);
	ft_debug("Registered hooks");
	return (0);
}

void generate_map(t_game *g, unsigned int seed)
{
	ft_printf("\e[95m[DBG]\e[m Seed %i\n", seed);
	for (int y = 0; y < g->opt.map_height; y++) {
		for (int x = 0; x < g->opt.map_width; x++) {
			g->map_matrix[y][x] = '1';
		}
	}

	int x = g->opt.map_width / 2;
	int y = g->opt.map_height / 2;
	int has_player = 0;

	g->map_matrix[y][x] = '0';

	for (int i = 0; i < g->opt.map_width * g->opt.map_height; i++) {
		int dir = ft_rand(seed) % 4;
		switch (dir) {
			case 0: if (x > 1) x--; break;
			case 1: if (x < g->opt.map_width - 2) x++; break;
			case 2: if (y > 1) y--; break;
			case 3: if (y < g->opt.map_height - 2) y++; break;
		}
		if (x >= g->opt.map_width - 1 || y >= g->opt.map_height - 1)
			ft_printf("\e[93m[WRN]\e[m invalid position %ix%i\n", g->opt.map_width, g->opt.map_height, x, y);
		if (g->map_matrix[y][x] == '1')
			g->map_matrix[y][x] = '0';
	}
	set_player_position(g, 1, 98);

	for (int y = 0; y < g->opt.map_height - 1; y++) {
		for (int x = 0; x < g->opt.map_width; x++) {
			if (g->map_matrix[y][x] == '0' && g->map_matrix[y + 1][x] == '1') {
				if (ft_rand(seed) % 5 == 0) {
					g->map_matrix[y][x] = 'C';
					if (!has_player)
					{
						set_player_position(g, x, y);
						g->map_matrix[y][x] = 'P';
						has_player = 1;
					}
				}
			}
		}
	}
}

void print_map(t_game *g) {
	for (int y = 0; y < g->opt.map_height; y++) {
		for (int x = 0; x < g->opt.map_width; x++) {
			printf("%c", g->map_matrix[y][x]);
		}
		printf("\n");
	}
}

int	allocate_map(t_game *g)
{
	g->map_matrix = malloc(sizeof(char *) * g->opt.map_height);
	if (!g->map_matrix)
		return (1);
	for (int i = 0; i < g->opt.map_height; i++)
	{
		g->map_matrix[i] = malloc(g->opt.map_width);
		if (!g->map_matrix[i])
			return (1);
	}
	return 0;
}

#include <errno.h>

int	main(int argc, char *argv[])
{
	t_game	g;
	int		seed;

	seed = ft_time(NULL);
	if (argc == 2)
		seed = ft_atoi_safe(argv[1]);
	if (errno != 0)
	{
		ft_printf("\e[91m[ERR]\e[m Invalid seed\n");
		return (1);
	}
	ft_memset(&g, 0, sizeof(t_game));
	options_init_default(&g.opt);
	allocate_map(&g);
	ft_printf("\e[94m[INF]\e[m Starting game\n");
	g.state.is_running = 1;
	generate_map(&g, seed);
	if (ft_init_renderer(&g))
	{
		cleanup(&g);
		return (1);
	}
	mlx_loop_hook(g.mlx, render, &g);
	mlx_loop(g.mlx);
	on_destroy(&g);
	return (0);
}
