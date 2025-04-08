/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smamalig <smamalig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 14:14:42 by smamalig          #+#    #+#             */
/*   Updated: 2025/04/08 23:19:41 by smamalig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SO_LONG_H
# define SO_LONG_H

# include "libft.h"

# include <pthread.h>
# include <stdatomic.h>
# include <stdbool.h>

# define DEBUG_COLOR 0xff0000

# define PARALLAX_LAYERS 1
# define PARALLAX_CONSTANT 0.8

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
	TEX_COLLECTIBLE,
	TEX_FONT,
	TEX_COUNT
};

typedef struct s_game
{
	void		*mlx;
	void		*win;
	void		*frame;
	void		*frame2;
	void		*parallaxes[PARALLAX_LAYERS];
	void		*textures[TEX_COUNT];
	struct s_player
	{
		float	px;
		float	py;
		float	x;
		float	y;
		float	vx;
		float	vy;
		int		w;
		int		h;
	}	player;
	struct s_threads
	{
		pthread_t	counter;
		pthread_t	render;
	}	threads;
	struct s_options
	{
		float	velocity;
		float	gravity;
		float	friction;
		float	jump_force;
		float	dash_multiplier;
		float	dash_frames;
		float	collision_offset;
		int		fps;
		int		map_width;
		int		map_height;
	}	opt;
	struct s_input
	{
		struct s_mouse
		{
			int		x;
			int		y;
			bool	left;
			bool	right;
		}	mouse;
		struct s_keys
		{
			bool	left;
			bool	right;
		}	keys;
	}	input;
	struct s_state
	{
		enum	e_scene {
			SCENE_MAIN_MENU,
			SCENE_LEVEL,
			SCENE_PAUSE_MENU,
			SCENE_OPTIONS_MENU,
		} scene;
		uint8_t		should_dash;
		uint8_t		should_jump;
		atomic_int	is_running;
		atomic_int	frame_count;
		atomic_int	should_render;
	}	state;
	struct s_time
	{
		int		frame_count;
		int		frame_rate;
		float	frame_time;
		float	frame_delta;
		float	frame_last;
	}	time;
	int			debug_mode;
	t_rect		window;
	t_rect		map;
	char		**map_matrix;
}	t_game;

int			ft_render_map(t_game *g);
int			ft_generate_background(t_game *g);
int			ft_load_textures(t_game *g);
t_hitbox	ft_absolute_hitbox(t_game *g, int x, int y);
int			get_texture_index(int mask);
int			compute_texture_mask(t_game *g, int x, int y);
bool		is_wall(t_game *g, int x, int y);
bool		is_solid(t_game *g, int x, int y, t_hitbox *box);
void		render_hitbox(t_game *g, int tex_idx, int x, int y);
void		ft_line(t_game *g, t_point p0, t_point p1);
t_vector	translate(t_game *g, float x, float y);
void		render_hitboxes(t_game *g);
void		options_init_default(struct s_options *opt);

typedef void	*(*t_thread)(void *);

// threads
void		launch_threads(t_game *g);
void		*counter_thread(t_game *g);
void		*render_thread(t_game *g);

// hooks
int			on_key_press(int keysym, t_game *g);
int			on_key_release(int keysym, t_game *g);
int			on_destroy(t_game *g);
int			ft_register_hooks(t_game *g);

void		ft_debug(char *msg);

#endif
