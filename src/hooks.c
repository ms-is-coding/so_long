/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hooks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smamalig <smamalig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 15:49:48 by smamalig          #+#    #+#             */
/*   Updated: 2025/04/08 20:22:18 by smamalig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"
#include "mlx.h"

#include <X11/X.h>
#include <X11/keysym.h>

int	on_key_press(int keysym, t_game *g)
{
	if (keysym == XK_Escape)
	{
		if (g->state.scene == SCENE_PAUSE_MENU)
			g->state.scene = SCENE_LEVEL;
		else if (g->state.scene == SCENE_LEVEL)
			g->state.scene = SCENE_PAUSE_MENU;
		else if (g->state.scene == SCENE_OPTIONS_MENU)
			g->state.scene = SCENE_PAUSE_MENU;
	}
	if (keysym == XK_a || keysym == XK_Left)
		g->input.keys.left = 1;
	if (keysym == XK_d || keysym == XK_Right)
		g->input.keys.right = 1;
	if (g->state.scene != SCENE_LEVEL)
		return (0);
	if (keysym == XK_w || keysym == XK_Up || keysym == XK_space)
		g->state.should_jump = 1;
	if (keysym == XK_g)
		g->debug_mode ^= 1;
	if (keysym == XK_Shift_L || keysym == XK_Shift_R)
		g->state.should_dash = g->opt.dash_frames;
	return (0);
}

int	on_key_release(int keysym, t_game *g)
{
	if (keysym == XK_a || keysym == XK_Left)
		g->input.keys.left = 0;
	if (keysym == XK_d || keysym == XK_Right)
		g->input.keys.right = 0;
	return (0);
}

// buttons:
// 1 - LMB
// 2 - MMB
// 3 - RMB
// 4 - scroll up
// 5 - scroll down

int	on_mouse_down(int button, int x, int y, t_game *g)
{
	g->input.mouse.x = x;
	g->input.mouse.y = y;
	g->input.mouse.left = button == 1;
	g->input.mouse.right = button == 3;
	return (0);
}

int	on_mouse_up(int button, int x, int y, t_game *g)
{
	g->input.mouse.x = x;
	g->input.mouse.y = y;
	g->input.mouse.left ^= button == 1;
	g->input.mouse.right ^= button == 3;
	return (0);
}

int	on_mouse_move(int x, int y, t_game *g)
{
	g->input.mouse.x = x;
	g->input.mouse.y = y;
	return (0);
}

int	ft_register_hooks(t_game *g)
{
	mlx_hook(g->win, KeyPress, KeyPressMask, on_key_press, g);
	mlx_hook(g->win, KeyRelease, KeyReleaseMask, on_key_release, g);
	mlx_hook(g->win, DestroyNotify, 0, on_destroy, g);
	mlx_hook(g->win, ButtonPress, ButtonPressMask, on_mouse_down, g);
	mlx_hook(g->win, ButtonRelease, ButtonReleaseMask, on_mouse_up, g);
	mlx_hook(g->win, MotionNotify, PointerMotionMask, on_mouse_move, g);
	return (0);
}
