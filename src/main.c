/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smamalig <smamalig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/17 10:34:52 by smamalig          #+#    #+#             */
/*   Updated: 2025/03/24 23:01:25 by smamalig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "mlx_int.h"
#include "so_long.h"
#include "mlx.h"

#include <X11/keysym.h>
#include <X11/X.h>
#include <stdatomic.h>
#include <stdint.h>
#include <time.h>

int	g_debug_mode = 0;

static char map[MAP_HEIGHT][MAP_WIDTH] = {
	"11111111111111111111111111111111",
	"1P000000000000000000000000000001",
	"11111101111111111111111111111101",
	"10000000000000000000000000000001",
	"10111111011111111111111111111111",
	"10000000000000000000000000000001",
	"11111111111011111111111111111101",
	"10000000000000000000000000000001",
	"10111111111111011111111111111111",
	"10000000000000000000000000000001",
	"11111111111111111011111111111101",
	"10000000000000000000000000000001",
	"10111111111111111110111111111111",
	"10000000000000000000000000000001",
	"11111111111111111111101111111101",
	"10000000000000000000000000000001",
	"10111111111111111111111011111111",
	"10000000000000000000000000000001",
	"11111111111111111111111110111101",
	"10000000000000000000000000000001",
	"10111111111111111111111111101111",
	"10000000000000000000000000000001",
	"11111111111111111111111110111101",
	"10000000000000000000000000000001",
	"10111111111111111111111011111111",
	"10000000000000000000000000000001",
	"11111111111111111111101111111101",
	"10000000000000000000000000000001",
	"10111111111111111110111111111111",
	"10000000000000000000000000000001",
	"10000000000000000000000000000001",
	"11111111111111111111111111111111",
};

static char texture_index_lookup[0x100] = {
[0x00] = TEX_PLATFORM,
[0x01] = TEX_PLATFORM,
[0x02] = TEX_PLATFORM,
[0x03] = TEX_PLATFORM,
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
[0x63] = TEX_CORNER_EXT_TR,
[0x64] = TEX_LJUNC_BL,
[0x65] = TEX_LJUNC_BL,
[0x66] = TEX_CORNER_EXT_TR,
[0x67] = TEX_CORNER_EXT_TR,
[0x68] = TEX_LJUNC_BL,
[0x69] = TEX_LJUNC_BL,
[0x6a] = TEX_CORNER_EXT_TR,
[0x6b] = TEX_CORNER_EXT_TR,
[0x6c] = TEX_LJUNC_BL,
[0x6d] = TEX_LJUNC_BL,
[0x6e] = TEX_CORNER_EXT_TR,
[0x6f] = TEX_CORNER_EXT_TR,


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

	alpha = 1 - (fg >> 24) / 255.0f;
	if (alpha == 1) return fg;
	if (alpha == 0) return bg;
	r = ((fg >> 16) & 0xff) * alpha + ((bg >> 16) & 0xff) * (1 - alpha);
	g = ((fg >> 8) & 0xff) * alpha + ((bg >> 8) & 0xff) * (1 - alpha);
	b = (fg & 0xff) * alpha + (bg & 0xff) * (1 - alpha);
	return ((r << 16) | (g << 8) | b);
}

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

bool is_wall(int tx, int ty)
{
	return (tx < 0 || tx >= MAP_WIDTH || ty < 0
		|| ty >= MAP_HEIGHT || map[ty][tx] == '1');
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


int	cleanup(t_renderer *r)
{
	int	tex_idx;

	tex_idx = 0;
	r->is_running = 0;
	if (r->counter_thread)
		pthread_join(r->counter_thread, NULL);
	if (r->render_thread)
		pthread_join(r->render_thread, NULL);
	while (tex_idx < TEX_COUNT)
	{
		if (r->textures[tex_idx]) {
			mlx_destroy_image(r->mlx, r->textures[tex_idx]);
			r->textures[tex_idx] = 0;
		}
		tex_idx++;
	}
	if (r->frame)
		mlx_destroy_image(r->mlx, r->frame);
	if (r->win)
		mlx_destroy_window(r->mlx, r->win);
	if (r->mlx)
	{
		mlx_destroy_display(r->mlx);
		free(r->mlx);
	}
	return (0);
}

int	on_destroy(t_renderer *r)
{
	cleanup(r);
	exit(0);
	return (0);
}

int	on_key_press(int keysym, t_renderer *r)
{
	if (keysym == XK_Escape)
		return (on_destroy(r));

	if (keysym == XK_w || keysym == XK_Up || keysym == XK_space)
	{
		r->player.vy -= JUMP_FORCE;
	}
	if (keysym == XK_g)
	{
		g_debug_mode ^= 1;
		render_hitboxes(r);
	}
	if (keysym == XK_Shift_L || keysym == XK_Shift_R)
		r->should_dash = DASH_FRAMES;
	if (keysym == XK_a || keysym == XK_Left)
		r->keys |= KEY_LEFT;
	if (keysym == XK_d || keysym == XK_Right)
		r->keys |= KEY_RIGHT;
	return (0);
}

int	on_key_release(int keysym, t_renderer *r)
{
	if (keysym == XK_a || keysym == XK_Left)
		r->keys &= ~KEY_LEFT;
	if (keysym == XK_d || keysym == XK_Right)
		r->keys &= ~KEY_RIGHT;
	return (0);
}

t_vector translate(t_renderer *r, float x, float y)
{
	return (t_vector){
		.x = x - r->window.x,
		.y = y - r->window.y
	};
}

int	ft_fill_tile(t_renderer *r, int tx, int ty)
{
	int _;
	void *bg = mlx_get_data_addr(r->frame, &_, &_, &_);
	if (!bg)
		return (1);

	if (tx < 0 || ty < 0 || tx >= MAP_WIDTH || ty >= MAP_HEIGHT)
		return (0);

	t_vector t = translate(r, tx * TILE_SIZE, ty * TILE_SIZE);
	for (int j = 0; j < TILE_SIZE; j++) {
		if (t.y + j >= r->window.h || t.y + j < 0)
			continue;
		for (int i = 0; i < TILE_SIZE; i++) {
			if (t.x + i > r->window.w || t.x + i < 0)
				continue;
			int bg_idx = ((t.y + j) * r->window.w) + (t.x + i);
			((uint32_t *)bg)[bg_idx] = BG_COLOR;
		}
	}
	return (0);
}


int	ft_generate_map(t_renderer *r);

void	set_player_position(t_renderer *r, int tx, int ty)
{
	r->player.x = tx * TILE_SIZE + .5 * (TILE_SIZE - r->player.w);
	r->player.y = ty * TILE_SIZE + .5 * (TILE_SIZE - r->player.h);
	r->player.px = r->player.x;
	r->player.py = r->player.y;
}

void	render_hitboxes(t_renderer *r)
{
	if (!g_debug_mode) {
		ft_generate_map(r);
		return;
	}
	for (int j = 0; j < MAP_HEIGHT; j++) {
		for (int i = 0; i < MAP_WIDTH; i++) {
			if (!is_wall(i, j)) continue ;
			int mask = compute_texture_mask(i, j);
			int tex_idx = get_texture_index(mask);
			render_hitbox(r, tex_idx, i, j);
		}
	}
}

void	player_hitbox(t_renderer *r)
{
	if (!g_debug_mode)
		return ;

	t_vector t0 = translate(r, r->player.x, r->player.y);
	t_vector t1 = translate(r, r->player.x + r->player.w, r->player.y + r->player.h);

	ft_line(r, (t_point){ t0.x, t0.y }, (t_point){ t1.x, t0.y });
	ft_line(r, (t_point){ t1.x, t0.y }, (t_point){ t1.x, t1.y });
	ft_line(r, (t_point){ t1.x, t1.y }, (t_point){ t0.x, t1.y });
	ft_line(r, (t_point){ t0.x, t1.y }, (t_point){ t0.x, t0.y });
	ft_line(r, (t_point){ t0.x, t0.y }, (t_point){ t1.x, t1.y });
	ft_line(r, (t_point){ t0.x, t1.y }, (t_point){ t1.x, t0.y });
}

void	ft_image_to_vbuffer(t_renderer *r, void *img, t_rect pos);

void	ft_render_tile(t_renderer *r, int tx, int ty)
{
	if (!is_wall(tx, ty)) {
		ft_fill_tile(r, tx, ty);
		return ;
	}
	int mask = compute_texture_mask(tx, ty);
	int tex_idx = get_texture_index(mask);
	t_vector t = translate(r, tx * TILE_SIZE, ty * TILE_SIZE);
	ft_image_to_vbuffer(r, r->textures[tex_idx], (t_rect){
		t.x, t.y, TILE_SIZE, TILE_SIZE
	});
	if (g_debug_mode)
		render_hitbox(r, tex_idx, tx, ty);
}

void	player_update_tiles(t_renderer *r)
{
	int x0_ = r->player.px / TILE_SIZE;
	int y0_ = r->player.py / TILE_SIZE;
	int x1_ = (r->player.px + r->player.w) / TILE_SIZE;
	int y1_ = (r->player.py + r->player.h) / TILE_SIZE;

	int	x0 = r->player.x / TILE_SIZE;
	int	y0 = r->player.y / TILE_SIZE;
	int	x1 = (r->player.x + r->player.w) / TILE_SIZE;
	int	y1 = (r->player.y + r->player.h) / TILE_SIZE;

	ft_render_tile(r, x0_, y0_);
	ft_render_tile(r, x0_, y1_);
	ft_render_tile(r, x1_, y0_);
	ft_render_tile(r, x1_, y1_);

	ft_render_tile(r, x0, y0);
	ft_render_tile(r, x0, y1);
	ft_render_tile(r, x1, y0);
	ft_render_tile(r, x1, y1);
}

int horizontal_overlap(t_player *player, t_hitbox box) {
	return (player->x < box.r) && (player->x + player->w > box.l);
}

int vertical_overlap(t_player *player, t_hitbox box) {
	return (player->y < box.b) && (player->y + player->h > box.t);
}

void move_down(t_player *p, t_hitbox box) {
	float overlap = p->y + p->h - box.t;
	float prev_overlap = p->py + p->h - box.t;
	if (prev_overlap <= 0 && overlap > 0 && horizontal_overlap(p, box)) {
		p->vy = 0;
		p->y = box.t - p->h - COLLISION_OFFSET;
	}
}

void move_up(t_player *p, t_hitbox box) {
	float overlap = box.b - p->y;
	float prev_overlap = box.b - p->py;
	if (prev_overlap <= 0 && overlap > 0 && horizontal_overlap(p, box)) {
		p->vy = 0;
		p->y = box.b + COLLISION_OFFSET;
	}
}

void move_right(t_player *p, t_hitbox box) {
	float overlap = p->x + p->w - box.l;
	float prev_overlap = p->px + p->w - box.l;
	if (prev_overlap <= 0 && overlap > 0 && vertical_overlap(p, box)) {
		p->vx = 0;
		p->x = box.l - p->w - COLLISION_OFFSET;
	}
}
void move_left(t_player *p, t_hitbox box) {
	float overlap = box.r - p->x;
	float prev_overlap = box.r - p->px;
	if (prev_overlap <= 0 && overlap > 0 && vertical_overlap(p, box)) {
		p->vx = 0;
		p->x = box.r + COLLISION_OFFSET;
	}
}

void	ft_collision_x(t_player *p)
{
	t_hitbox	box;
	int			y0 = p->y / TILE_SIZE;
	int			y1 = (p->y + p->h) / TILE_SIZE;

	if (p->vx > 0) {
		for (int y = y0; y <= y1; y++) {
			int	tx = (p->x + p->w) / TILE_SIZE;
			if (is_solid(tx, y, &box) == 1)
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
}

void	ft_collision_y(t_player *p)
{
	t_hitbox	box;
	int x0 = p->x / TILE_SIZE;
	int x1 = (p->x + p->w) / TILE_SIZE;
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

void	ft_player_update(t_renderer *r)
{
	t_player	*p;

	player_update_tiles(r);
	player_hitbox(r);
	p = &r->player;
	p->vx *= FRICTION;
	p->vy += GRAVITY;
	if (r->keys & KEY_LEFT)
		p->vx = -VELOCITY;
	else if (r->keys & KEY_RIGHT)
		p->vx = VELOCITY;
	if (r->should_dash) {
		if (r->keys & KEY_LEFT)
			p->vx = -VELOCITY * DASH_MULTIPLIER;
		else if (r->keys & KEY_RIGHT)
			p->vx = VELOCITY * DASH_MULTIPLIER;
		r->should_dash--;
	}
	p->px = p->x;
	p->x += p->vx;
	ft_collision_x(p);
	p->py = p->y;
	p->y += p->vy;
	ft_collision_y(p);
}

void	ft_image_to_vbuffer(t_renderer *r, void *img, t_rect p)
{
	int	_;
	void	*bg = mlx_get_data_addr(r->frame, &_, &_, &_);
	void	*fg = mlx_get_data_addr(img, &_, &_, &_);

	int x0 = (p.x < 0) ? -p.x : 0;
	int x1 = (p.x + p.w > r->window.w) ? r->window.w - p.x : p.w;
	int y0 = (p.y < 0) ? -p.y : 0;
	int y1 = (p.y + p.h > r->window.h) ? r->window.h - p.y : p.h;

	uint32_t *fg_ptr = (uint32_t *)fg + (y0 * p.w) + x0;
	uint32_t *bg_ptr = (uint32_t *)bg + ((p.y + y0) * r->window.w) + (p.x + x0);

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
		bg_ptr += r->window.w;
	}
}

void	ft_camera_update(t_renderer *r)
{
	int x_margin = 0;
	int y_margin = 0;
	if (r->player.x - x_margin > r->window.x + r->window.w / 2.
		&& r->window.x + r->window.w < r->map.w)
		r->window.x = r->player.x - r->window.w / 2. - x_margin;
	else if (r->player.x + x_margin < r->window.x + r->window.w / 2.
		&& r->window.x > 0)
		r->window.x = r->player.x - r->window.w / 2. + x_margin;
	if (r->player.y - y_margin > r->window.y + r->window.h / 2.
		&& r->window.y + r->window.h < r->map.h)
		r->window.y = r->player.y - r->window.h / 2. - y_margin;
	else if (r->player.y + y_margin < r->window.y + r->window.h / 2.
		&& r->window.y > 0)
		r->window.y = r->player.y - r->window.h / 2. + y_margin;
}

int	render(t_renderer *r)
{
	if (!r->should_render)
		return (0);
	r->frame_count++;
	r->should_render = 0;
	ft_player_update(r);
	ft_camera_update(r);
	ft_generate_background(r);
	ft_generate_map(r);
	t_rect p = {
		-r->window.x * PARALLAX_CONSTANT,
		-r->window.y * PARALLAX_CONSTANT,
		MAP_WIDTH * TILE_SIZE * PARALLAX_CONSTANT,
		MAP_HEIGHT * TILE_SIZE * PARALLAX_CONSTANT
	};
	ft_image_to_vbuffer(r, r->parallaxes[0], p);
	render_hitboxes(r);
	t_vector t = translate(r,
		r->player.x - .5 * (TILE_SIZE - r->player.w),
		r->player.y - .5 * (TILE_SIZE - r->player.h));
	ft_image_to_vbuffer(r, r->textures[TEX_PLAYER], (t_rect){
		t.x, t.y, TILE_SIZE, TILE_SIZE
	});
	mlx_put_image_to_window(r->mlx, r->win, r->frame, 0, 0);
	return (0);
}

int	ft_register_hooks(t_renderer *r)
{
	mlx_hook(r->win, KeyPress, KeyPressMask, on_key_press, r);
	mlx_hook(r->win, KeyRelease, KeyReleaseMask, on_key_release, r);
	mlx_hook(r->win, DestroyNotify, 0, on_destroy, r);
	mlx_loop_hook(r->mlx, render, r);
	return (0);
}

int	ft_init_player(t_renderer *r)
{
	r->player.w = 32;
	r->player.h = 56;
	r->window.w = WINDOW_W;
	r->window.h = WINDOW_H;
	r->map.w = MAP_WIDTH * TILE_SIZE;
	r->map.h = MAP_HEIGHT * TILE_SIZE;
	if (r->map.w < r->window.w)
		r->window.x = (r->map.w - r->window.w) / 2;
	if (r->map.h < r->window.h)
		r->window.y = (r->map.h - r->window.h) / 2;
	return (0);
}

int	ft_generate_map(t_renderer *r)
{
	for (int j = 0; j < MAP_HEIGHT; j++) {
		for (int i = 0; i < MAP_WIDTH; i++) {
			t_vector t = translate(r, i * TILE_SIZE, j * TILE_SIZE);
			if (map[j][i] == 'P' && r->player.x == 0 && r->player.y == 0)
				set_player_position(r, i, j);
			if (map[j][i] == 'E')
				ft_image_to_vbuffer(r, r->textures[TEX_EXIT], (t_rect){
					t.x, t.y, TILE_SIZE, TILE_SIZE
				});
			if (!is_wall(i, j)) continue ;
			int mask = compute_texture_mask(i, j);
			int tex_idx = get_texture_index(mask);
			ft_image_to_vbuffer(r, r->textures[tex_idx], (t_rect){
				t.x, t.y, TILE_SIZE, TILE_SIZE
			});
		}
	}
	return (0);
}

int	ft_generate_background(t_renderer *r)
{
	for(int j = 0; j < MAP_HEIGHT; j++)
		for (int i = 0; i < MAP_WIDTH; i++)
			if (ft_fill_tile(r, i, j))
				return (1);
	return (0);
}

void	ft_debug(char *info)
{
	ft_printf("\e[95m[DBG]\e[m %s\n", info);
}

int	ft_init_parallax(t_renderer *r)
{
	float parallax = 1;
	int _;
	for (int k = 0; k < PARALLAX_LAYERS; k++)
	{
		parallax *= PARALLAX_CONSTANT;
		int width = MAP_WIDTH * TILE_SIZE * parallax;
		int height = MAP_HEIGHT * TILE_SIZE * parallax;
		printf("Trying to init %ix%i parallax\n", width, height);
		r->parallaxes[k] = mlx_new_image(r->mlx, width, height);
		if (!r->parallaxes[k])
			return (1);
		uint32_t *buf = (uint32_t *)mlx_get_data_addr(r->parallaxes[k], &_, &_, &_);
		for (int j = 0; j < height; j++) {
			for (int i = 0; i < width; i++) {
				uint8_t r = 255. * i / width;
				uint8_t g = 255. * j / width;
				uint8_t b = 128.;
				uint32_t color = (r << 16) | (g << 8) | b;
				buf[j * width + i] = color;
			}
		}
	}
	return (0);
}

void	launch_threads(t_renderer *r)
{
	pthread_create(&r->counter_thread, NULL, (t_thread)counter_thread, r);
	pthread_create(&r->render_thread, NULL, (t_thread)render_thread, r);
	ft_debug("Launched threads");
}

int	ft_init_renderer(t_renderer *r)
{
	r->keys = 0;
	r->mlx = mlx_init();
	if (!r->mlx)
		return (1);
	ft_debug("Initialized MLX");
	if (ft_load_textures(r))
		return (1);
	ft_debug("Loaded textures");
	ft_init_player(r);
	ft_debug("Initialized player");
	r->win = mlx_new_window(r->mlx, WINDOW_W, WINDOW_H,
		"So Long");
	if (!r->win)
	{
		ft_printf("\e[91m[ERR]\e[m Failed to create a window\n");
		free(r->mlx);
		return (1);
	}
	ft_debug("Created window");
	r->frame = mlx_new_image(r->mlx, WINDOW_W, WINDOW_H);
	if (!r->frame)
	{
		ft_printf("\e[91m[ERR]\e[m Failed to generate frame\n");
	}
	ft_debug("Prepared frame");
	if (ft_generate_background(r))
	{
		ft_printf("\e[91m[ERR]\e[m Failed to render background\n");
		return (1);
	}
	ft_debug("Generated background");
	if (ft_init_parallax(r))
	{
		ft_printf("\e[91m[ERR]\e[m Failed to initialize parallax: %m\n");
		return (1);
	}
	ft_debug("Initialized parallax");
	if (ft_generate_map(r))
	{
		ft_printf("\x1b[91m[ERR]\x1b[m Failed to generate map\n");
		return (1);
	}
	launch_threads(r);
	ft_debug("Generated map");
	ft_register_hooks(r);
	ft_debug("Registered hooks");
	return (0);
}

void generate_map(unsigned int seed)
{
    ft_printf("\e[95m[DBG]\e[m Seed %i\n", seed);
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            map[y][x] = '1';
        }
    }

    int player_x = MAP_WIDTH / 2;
    int player_y = MAP_HEIGHT / 2;

    map[player_y][player_x] = 'P';

    for (int i = 0; i < MAP_WIDTH * MAP_HEIGHT; i++) {
        int dir = ft_rand(seed) % 4;
        switch (dir) {
            case 0: if (player_x > 1) player_x--; break;
            case 1: if (player_x < MAP_WIDTH - 2) player_x++; break;
            case 2: if (player_y > 1) player_y--; break;
            case 3: if (player_y < MAP_HEIGHT - 2) player_y++; break;
        }
        if (map[player_y][player_x] == '1')
            map[player_y][player_x] = '0';
    }

    for (int y = 0; y < MAP_HEIGHT - 1; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            if (map[y][x] == '0' && map[y + 1][x] == '1') {
                if (ft_rand(seed) % 5 == 0) {
                    map[y][x] = 'C';
                }
            }
        }
    }
}

void print_map() {
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
			printf("%c", map[y][x]);
        }
        printf("\n");
    }
}

int	main(void)
{
	t_renderer	r;

	ft_memset(&r, 0, sizeof(t_renderer));
	ft_printf("\e[94m[INF]\e[m Starting game\n");
	r.is_running = 1;
	generate_map(ft_time(NULL));
	print_map();
	if (ft_init_renderer(&r))
	{
		cleanup(&r);
		return (1);
	}
	mlx_loop(r.mlx);
	on_destroy(&r);
	return (0);
}
