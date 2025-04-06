/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hitbox.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smamalig <smamalig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 22:20:12 by smamalig          #+#    #+#             */
/*   Updated: 2025/04/06 19:57:19 by smamalig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"
#include "mlx.h"

t_hitbox g_hitboxes[TEX_COUNT] = {
[TEX_EMPTY] = { 0, 0, 64, 64 },
[TEX_WALL_TOP] = { 0, 0, 64, 48 },
[TEX_WALL_BOTTOM] = { 0, 16, 64, 64 },
[TEX_WALL_LEFT] = { 0, 0, 48, 64 },
[TEX_WALL_RIGHT] = { 16, 0, 64, 64 },
[TEX_CORNER_TL] = { 0, 16, 64, 64 },
[TEX_CORNER_TR] = { 0, 16, 64, 64 },
[TEX_CORNER_BL] = { 0, 0, 64, 48 },
[TEX_CORNER_BR] = { 0, 0, 64, 48 },
[TEX_CORNER_EXT_TL] = { 16, 16, 64, 64 },
[TEX_CORNER_EXT_TR] = { 0, 16, 48, 64 },
[TEX_CORNER_EXT_BL] = { 16, 0, 64, 48 },
[TEX_CORNER_EXT_BR] = { 0, 0, 48, 48 },
[TEX_CORNER_TJUNC_T] = { 0, 16, 64, 64 },
[TEX_CORNER_TJUNC_B] = { 0, 0, 64, 48 },
[TEX_CORNER_TJUNC_L] = { 16, 0, 64, 64 },
[TEX_CORNER_TJUNC_R] = { 0, 0, 48, 64 },
[TEX_JUNC_CROSS] = { 16, 16, 48, 48 },
[TEX_TJUNC_T] = { 0, 16, 64, 48 },
[TEX_TJUNC_B] = { 0, 16, 64, 48 },
[TEX_TJUNC_L] = { 16, 0, 48, 64 },
[TEX_TJUNC_R] = { 16, 0, 48, 64 },
[TEX_LJUNC_TL] = { 0, 16, 48, 48 },
[TEX_LJUNC_TR] = { 16, 16, 64, 48 },
[TEX_LJUNC_BL] = { 0, 16, 48, 48 },
[TEX_LJUNC_BR] = { 16, 16, 64, 48 },
[TEX_PLATFORM] = { 16, 16, 48, 48 },
[TEX_PLATFORM_T] = { 16, 0, 48, 40 },
[TEX_PLATFORM_B] = { 16, 16, 48, 64 },
[TEX_PLATFORM_L] = { 0, 16, 16, 48 },
[TEX_PLATFORM_R] = { 48, 16, 64, 48 },
[TEX_PLATFORM_H] = { 0, 16, 64, 48 },
[TEX_PLATFORM_V] = { 16, 0, 48, 64 },
[TEX_HLJUNC_TL] = { 0, 16, 64, 48 },
[TEX_HLJUNC_TR] = { 0, 16, 64, 48 },
[TEX_HLJUNC_BL] = { 0, 16, 64, 48 },
[TEX_HLJUNC_BR] = { 0, 16, 64, 48 },
[TEX_VLJUNC_TL] = { 0, 16, 48, 64 },
[TEX_VLJUNC_TR] = { 16, 0, 48, 64 },
[TEX_VLJUNC_BL] = { 0, 0, 48, 48 },
[TEX_VLJUNC_BR] = { 16, 0, 64, 48 },
[TEX_CORNERS_XTL] = { 0, 16, 64, 48 },
[TEX_CORNERS_XTR] = { 0, 16, 64, 48 },
[TEX_CORNERS_XBL] = { 0, 16, 48, 64 },
[TEX_CORNERS_XBR] = { 16, 16, 64, 64 },
[TEX_DIAG_TLBR] = { 0, 16, 64, 48 },
[TEX_DIAG_TRBL] = { 0, 16, 64, 48 },
[TEX_PLAYER] = { 0, 0, 64, 64 },
[TEX_COLLECTIBLE] = { 20, 36, 44, 58 },
[TEX_EXIT] = { 0, 0, 64, 64},
};

void ft_line(t_game *g, t_point p0, t_point p1)
{
	int dx = ft_abs(p1.x - p0.x);
	int dy = ft_abs(p1.y - p0.y);
	int sx = (p0.x < p1.x) ? 1 : -1;
	int sy = (p0.y < p1.y) ? 1 : -1;
	int err = dx - dy;

	int t;
	void *frame = mlx_get_data_addr(g->frame, &t, &t, &t);

	while (p0.x != p1.x || p0.y != p1.y)
	{
		if (p0.x >= 0 && p0.x < g->window.w && p0.y >= 0
			&& p0.y < g->window.h) {
			((uint32_t *)frame)[p0.y * g->window.w + p0.x] = DEBUG_COLOR;
		}
		int e2 = 2 * err;
		if (e2 > -dy) {
			err -= dy;
			p0.x += sx;
		}
		if (e2 < dx)
		{
			err += dx;
			p0.y += sy;
		}
	}
}

bool is_solid(t_game *g, int tile_x, int tile_y, t_hitbox *hitbox)
{
	int			tex_idx;
	t_hitbox	box;

	if (!is_wall(g, tile_x, tile_y))
		return (0);
	tex_idx = get_texture_index(compute_texture_mask(g, tile_x, tile_y));
	box = g_hitboxes[tex_idx];
	hitbox->l = tile_x * TILE_SIZE + box.l;
	hitbox->t = tile_y * TILE_SIZE + box.t;
	hitbox->r = tile_x * TILE_SIZE + box.r;
	hitbox->b = tile_y * TILE_SIZE + box.b;
	return (1);
}

t_hitbox ft_absolute_hitbox(t_game *g, int tx, int ty)
{
	int			tex_idx;
	t_hitbox	box;

	tex_idx = get_texture_index(compute_texture_mask(g, tx, ty));
	box = g_hitboxes[tex_idx];
	return (t_hitbox){
		.l = tx * TILE_SIZE + box.l,
		.t = ty * TILE_SIZE + box.t,
		.r = tx * TILE_SIZE + box.r,
		.b = ty * TILE_SIZE + box.b
	};
}

void	render_hitbox(t_game *g, int index, int tx, int ty)
{
	t_hitbox hitbox = g_hitboxes[index];

	t_vector t0 = translate(g, tx * TILE_SIZE + hitbox.l,
		ty * TILE_SIZE + hitbox.t);
	t_vector t1 = translate(g, tx * TILE_SIZE + hitbox.r,
		ty * TILE_SIZE + hitbox.b);

	ft_line(g, (t_point){ t0.x, t0.y }, (t_point){ t1.x, t0.y });
	ft_line(g, (t_point){ t1.x, t0.y }, (t_point){ t1.x, t1.y });
	ft_line(g, (t_point){ t1.x, t1.y }, (t_point){ t0.x, t1.y });
	ft_line(g, (t_point){ t0.x, t1.y }, (t_point){ t0.x, t0.y });
	ft_line(g, (t_point){ t0.x, t0.y }, (t_point){ t1.x, t1.y });
	ft_line(g, (t_point){ t0.x, t1.y }, (t_point){ t1.x, t0.y });
}
