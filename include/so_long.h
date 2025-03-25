/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smamalig <smamalig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 14:14:42 by smamalig          #+#    #+#             */
/*   Updated: 2025/03/24 21:04:08 by smamalig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SO_LONG_H
# define SO_LONG_H

# include "libft.h"

# include <pthread.h>
# include <stdatomic.h>
# include <stdbool.h>
#include <stdint.h>

# define KEY_LEFT 1
# define KEY_RIGHT 2

# define BG_COLOR 0x8080ff
# define DEBUG_COLOR 0xff0000

# define VELOCITY 3
# define GRAVITY 0.2
# define FRICTION 0.9
# define JUMP_FORCE 5.8
# define DASH_MULTIPLIER 3.5
# define DASH_FRAMES 10
# define COLLISION_OFFSET 0

# define FRAMES_PER_SECOND 60
# define PARALLAX_LAYERS 1
# define PARALLAX_CONSTANT 0.8
# define MAP_WIDTH 32
# define MAP_HEIGHT 32
# define TILE_SIZE 64
# define WINDOW_W 1200
# define WINDOW_H 800

typedef struct s_vector
{
	float	x;
	float	y;
}	t_vector;

typedef struct s_point
{
	int	x;
	int	y;
}	t_point;

typedef struct s_rect
{
	int	x;
	int	y;
	int	w;
	int	h;
}	t_rect;

typedef struct s_hitbox
{
	int	l;
	int	t;
	int	r;
	int	b;
}	t_hitbox;

typedef struct s_player
{
	float	px;
	float	py;
	float	x;
	float	y;
	float	vx;
	float	vy;
	int		w;
	int		h;
}	t_player;

enum
{
	TEX_EMPTY,
	TEX_WALL_TOP,
	TEX_WALL_BOTTOM,
	TEX_WALL_LEFT,
	TEX_WALL_RIGHT,
	TEX_CORNER_TL,
	TEX_CORNER_TR,
	TEX_CORNER_BL,
	TEX_CORNER_BR,
	TEX_CORNER_EXT_TL,
	TEX_CORNER_EXT_TR,
	TEX_CORNER_EXT_BL,
	TEX_CORNER_EXT_BR,
	TEX_CORNER_TJUNC_T,
	TEX_CORNER_TJUNC_B,
	TEX_CORNER_TJUNC_L,
	TEX_CORNER_TJUNC_R,
	TEX_JUNC_CROSS,
	TEX_TJUNC_T,
	TEX_TJUNC_B,
	TEX_TJUNC_L,
	TEX_TJUNC_R,
	TEX_LJUNC_TL,
	TEX_LJUNC_TR,
	TEX_LJUNC_BL,
	TEX_LJUNC_BR,
	TEX_PLATFORM,
	TEX_PLATFORM_T,
	TEX_PLATFORM_B,
	TEX_PLATFORM_L,
	TEX_PLATFORM_R,
	TEX_PLATFORM_H,
	TEX_PLATFORM_V,
	TEX_HLJUNC_TL,
	TEX_HLJUNC_TR,
	TEX_HLJUNC_BL,
	TEX_HLJUNC_BR,
	TEX_VLJUNC_TL,
	TEX_VLJUNC_TR,
	TEX_VLJUNC_BL,
	TEX_VLJUNC_BR,
	TEX_CORNERS_XTL,
	TEX_CORNERS_XTR,
	TEX_CORNERS_XBL,
	TEX_CORNERS_XBR,
	TEX_DIAG_TLBR,
	TEX_DIAG_TRBL,
	TEX_PLAYER,
	TEX_EXIT,
	TEX_LOADING_0,
	TEX_LOADING_1,
	TEX_LOADING_2,
	TEX_LOADING_3,
	TEX_LOADING_4,
	TEX_LOADING_5,
	TEX_LOADING_6,
	TEX_LOADING_7,
	TEX_LOADING_8,
	TEX_LOADING_9,
	TEX_LOADING_10,
	TEX_LOADING_11,
	TEX_LOADING_12,
	TEX_LOADING_13,
	TEX_COUNT
};

typedef struct s_renderer
{
	void		*mlx;
	void		*win;
	void		*frame;
	void		*parallaxes[PARALLAX_LAYERS];
	void		*textures[TEX_COUNT];
	pthread_t	counter_thread;
	pthread_t	render_thread;
	t_player	player;
	t_rect		window;
	t_rect		map;
	int			should_dash;
	int			keys;
	atomic_int	is_running;
	atomic_int	frame_count;
	atomic_int	should_render;
}	t_renderer;

int			ft_generate_map(t_renderer *r);
int			ft_generate_background(t_renderer *r);
int			ft_load_textures(t_renderer *r);
t_hitbox	ft_absolute_hitbox(int x, int y);
int			get_texture_index(int mask);
int			compute_texture_mask(int x, int y);
bool		is_wall(int x, int y);
bool		is_solid(int x, int y, t_hitbox *box);
void		render_hitbox(t_renderer *r, int tex_idx, int x, int y);
void		ft_line(t_renderer *r, t_point p0, t_point p1);
t_vector	translate(t_renderer *r, float x, float y);
void		render_hitboxes(t_renderer *r);

typedef void	*(*t_thread)(void *);

void		*counter_thread(t_renderer *r);
void		*render_thread(t_renderer *r);

int	ft_rand(unsigned int seed);

#endif
