/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smamalig <smamalig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 10:34:52 by smamalig          #+#    #+#             */
/*   Updated: 2025/04/13 22:53:35 by smamalig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
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
				render_hitbox(g, TEX_SNACK_0, x, y);
			else if (g->map_matrix[y][x] == '1')
				render_hitbox(g, tex_idx, x, y);
			else if (g->map_matrix[y][x] == 'E')
				render_hitbox(g, TEX_EXIT, x, y);
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

void	ft_collect_item(t_game *g)
{
	int	x0 = g->player.x / TILE_SIZE;
	int	x1 = (g->player.x + g->player.w) / TILE_SIZE;
	int	y0 = g->player.y / TILE_SIZE;
	int y1 = (g->player.y + g->player.h) / TILE_SIZE;

	for (int y = y0; y <= y1; y++) {
		for (int x = x0; x <= x1; x++) {
			t_hitbox hitbox = ft_absolute_hitbox(g, x, y);
			if (g->player.x + .5 * g->player.w < hitbox.l
				|| g->player.y + .5 * g->player.h < hitbox.t
				|| g->player.x + .5 * g->player.w > hitbox.r
				|| g->player.y + .5 * g->player.h > hitbox.b)
				continue ;
			if (g->map_matrix[y][x] == 'C') {
				g->state.snacks_eaten++;
				g->map_matrix[y][x] = '0';
			}
			if (g->map_matrix[y][x] == 'E' && g->state.snacks_eaten == g->state.snack_count) {
				t_random rand = ft_srand(ft_time(NULL));
				generate_map(g, &rand);
			}
		}
	}
}

void	render_text(t_game *g, t_point pos, const char *text, int color);

void	ft_print_movement(t_game *g)
{
	const int	px = (g->player.px + .5 * g->player.w) / TILE_SIZE;
	const int	py = (g->player.py + .5 * g->player.h) / TILE_SIZE;
	const int	cx = (g->player.x + .5 * g->player.w) / TILE_SIZE;
	const int	cy = (g->player.y + .5 * g->player.h) / TILE_SIZE;

	char buf[16];
	ft_snprintf(buf, 16, "%i moves", g->state.move_count);
	render_text(g, (t_point){1190 - 30 * ft_strlen(buf), 10}, buf, 0x80ffffff);
	if (px == cx && py == cy)
		return ;
	g->state.move_count++;
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
	ft_collect_item(g);
}

void	ft_image_transform(t_game *g, void *dst, void *src, t_rect p,
	uint32_t (*transform)(t_game *g, uint32_t *buf, t_rect p))
{
	int		_;
	void	*bg = mlx_get_data_addr(dst, &_, &_, &_);
	void	*fg = mlx_get_data_addr(src, &_, &_, &_);

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

void	ft_camera_update(t_game *g)
{
	g->window.x = g->player.x - .5 * (g->window.w - g->player.w);
	g->window.y = g->player.y - .5 * (g->window.h - g->player.h);
}

uint32_t	ft_transform_mirror(t_game *g, uint32_t *buf, t_rect p)
{
	if (g->player.vx < 0)
		return buf[(p.y + 1) * p.w - p.x];
	return buf[p.y * p.w + p.x];
}

uint32_t	ft_transform_scale_down(t_game *game, uint32_t *buf, t_rect p)
{
	(void)game;
	const int src_width = 64;
	float src_x = p.x * (64.0f / 48.0f);
	float src_y = p.y * (64.0f / 48.0f);
	int x0 = src_x;
	int y0 = src_y;
	int x1 = src_x + 1;
	int y1 = src_y + 1;
	x1 = (x1 >= 64) ? 63 : x1;
	y1 = (y1 >= 64) ? 63 : y1;
	float dx = src_x - x0;
	float dy = src_y - y0;
	uint32_t px00 = buf[y0 * src_width + x0];
	uint32_t px01 = buf[y0 * src_width + x1];
	uint32_t px10 = buf[y1 * src_width + x0];
	uint32_t px11 = buf[y1 * src_width + x1];
	#define INTERP_CHANNEL(channel, shift) \
		( (1 - dy) * ( (1 - dx) * ((px00 >> shift) & 0xFF) + dx * ((px01 >> shift) & 0xFF) ) + \
		dy * ( (1 - dx) * ((px10 >> shift) & 0xFF) + dx * ((px11 >> shift) & 0xFF) ) )
	float a = INTERP_CHANNEL(24, 24);
	float r = INTERP_CHANNEL(16, 16);
	float g = INTERP_CHANNEL(8, 8);
	float b = INTERP_CHANNEL(0, 0);
	uint8_t alpha = (uint8_t)(a + 0.5f);
	uint8_t red   = (uint8_t)(r + 0.5f);
	uint8_t green = (uint8_t)(g + 0.5f);
	uint8_t blue  = (uint8_t)(b + 0.5f);

	return (alpha << 24) | (red << 16) | (green << 8) | blue;
}

int	get_animation_state(t_game *g)
{
	const float epsilon = 0.1;
	static int walk = TEX_PLAYER_WALK_0;
	static int idle = TEX_PLAYER_IDLE_0;
	static int dash = TEX_PLAYER_DASH_0;
	if (g->state.should_dash) {
		idle = TEX_PLAYER_IDLE_0;
		walk = TEX_PLAYER_WALK_0;
		dash = dash == TEX_PLAYER_DASH_15 ? TEX_PLAYER_DASH_0 : dash + 1;
		return dash;
	} else if (g->player.vx > epsilon || g->player.vx < -epsilon) {
		idle = TEX_PLAYER_IDLE_0;
		dash = TEX_PLAYER_DASH_0;
		if (g->state.frame_count % 6 == 0)
			walk = walk == TEX_PLAYER_WALK_19 ? TEX_PLAYER_WALK_0 : walk + 1;
		return walk;
	} else {
		walk = TEX_PLAYER_WALK_0;
		dash = TEX_PLAYER_DASH_0;
		if (g->state.frame_count % 6 == 0)
			idle = idle == TEX_PLAYER_IDLE_19 ? TEX_PLAYER_IDLE_0 : idle + 1;
		return idle;
	}
}

void	ft_render_player(t_game *g)
{
	t_vector t = translate(g,
		g->player.x - .5 * (TILE_SIZE - g->player.w),
		g->player.y - .5 * (TILE_SIZE - g->player.h));
	int	tex = get_animation_state(g);
	ft_image_transform(g, g->frame, g->textures[tex], (t_rect){
		t.x, t.y, TILE_SIZE, TILE_SIZE }, ft_transform_mirror);
}

void	draw_char(t_game *g, t_point pos, char c, int color)
{
	int	_;
	uint32_t	*bg = (uint32_t *)mlx_get_data_addr(g->frame, &_, &_, &_);
	uint32_t	*fg = (uint32_t *)mlx_get_data_addr(g->textures[TEX_FONT], &_, &_, &_);

	// 00101010
	// 00001111
	// 00001010

	int x_offset = (c & 0xf) * 30;
	int y_offset = ((c >> 4) - 2) * 30;

	for (int j = 0; j < 30; j++) {
		for (int i = 0; i < 30; i++) {
			int idx = (pos.y + j) * g->window.w + pos.x + i;
			if (fg[(y_offset + j) * 480 + x_offset + i] == 0xffffff)
				bg[idx] = blend_pixel_fast(color, bg[idx]);
		}
	}
}

void	render_text(t_game *g, t_point pos, const char *text, int color)
{
	int i = 0;
	while (*text) {
		draw_char(g, (t_point){ pos.x + i, pos.y }, *text, color);
		i += 30;
		text++;
	}
}

uint32_t	ft_transform_gaussian_blur(t_game *game, uint32_t *buf, t_rect p)
{
	(void)game;
	static const int kernel[5][5] = {
		{1,  4,  6,  4, 1},
		{4, 16, 20, 16, 4},
		{6, 20, 36, 20, 6},
		{4, 16, 20, 16, 4},
		{1,  4,  6,  4, 1},
	};
	uint32_t sum_r = 0;
	uint32_t sum_g = 0;
	uint32_t sum_b = 0;

	for (int dy = -2; dy <= 2; dy++) {
		for (int dx = -2; dx <= 2; dx++) {
			int sample_x = p.x + dx;
			int sample_y = p.y + dy;
			if (sample_x < 0)
				sample_x = 0;
			else if (sample_x >= p.w)
				sample_x = p.w - 1;
			if (sample_y < 0)
				sample_y = 0;
			else if (sample_y >= p.h)
				sample_y = p.h - 1;
			uint32_t pixel = buf[sample_y * p.w + sample_x];
			int r = (pixel >> 16) & 0xFF;
			int g = (pixel >> 8) & 0xFF;
			int b = (pixel >> 0) & 0xFF;
			int weight = kernel[dy + 2][dx + 2];
			sum_r += r * weight;
			sum_g += g * weight;
			sum_b += b * weight;
		}
	}
	sum_r >>= 8;
	sum_g >>= 8;
	sum_b >>= 8;
	return (sum_r << 16) | (sum_g << 8) | sum_b;
}

uint32_t	ft_transform_none(t_game *g, uint32_t *buf, t_rect p)
{
	(void)g;
	return buf[p.y * p.w + p.x];
}

uint32_t	ft_transform_ignore_alpha(t_game *g, uint32_t *buf, t_rect p)
{
	(void)g;
	return 0xffffff & buf[p.y * p.w + p.x];
}

void	new_button(t_game *g, const char *text, t_point btn, void (*on_click)(t_game *g))
{
	static bool clicked = false;
	struct s_mouse mouse = g->input.mouse;
	bool is_inside = mouse.x > btn.x && mouse.x <= btn.x + 30 * (int)ft_strlen(text)
		&& mouse.y >= btn.y && mouse.y <= btn.y + 30;
	render_text(g, btn, text, is_inside ? 0x80ff00 : 0xffffff);
	if (is_inside && mouse.left)
		clicked = true;
	if (is_inside && clicked && !mouse.left)
	{
		if (on_click)
			on_click(g);
		clicked = false;
	}
}

struct	s_link_render_options
{
	const char	*url;
	const char	*label;
	t_point		pos;
	int			color;
	int			hover_color;
};

void	render_link(t_game *g, struct s_link_render_options opt)
{
	static bool clicked = false;
	struct s_mouse mouse = g->input.mouse;
	bool is_inside = mouse.x > opt.pos.x && opt.pos.x <= opt.pos.x + 30 * (int)ft_strlen(opt.label)
		&& mouse.y >= opt.pos.y && mouse.y <= opt.pos.y + 30;
	render_text(g, opt.pos, opt.label, is_inside ? opt.hover_color : opt.color);
	if (is_inside && mouse.left)
		clicked = true;
	char command[0xff];
	ft_snprintf(command, 0xff, "xdg-open %s", opt.url);
	if (is_inside && clicked && !mouse.left)
	{
		system(command);
		clicked = false;
	}
}

void	start_click(t_game *g)
{
	g->state.scene = SCENE_LEVEL;
}

void	resume_click(t_game *g)
{
	g->state.scene = SCENE_LEVEL;
}

void	options_click(t_game *g)
{
	g->state.scene = SCENE_OPTIONS_MENU;
}

void	main_options_click(t_game *g)
{
	g->state.scene = SCENE_MAIN_OPTIONS_MENU;
}

void	exit_click(t_game *g)
{
	on_destroy(g);
}

void	pause_click(t_game *g)
{
	g->state.scene = SCENE_PAUSE_MENU;
}

void	credits_click(t_game *g)
{
	g->state.scene = SCENE_CREDITS;
}

void	fps_click(t_game *g)
{
	if (g->opt.fps == 60)
		g->opt.fps = 120;
	else if (g->opt.fps == 120)
		g->opt.fps = 30;
	else
		g->opt.fps = 60;
}

void	gvt_click(t_game *g)
{
	if (g->opt.gravity == 0.2f)
		g->opt.gravity = 0.4f;
	else if (g->opt.gravity == 0.4f)
		g->opt.gravity = 0.1f;
	else
		g->opt.gravity = 0.2f;
}

void	dbg_click(t_game *g)
{
	g->debug_mode ^= 1;
}

void	menu_click(t_game *g)
{
	g->state.scene = SCENE_MAIN_MENU;
}

void	new_game_click(t_game *g)
{
	t_random rand = ft_srand(ft_time(NULL));
	generate_map(g, &rand);
	g->state.scene = SCENE_LEVEL;
}

static bool blur_rendered = false;

void	render_blur(t_game *g)
{
	if (!blur_rendered) {
		blur_rendered = true;
		ft_image_transform(g, g->frame2, g->frame, (t_rect){ 0, 0, WINDOW_W, WINDOW_H },
			ft_transform_gaussian_blur);
		ft_image_transform(g, g->frame, g->frame2, (t_rect){ 0, 0, WINDOW_W, WINDOW_H },
			ft_transform_gaussian_blur);
		ft_image_transform(g, g->frame2, g->frame, (t_rect){ 0, 0, WINDOW_W, WINDOW_H },
			ft_transform_gaussian_blur);
	}
	ft_image_transform(g, g->frame, g->frame2, (t_rect){ 0, 0, WINDOW_W, WINDOW_H },
		ft_transform_ignore_alpha);
}

void	render_game_name(t_game *g)
{
	render_text(g, (t_point){ 30, 30 }, "so long", 0x8080cc);
	render_text(g, (t_point){ 330, 30 }, "v0.0.3", 0xc0ffffff);
}

int	render_menu(t_game *g)
{
	ft_camera_update(g);
	t_rect p = {
		-g->window.x * PARALLAX_CONSTANT,
		-g->window.y * PARALLAX_CONSTANT,
		g->map.w * PARALLAX_CONSTANT + g->window.w * PARALLAX_CONSTANT * 0.25,
		g->map.h * PARALLAX_CONSTANT + g->window.h * PARALLAX_CONSTANT * 0.25,
	};
	ft_image_to_vbuffer(g, g->parallaxes[0], p);
	ft_render_player(g);
	ft_render_map(g);
	if (g->debug_mode)
		render_hitboxes(g);
	render_blur(g);
	render_game_name(g);
	new_button(g, "continue", (t_point){100, 200}, start_click);
	new_button(g, "new game", (t_point){ 100, 250 }, new_game_click);
	new_button(g, "options", (t_point){ 100, 300 }, main_options_click);
	new_button(g, "credits", (t_point){ 100, 350 }, credits_click);
	new_button(g, "quit", (t_point){ 100, 450 }, exit_click);
	mlx_put_image_to_window(g->mlx, g->win, g->frame, 0, 0);
	return (0);
}

const char *get_gvt_preset(t_game *g)
{
	if (g->opt.gravity == 0.2f)
		return "normal";
	else if (g->opt.gravity == 0.4f)
		return "high";
	else if (g->opt.gravity == 0.1f)
		return "low";
	else
		return "(error)";
}

int	render_credits(t_game *g)
{
	render_blur(g);
	render_text(g, (t_point){ 495, 60 }, "so long", 0x8040ff);
	render_text(g, (t_point){ 510, 110 }, "v0.0.3", 0xcccccc);
	render_text(g, (t_point){ 450, 210 }, "Developers", 0xffff00);
	render_link(g, (struct s_link_render_options){ .hover_color = 0x0080ff,
		.pos = { 420, 260 }, .label = "ms-is-coding",
		.url = "https://github.com/ms-is-coding", .color = 0xcccccc });
	render_text(g, (t_point){ 525, 360 }, "Fonts", 0xffff00);
	render_link(g, (struct s_link_render_options){ .hover_color = 0x0080ff,
		.pos = { 285, 410 }, .label = "Daydream by DoubleGum",
		.url = "https://www.dafont.com/daydream-3.font", .color = 0xcccccc });
	render_text(g, (t_point){ 510, 510 }, "Assets", 0xffff00);
	render_link(g, (struct s_link_render_options){ .hover_color = 0x0080ff,
		.url = "https://maaot.itch.io/mossy-cavern", .color = 0xcccccc,
		.label = "Mossy Cavern by Maaot", .pos = { 285, 560 } });
	render_link(g, (struct s_link_render_options){ .hover_color = 0x0080ff,
		.pos = { 105, 610 }, .label = "Free Pixel foods by ghostpixxells",
		.url = "https://ghostpixxells.itch.io/pixelfood", .color = 0xcccccc });
	new_button(g, "return", (t_point){ 510, 710 }, menu_click);
	mlx_put_image_to_window(g->mlx, g->win, g->frame, 0, 0);
	return (0);
}

int	render_main_options_menu(t_game *g)
{
	render_blur(g);

	char	fps_text[24];
	char	gvt_text[24];
	char	dbg_text[24];
	ft_snprintf(fps_text, 24, "fps        - %i", g->opt.fps);
	ft_snprintf(gvt_text, 24, "gravity    - %s", get_gvt_preset(g));
	ft_snprintf(dbg_text, 24, "hitboxes   - %s", g->debug_mode ? "on" : "off");
	render_game_name(g);
	new_button(g, fps_text, (t_point){ 100, 200 }, fps_click);
	new_button(g, gvt_text, (t_point){ 100, 250 }, gvt_click);
	new_button(g, dbg_text, (t_point){ 100, 300 }, dbg_click);
	new_button(g, "return", (t_point){ 100, 400 }, menu_click);
	mlx_put_image_to_window(g->mlx, g->win, g->frame, 0, 0);
	return (0);
}

int	render_options_menu(t_game *g)
{
	render_blur(g);

	char	fps_text[24];
	char	gvt_text[24];
	char	dbg_text[24];
	ft_snprintf(fps_text, 24, "fps        - %i", g->opt.fps);
	ft_snprintf(gvt_text, 24, "gravity    - %s", get_gvt_preset(g));
	ft_snprintf(dbg_text, 24, "hitboxes   - %s", g->debug_mode ? "on" : "off");
	render_game_name(g);
	new_button(g, fps_text, (t_point){ 100, 200 }, fps_click);
	new_button(g, gvt_text, (t_point){ 100, 250 }, gvt_click);
	new_button(g, dbg_text, (t_point){ 100, 300 }, dbg_click);
	new_button(g, "return", (t_point){ 100, 400 }, pause_click);
	mlx_put_image_to_window(g->mlx, g->win, g->frame, 0, 0);
	return (0);
}

int	render_pause_menu(t_game *g)
{
	render_blur(g);
	render_game_name(g);
	new_button(g, "resume", (t_point){ 100, 200 }, resume_click);
	new_button(g, "options", (t_point){ 100, 250 }, options_click);
	new_button(g, "main menu", (t_point){ 100, 350 }, menu_click);
	mlx_put_image_to_window(g->mlx, g->win, g->frame, 0, 0);
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
	if (g->state.scene == SCENE_OPTIONS_MENU)
		return (render_options_menu(g));
	if (g->state.scene == SCENE_CREDITS)
		return (render_credits(g));
	if (g->state.scene == SCENE_MAIN_OPTIONS_MENU)
		return (render_main_options_menu(g));
	blur_rendered = false;
	ft_player_update(g);
	ft_camera_update(g);
	t_rect p = {
		-g->window.x * PARALLAX_CONSTANT,
		-g->window.y * PARALLAX_CONSTANT,
		g->map.w * PARALLAX_CONSTANT + g->window.w * PARALLAX_CONSTANT * 0.25,
		g->map.h * PARALLAX_CONSTANT + g->window.h * PARALLAX_CONSTANT * 0.25,
	};
	ft_image_to_vbuffer(g, g->parallaxes[0], p);
	ft_render_player(g);
	ft_render_map(g);
	char buf[12];
	ft_snprintf(buf, 12, "%i/%i", g->state.snacks_eaten, g->state.snack_count);
	render_text(g, (t_point){10, 10}, buf, 0x80ffffff);
	ft_print_movement(g);
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
				ft_image_to_vbuffer(g, g->textures[TEX_SNACK_0 + ((i + j) & 7)], (t_rect){
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
	g->frame2 = mlx_new_image(g->mlx, WINDOW_W, WINDOW_H);
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

#include <limits.h>

void	generate_map(t_game *g, t_random *rand)
{
	ft_printf("\e[94m[INF]\e[m Seed %i\n", rand->seed);
	for (int y = 0; y < g->opt.map_height; y++) {
		for (int x = 0; x < g->opt.map_width; x++) {
			g->map_matrix[y][x] = '1';
		}
	}

	g->state.move_count = 0;
	g->state.snack_count = -1;
	g->state.snacks_eaten = 0;
	t_u32 x = g->opt.map_width / 2;
	t_u32 y = g->opt.map_height / 2;
	bool	has_player = false;
	t_u32 last_x = 0;
	t_u32 last_y = 0;

	g->map_matrix[y][x] = '0';

	for (t_u32 i = 0; (int)i < g->opt.map_width * g->opt.map_height; i++) {
		int dir = ft_rand(rand) % 4;
		switch (dir) {
			case 0: if ((int)x > 1) x--; break;
			case 1: if ((int)x < g->opt.map_width - 2) x++; break;
			case 2: if ((int)y > 1) y--; break;
			case 3: if ((int)y < g->opt.map_height - 2) y++; break;
		}
		if ((int)x >= g->opt.map_width - 1 || (int)y >= g->opt.map_height - 1)
			ft_printf("\e[93m[WRN]\e[m invalid position %ix%i\n", g->opt.map_width, g->opt.map_height, x, y);
		if (g->map_matrix[y][x] == '1')
			g->map_matrix[y][x] = '0';
	}

	for (t_u32 y = 0; (int)y < g->opt.map_height - 1; y++) {
		for (t_u32 x = 0; (int)x < g->opt.map_width; x++) {
			if (g->map_matrix[y][x] == '0' && g->map_matrix[y + 1][x] == '1') {
				if (ft_rand(rand) % 5 == 0) {
					g->map_matrix[y][x] = 'C';
					last_x = x;
					last_y = y;
					g->state.snack_count++;
					if (!has_player)
					{
						set_player_position(g, x, y);
						g->state.snack_count--;
						g->map_matrix[y][x] = 'P';
						has_player = true;
					}
				}
			}
		}
	}

	g->map_matrix[last_y][last_x] = 'E';

	if (g->state.snack_count > 0)
		return ;
	rand->seed++;
	generate_map(g, rand);
}

void	print_map(t_game *g) {
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
	t_game		g;
	t_random	rand;

	rand = ft_srand(ft_time(NULL));
	if (argc == 2)
		rand = ft_srand(ft_atoi_safe(argv[1]));
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
	generate_map(&g, &rand);
	// print_map(&g);
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
