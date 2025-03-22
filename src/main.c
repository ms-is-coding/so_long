/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smamalig <smamalig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 10:34:52 by smamalig          #+#    #+#             */
/*   Updated: 2025/03/23 00:36:50 by smamalig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "so_long.h"
#include "mlx.h"

#include <X11/keysym.h>
#include <X11/X.h>
#include <stdatomic.h>
#include <stdint.h>

atomic_int	g_frame_count = 0;
atomic_int	g_running = 1;
atomic_int	g_should_render = 1;
int			g_debug_mode = 0;

static char map[MAP_SIZE][MAP_SIZE] = {
	"111111111111",
	"10P100000001",
	"101101010011",
	"100001010111",
	"111011010001",
	"101010010101",
	"100011010001",
	"110000000001",
	"110001000001",
	"100011100001",
	"101001000111",
	"111111111111",
};

static char texture_index_lookup[0x100] = {
[0x00] = TEX_PLATFORM,
[0x01] = TEX_PLATFORM,
[0x02] = TEX_PLATFORM,
[0x04] = TEX_PLATFORM,
[0x05] = TEX_PLATFORM,
[0x06] = TEX_PLATFORM,
[0x07] = TEX_PLATFORM,
[0x08] = TEX_PLATFORM,
[0x09] = TEX_PLATFORM,
[0x0a] = TEX_PLATFORM,
[0x0b] = TEX_PLATFORM,
[0x0c] = TEX_PLATFORM,
[0x0d] = TEX_PLATFORM,
[0x0e] = TEX_PLATFORM,
[0x0f] = TEX_PLATFORM,

[0x10] = TEX_PLATFORM_R,
[0x11] = TEX_PLATFORM_R,
[0x12] = TEX_PLATFORM_R,
[0x13] = TEX_PLATFORM_R,
[0x14] = TEX_PLATFORM_R,
[0x15] = TEX_PLATFORM_R,
[0x16] = TEX_PLATFORM_R,
[0x17] = TEX_PLATFORM_R,
[0x18] = TEX_PLATFORM_R,
[0x19] = TEX_PLATFORM_R,
[0x1a] = TEX_PLATFORM_R,
[0x1b] = TEX_PLATFORM_R,
[0x1c] = TEX_PLATFORM_R,
[0x1d] = TEX_PLATFORM_R,
[0x1e] = TEX_PLATFORM_R,
[0x1f] = TEX_PLATFORM_R,

[0x20] = TEX_PLATFORM_L,
[0x21] = TEX_PLATFORM_L,
[0x22] = TEX_PLATFORM_L,
[0x23] = TEX_PLATFORM_L,
[0x24] = TEX_PLATFORM_L,
[0x25] = TEX_PLATFORM_L,
[0x26] = TEX_PLATFORM_L,
[0x27] = TEX_PLATFORM_L,
[0x28] = TEX_PLATFORM_L,
[0x29] = TEX_PLATFORM_L,
[0x2a] = TEX_PLATFORM_L,
[0x2b] = TEX_PLATFORM_L,
[0x2c] = TEX_PLATFORM_L,
[0x2d] = TEX_PLATFORM_L,
[0x2e] = TEX_PLATFORM_L,
[0x2f] = TEX_PLATFORM_L,

[0x30] = TEX_PLATFORM_H,
[0x31] = TEX_PLATFORM_H,
[0x32] = TEX_PLATFORM_H,
[0x33] = TEX_PLATFORM_H,
[0x34] = TEX_PLATFORM_H,
[0x35] = TEX_PLATFORM_H,
[0x36] = TEX_PLATFORM_H,
[0x37] = TEX_PLATFORM_H,
[0x38] = TEX_PLATFORM_H,
[0x39] = TEX_PLATFORM_H,
[0x3a] = TEX_PLATFORM_H,
[0x3b] = TEX_PLATFORM_H,
[0x3c] = TEX_PLATFORM_H,
[0x3d] = TEX_PLATFORM_H,
[0x3e] = TEX_PLATFORM_H,
[0x3f] = TEX_PLATFORM_H,

[0x40] = TEX_PLATFORM_B,
[0x41] = TEX_PLATFORM_B,
[0x42] = TEX_PLATFORM_B,
[0x43] = TEX_PLATFORM_B,
[0x44] = TEX_PLATFORM_B,
[0x45] = TEX_PLATFORM_B,
[0x46] = TEX_PLATFORM_B,
[0x47] = TEX_PLATFORM_B,
[0x48] = TEX_PLATFORM_B,
[0x49] = TEX_PLATFORM_B,
[0x4a] = TEX_PLATFORM_B,
[0x4b] = TEX_PLATFORM_B,
[0x4c] = TEX_PLATFORM_B,
[0x4d] = TEX_PLATFORM_B,
[0x4e] = TEX_PLATFORM_B,
[0x4f] = TEX_PLATFORM_B,

[0x50] = TEX_LJUNC_BR,
[0x51] = TEX_CORNER_EXT_TL,
[0x52] = TEX_LJUNC_BR,
[0x53] = TEX_CORNER_EXT_TL,
[0x54] = TEX_LJUNC_BR,
[0x55] = TEX_CORNER_EXT_TL,
[0x56] = TEX_LJUNC_BR,
[0x57] = TEX_CORNER_EXT_TL,
[0x58] = TEX_LJUNC_BR,
[0x59] = TEX_CORNER_EXT_TL,
[0x5a] = TEX_LJUNC_BR,
[0x5b] = TEX_CORNER_EXT_TL,
[0x5c] = TEX_LJUNC_BR,
[0x5d] = TEX_CORNER_EXT_TL,
[0x5e] = TEX_LJUNC_BR,
[0x5f] = TEX_CORNER_EXT_TL,

[0x60] = TEX_LJUNC_BL,
[0x61] = TEX_LJUNC_BL,
[0x62] = TEX_CORNER_EXT_TR,
[0x70] = TEX_TJUNC_B,
[0x71] = TEX_HLJUNC_BL,
[0x72] = TEX_HLJUNC_BR,
[0x73] = TEX_WALL_BOTTOM,
[0x74] = TEX_TJUNC_B,
[0x75] = TEX_HLJUNC_BL,
[0x76] = TEX_HLJUNC_BR,
[0x77] = TEX_WALL_BOTTOM,
[0x78] = TEX_TJUNC_B,
[0x79] = TEX_HLJUNC_BL,
[0x7a] = TEX_HLJUNC_BR,
[0x7b] = TEX_WALL_BOTTOM,
[0x7c] = TEX_TJUNC_B,
[0x7d] = TEX_HLJUNC_BL,
[0x7e] = TEX_HLJUNC_BR,
[0x7f] = TEX_WALL_BOTTOM,

[0x80] = TEX_PLATFORM_T,
[0x81] = TEX_PLATFORM_T,
[0x82] = TEX_PLATFORM_T,
[0x83] = TEX_PLATFORM_T,
[0x84] = TEX_PLATFORM_T,
[0x85] = TEX_PLATFORM_T,
[0x86] = TEX_PLATFORM_T,
[0x87] = TEX_PLATFORM_T,
[0x88] = TEX_PLATFORM_T,
[0x89] = TEX_PLATFORM_T,
[0x8a] = TEX_PLATFORM_T,
[0x8b] = TEX_PLATFORM_T,
[0x8c] = TEX_PLATFORM_T,
[0x8d] = TEX_PLATFORM_T,
[0x8e] = TEX_PLATFORM_T,
[0x8f] = TEX_PLATFORM_T,

[0x90] = TEX_LJUNC_TR,
[0x91] = TEX_LJUNC_TR,
[0x92] = TEX_LJUNC_TR,
[0x93] = TEX_LJUNC_TR,
[0x94] = TEX_CORNER_EXT_BL,
[0x95] = TEX_CORNER_EXT_BL,
[0x96] = TEX_CORNER_EXT_BL,
[0x97] = TEX_CORNER_EXT_BL,
[0x98] = TEX_LJUNC_TR,
[0x99] = TEX_LJUNC_TR,
[0x9a] = TEX_LJUNC_TR,
[0x9b] = TEX_LJUNC_TR,
[0x9c] = TEX_CORNER_EXT_BL,
[0x9d] = TEX_CORNER_EXT_BL,
[0x9e] = TEX_CORNER_EXT_BL,
[0x9f] = TEX_CORNER_EXT_BL,

[0xa0] = TEX_LJUNC_TL,
[0xa1] = TEX_LJUNC_TL,
[0xa2] = TEX_LJUNC_TL,
[0xa3] = TEX_LJUNC_TL,
[0xa4] = TEX_LJUNC_TL,
[0xa5] = TEX_LJUNC_TL,
[0xa6] = TEX_LJUNC_TL,
[0xa7] = TEX_LJUNC_TL,
[0xa8] = TEX_CORNER_EXT_BR,
[0xa9] = TEX_CORNER_EXT_BR,
[0xaa] = TEX_CORNER_EXT_BR,
[0xab] = TEX_CORNER_EXT_BR,
[0xac] = TEX_CORNER_EXT_BR,
[0xad] = TEX_CORNER_EXT_BR,
[0xae] = TEX_CORNER_EXT_BR,
[0xaf] = TEX_CORNER_EXT_BR,

[0xb0] = TEX_TJUNC_T,
[0xb1] = TEX_TJUNC_T,
[0xb2] = TEX_TJUNC_T,
[0xb3] = TEX_TJUNC_T,
[0xb4] = TEX_HLJUNC_TL,
[0xb5] = TEX_HLJUNC_TL,
[0xb6] = TEX_HLJUNC_TL,
[0xb7] = TEX_HLJUNC_TL,
[0xb8] = TEX_HLJUNC_TR,
[0xb9] = TEX_HLJUNC_TR,
[0xba] = TEX_HLJUNC_TR,
[0xbb] = TEX_HLJUNC_TR,
[0xbc] = TEX_WALL_TOP,
[0xbd] = TEX_WALL_TOP,
[0xbe] = TEX_WALL_TOP,
[0xbf] = TEX_WALL_TOP,

[0xc0] = TEX_PLATFORM_V,
[0xc1] = TEX_PLATFORM_V,
[0xc2] = TEX_PLATFORM_V,
[0xc3] = TEX_PLATFORM_V,
[0xc4] = TEX_PLATFORM_V,
[0xc5] = TEX_PLATFORM_V,
[0xc6] = TEX_PLATFORM_V,
[0xc7] = TEX_PLATFORM_V,
[0xc8] = TEX_PLATFORM_V,
[0xc9] = TEX_PLATFORM_V,
[0xca] = TEX_PLATFORM_V,
[0xcb] = TEX_PLATFORM_V,
[0xcc] = TEX_PLATFORM_V,
[0xcd] = TEX_PLATFORM_V,
[0xce] = TEX_PLATFORM_V,
[0xcf] = TEX_PLATFORM_V,

[0xd0] = TEX_TJUNC_R,
[0xd1] = TEX_VLJUNC_TR,
[0xd2] = TEX_TJUNC_R,
[0xd3] = TEX_VLJUNC_TR,
[0xd4] = TEX_VLJUNC_BR,
[0xd5] = TEX_WALL_RIGHT,
[0xd6] = TEX_VLJUNC_BR,
[0xd7] = TEX_WALL_RIGHT,
[0xd8] = TEX_TJUNC_R,
[0xd9] = TEX_VLJUNC_TR,
[0xda] = TEX_TJUNC_R,
[0xdb] = TEX_VLJUNC_TR,
[0xdc] = TEX_VLJUNC_BR,
[0xdd] = TEX_WALL_RIGHT,
[0xde] = TEX_VLJUNC_BR,
[0xdf] = TEX_WALL_RIGHT,

[0xe0] = TEX_TJUNC_L,
[0xe1] = TEX_TJUNC_L,
[0xe2] = TEX_VLJUNC_TL,
[0xe3] = TEX_VLJUNC_TL,
[0xe4] = TEX_TJUNC_L,
[0xe5] = TEX_TJUNC_L,
[0xe6] = TEX_VLJUNC_TL,
[0xe7] = TEX_VLJUNC_TL,
[0xe8] = TEX_VLJUNC_BL,
[0xe9] = TEX_VLJUNC_BL,
[0xea] = TEX_WALL_LEFT,
[0xeb] = TEX_WALL_LEFT,
[0xec] = TEX_VLJUNC_BL,
[0xed] = TEX_VLJUNC_BL,
[0xee] = TEX_WALL_LEFT,
[0xef] = TEX_WALL_LEFT,

[0xf0] = TEX_JUNC_CROSS,
[0xf1] = TEX_CORNERS_XBR,
[0xf2] = TEX_CORNERS_XBL,
[0xf3] = TEX_CORNER_TJUNC_T,
[0xf4] = TEX_CORNERS_XTR,
[0xf5] = TEX_CORNER_TJUNC_L,
[0xf6] = TEX_DIAG_TRBL,
[0xf7] = TEX_CORNER_TL,
[0xf8] = TEX_CORNERS_XTL,
[0xf9] = TEX_DIAG_TLBR,
[0xfa] = TEX_CORNER_TJUNC_R,
[0xfb] = TEX_CORNER_TR,
[0xfc] = TEX_CORNER_TJUNC_B,
[0xfd] = TEX_CORNER_BL,
[0xfe] = TEX_CORNER_BR,
[0xff] = TEX_EMPTY,
};

uint32_t	blend_pixel(uint32_t fg, uint32_t bg)
{
    float	alpha;
	uint8_t	r;
	uint8_t	g;
	uint8_t	b;

	alpha = 1 - ((fg >> 24) & 0xFF) / 255.0f;
	r = ((fg >> 16) & 0xFF) * alpha + ((bg >> 16) & 0xFF) * (1 - alpha);
	g = ((fg >> 8) & 0xFF) * alpha + ((bg >> 8) & 0xFF) * (1 - alpha);
	b = (fg & 0xFF) * alpha + (bg & 0xFF) * (1 - alpha);
	return ((r << 16) | (g << 8) | b);
}

bool is_wall(int tx, int ty)
{
	return (tx < 0 || tx >= MAP_SIZE || ty < 0
		|| ty >= MAP_SIZE || map[ty][tx] == '1');
}

int	compute_texture_mask(int x, int y)
{
	int	mask;

	mask = 0;
	mask |= is_wall(x, y - 1) << 7;
	mask |= is_wall(x, y + 1) << 6;
	mask |= is_wall(x - 1, y) << 5;
	mask |= is_wall(x + 1, y) << 4;
	mask |= is_wall(x - 1, y - 1) << 3;
	mask |= is_wall(x + 1, y - 1) << 2;
	mask |= is_wall(x - 1, y + 1) << 1;
	mask |= is_wall(x + 1, y + 1) << 0;
	return (mask);
}

int	get_texture_index(int mask)
{

	return (texture_index_lookup[mask]);
}

void *get_texture(t_renderer *renderer, int x, int y) {
    if (!is_wall(x, y)) return NULL;

	int mask = compute_texture_mask(x, y);
	return renderer->textures[get_texture_index(mask)];
}

void	*counter_thread()
{
	int	frames = 0;
	while (g_running) {
		ft_usleep(1000000 / 60);
		g_should_render = 1;
		frames++;
		if (frames == 60) {
			ft_printf("\rFPS: %i", g_frame_count);
			frames = 0;
			g_frame_count = 0;
		}
	}
	return NULL;
}


int	cleanup(t_renderer *renderer)
{
	int	tex_idx;

	tex_idx = 0;
	g_running = 0;
	pthread_join(renderer->counter_thread, NULL);
	while (tex_idx < TEX_COUNT)
	{
		if (renderer->textures[tex_idx]) {
			mlx_destroy_image(renderer->mlx, renderer->textures[tex_idx]);
			renderer->textures[tex_idx] = 0;
		}
		tex_idx++;
	}
	if (renderer->frame)
		mlx_destroy_image(renderer->mlx, renderer->frame);
	if (renderer->win)
		mlx_destroy_window(renderer->mlx, renderer->win);
	if (renderer->mlx)
	{
		mlx_destroy_display(renderer->mlx);
		free(renderer->mlx);
	}
	return (0);
}

int	on_destroy(t_renderer *renderer)
{
	cleanup(renderer);
	exit(0);
	return (0);
}

void	render_hitboxes(t_renderer *renderer);

int	on_key_press(int keysym, t_renderer *renderer)
{
	if (keysym == XK_Escape)
		return (on_destroy(renderer));

	if (keysym == XK_w || keysym == XK_Up || keysym == XK_space)
	{
		renderer->player.vy -= JUMP_FORCE;
	}
	if (keysym == XK_g)
	{
		g_debug_mode ^= 1;
		render_hitboxes(renderer);
	}
	if (keysym == XK_a || keysym == XK_Left)
		renderer->keys |= KEY_LEFT;
	if (keysym == XK_d || keysym == XK_Right)
		renderer->keys |= KEY_RIGHT;
	return (0);
}

int	on_key_release(int keysym, t_renderer *renderer)
{
	if (keysym == XK_a || keysym == XK_Left)
		renderer->keys &= ~KEY_LEFT;
	if (keysym == XK_d || keysym == XK_Right)
		renderer->keys &= ~KEY_RIGHT;
	return (0);
}

void	ft_fill_tile(t_renderer *renderer, int x, int y)
{
	int t;
	void *bg = mlx_get_data_addr(renderer->frame, &t, &t, &t);

	if (x < 0 || y < 0 || x >= MAP_SIZE || y >= MAP_SIZE)
		return ;

	for (int j = 0; j < TILE_SIZE; j++) {
		for (int i = 0; i < TILE_SIZE; i++) {
			int bg_idx = ((y * TILE_SIZE + j) * renderer->window.w) + (x * TILE_SIZE + i);
			((uint32_t *)bg)[bg_idx] = g_debug_mode ? DEBUG_COLOR : BG_COLOR;
		}
	}
}


int	ft_generate_map(t_renderer *renderer);

void	set_player_position(t_renderer *renderer, int i, int j)
{
	renderer->player.x = i * TILE_SIZE + .5 * (TILE_SIZE - renderer->player.w);
	renderer->player.y = j * TILE_SIZE + .5 * (TILE_SIZE - renderer->player.h);
}

void	render_hitboxes(t_renderer *renderer)
{
	if (!g_debug_mode) {
		ft_generate_map(renderer);
		return;
	}
	for (int j = 0; j < MAP_SIZE; j++) {
		for (int i = 0; i < MAP_SIZE; i++) {
			if (!is_wall(i, j)) continue ;
			int mask = compute_texture_mask(i, j);
			int index = get_texture_index(mask);
			render_hitbox(renderer, index, i, j);
		}
	}
}


void	player_hitbox(t_renderer *renderer)
{
	if (!g_debug_mode)
		return ;
	int x0 = renderer->player.x;
	int y0 = renderer->player.y;
	int x1 = renderer->player.x + renderer->player.w;
	int y1 = renderer->player.y + renderer->player.h;

	ft_line(renderer, (t_point){ x0, y0 }, (t_point){ x1, y0 });
	ft_line(renderer, (t_point){ x1, y0 }, (t_point){ x1, y1 });
	ft_line(renderer, (t_point){ x1, y1 }, (t_point){ x0, y1 });
	ft_line(renderer, (t_point){ x0, y1 }, (t_point){ x0, y0 });
	ft_line(renderer, (t_point){ x0, y0 }, (t_point){ x1, y1 });
	ft_line(renderer, (t_point){ x0, y1 }, (t_point){ x1, y0 });
}

void	ft_image_to_vbuffer(t_renderer *renderer, int texture_index, int x, int y);

void	ft_render_tile(t_renderer *renderer, int x, int y)
{
	if (!is_wall(x, y))
		return ft_fill_tile(renderer, x, y);
	int mask = compute_texture_mask(x, y);
	int index = get_texture_index(mask);
	ft_image_to_vbuffer(renderer, index, x * TILE_SIZE, y * TILE_SIZE);
	if (g_debug_mode)
		render_hitbox(renderer, index, x, y);
}

void	player_update_tiles(t_renderer *renderer)
{
	int	x0 = renderer->player.x / TILE_SIZE;
	int	y0 = (renderer->player.y) / TILE_SIZE;
	int	x1 = (renderer->player.x + renderer->player.w) / TILE_SIZE;
	int	y1 = (renderer->player.y + renderer->player.h) / TILE_SIZE;

	ft_render_tile(renderer, x0, y0);
	ft_render_tile(renderer, x0, y1);
	ft_render_tile(renderer, x1, y0);
	ft_render_tile(renderer, x1, y1);
}

int horizontal_overlap(t_player *player, t_hitbox box) {
	return (player->x < box.r) && (player->x + player->w > box.l);
}

int vertical_overlap(t_player *player, t_hitbox box) {
	return (player->y < box.b) && (player->y + player->h > box.t);
}

void move_down(t_player *p, t_hitbox box) {
	float overlap = p->y + p->h - box.t;
	float prev_overlap = p->prev_y + p->h - box.t;
	if (prev_overlap <= 0 && overlap > 0 && horizontal_overlap(p, box)) {
		p->vy = 0;
		p->y = box.t - p->h - 0.01f;
	}
}

void move_up(t_player *p, t_hitbox box) {
	float overlap = box.b - p->y;
	float prev_overlap = box.b - p->prev_y;
	if (prev_overlap <= 0 && overlap > 0 && horizontal_overlap(p, box)) {
		p->vy = 0;
		p->y = box.b + 0.01f;
	}
}

void move_right(t_player *p, t_hitbox box) {
	float overlap = p->x + p->w - box.l;
	float prev_overlap = p->prev_x + p->w - box.l;
	if (prev_overlap <= 0 && overlap > 0 && vertical_overlap(p, box)) {
		p->vx = 0;
		p->x = box.l - p->w - 0.01f;
	}
}
void move_left(t_player *p, t_hitbox box) {
	float overlap = box.r - p->x;
	float prev_overlap = box.r - p->prev_x;
	if (prev_overlap <= 0 && overlap > 0 && vertical_overlap(p, box)) {
		p->vx = 0;
		p->x = box.r + 0.01f;
	}
}

void ft_player_update(t_renderer *renderer) {
	t_player	*p;
	t_hitbox	box;

	player_update_tiles(renderer);
	player_hitbox(renderer);
	p = &renderer->player;
	p->vx *= FRICTION;
	p->vy += GRAVITY;
	if (renderer->keys & KEY_LEFT)
		p->vx = -VELOCITY;
	else if (renderer->keys & KEY_RIGHT)
		p->vx = VELOCITY;
	float	new_x = p->x + p->vx;
	float	new_y = p->y + p->vy;
	p->prev_x = p->x;
	p->x = new_x;
	int	x0 = p->x / TILE_SIZE;
	int	y0 = p->y / TILE_SIZE;
	int	x1 = (p->x + p->w) / TILE_SIZE;
	int	y1 = (p->y + p->h) / TILE_SIZE;
	if (p->vx > 0) {
		for (int y = y0; y <= y1; y++) {
			int	tx = (p->x + p->w) / TILE_SIZE;
			if (is_solid(tx, y, &box))
				move_right(p, box);
		}
	}
	else if (p->vx < 0) {
		for (int y = y0; y <= y1; y++) {
			int	tx = p->x / TILE_SIZE;
			if (is_solid(tx, y, &box))
				move_left(p, box);
		}
	}
	p->prev_y = p->y;
	p->y = new_y;
	x0 = p->x / TILE_SIZE;
	y0 = p->y / TILE_SIZE;
	x1 = (p->x + p->w) / TILE_SIZE;
	y1 = (p->y + p->h) / TILE_SIZE;
	if (p->vy > 0) {
		for (int x = x0; x <= x1; x++) {
			int	ty = (p->y + p->h) / TILE_SIZE;
			if (is_solid(x, ty, &box))
				move_down(p, box);
		}
	}
	else if (p->vy < 0) {
		for (int x = x0; x <= x1; x++) {
			int	ty = p->y / TILE_SIZE;
			if (is_solid(x, ty, &box))
				move_up(p, box);
		}
	}
}

void	ft_image_to_vbuffer(t_renderer *renderer, int texture_index, int x, int y)
{
	int	size_line = 0;
	int	bpp = 0;
	int	endian = 0;

	void	*bg = mlx_get_data_addr(renderer->frame, &bpp, &size_line, &endian);
	void	*fg = mlx_get_data_addr(renderer->textures[texture_index], &bpp, &size_line, &endian);

	for (int j = 0; j < TILE_SIZE; j++) {
		for (int i = 0; i < TILE_SIZE; i++) {
			int fg_idx = j * TILE_SIZE + i;
			int bg_idx = ((y + j) * renderer->window.w) + (x + i);

			if (x + i > renderer->window.w) continue;
			if (y + j > renderer->window.w) continue;
			if (x + i < 0) continue ;
			if (y + j < 0) continue ;

			uint32_t fg_pixel = ((uint32_t *)fg)[fg_idx];
			uint32_t bg_pixel = ((uint32_t *)bg)[bg_idx];

			if (texture_index != TEX_PLAYER)
				((uint32_t *)bg)[bg_idx] = blend_pixel(fg_pixel, g_debug_mode ? DEBUG_COLOR : BG_COLOR);
			else
				((uint32_t *)bg)[bg_idx] = blend_pixel(fg_pixel, bg_pixel);
		}
	}
}

int	render(t_renderer *renderer)
{
	if (!g_should_render)
		return (0);
	g_frame_count++;
	g_should_render = 0;
	ft_player_update(renderer);
	ft_image_to_vbuffer(renderer, TEX_PLAYER,
		renderer->player.x - .5 * (TILE_SIZE - renderer->player.w),
		renderer->player.y - .5 * (TILE_SIZE - renderer->player.h));
	mlx_put_image_to_window(renderer->mlx, renderer->win, renderer->frame, 0, 0);
	return (0);
}

int	ft_register_hooks(t_renderer *renderer)
{
	mlx_hook(renderer->win, KeyPress, KeyPressMask,	on_key_press, renderer);
	mlx_hook(renderer->win, KeyRelease, KeyReleaseMask,	on_key_release,
		renderer);
	mlx_hook(renderer->win, DestroyNotify, 0, on_destroy, renderer);
	mlx_loop_hook(renderer->mlx, render, renderer);
	return (0);
}

int	ft_init_player(t_renderer *renderer)
{
	ft_memset(renderer->textures, 0, sizeof(void *) * TEX_COUNT);
	renderer->player.w = 32;
	renderer->player.h = 56;
	renderer->window.x = 0;
	renderer->window.y = 0;
	renderer->window.w = WINDOW_SIZE;
	renderer->window.h = WINDOW_SIZE;
	return (0);
}

int	ft_generate_map(t_renderer *renderer)
{
	for (int j = 0; j < MAP_SIZE; j++) {
		for (int i = 0; i < MAP_SIZE; i++) {
			if (map[j][i] == 'P')
				set_player_position(renderer, i, j);
			if (map[j][i] == 'E')
				ft_image_to_vbuffer(renderer, TEX_PORTAL, i * TILE_SIZE, j * TILE_SIZE);
			if (!is_wall(i, j)) continue ;
			int mask = compute_texture_mask(i, j);
			int idx = get_texture_index(mask);
			ft_image_to_vbuffer(renderer, idx,
				i * TILE_SIZE, j * TILE_SIZE);
		}
	}
	return (0);
}

int	ft_generate_background(t_renderer *renderer)
{
	for(int j = 0; j < MAP_SIZE; j++) {
		for (int i = 0; i < MAP_SIZE; i++)
			ft_fill_tile(renderer, i, j);
	}
	return (0);
}

int	ft_init_renderer(t_renderer *renderer)
{
	renderer->keys = 0;
	renderer->mlx = mlx_init();
	if (!renderer->mlx)
		return (1);
	if (ft_load_textures(renderer))
		return (1);
	ft_init_player(renderer);
	renderer->win = mlx_new_window(renderer->mlx, WINDOW_SIZE, WINDOW_SIZE,
		"So Long");
	if (!renderer->win)
	{
		ft_printf("\e[91m[ERR]\e[m Failed to create a window\n");
		free(renderer->mlx);
		return (1);
	}
	renderer->frame = mlx_new_image(renderer->mlx, WINDOW_SIZE, WINDOW_SIZE);
	if (ft_generate_background(renderer))
	{
		ft_printf("\e[91m[ERR]\e[m Failed to render background\n");
		return (1);
	}
	if (ft_generate_map(renderer))
	{
		ft_printf("\x1b[91m[ERR]\x1b[m Failed to generate map\n");
		return (1);
	}
	ft_register_hooks(renderer);
	return (0);
}

int	main(void)
{
	t_renderer	renderer;

	pthread_create(&renderer.counter_thread, NULL, counter_thread, NULL);
	if (ft_init_renderer(&renderer))
	{
		cleanup(&renderer);
		return (1);
	}
	mlx_loop(renderer.mlx);
	on_destroy(&renderer);
	return (0);
}
