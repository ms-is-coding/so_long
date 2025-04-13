/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smamalig <smamalig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 16:49:49 by smamalig          #+#    #+#             */
/*   Updated: 2025/04/13 21:09:51 by smamalig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

void	*counter_thread(t_game *g)
{
	while (g->state.is_running)
	{
		ft_sleep(1);
		ft_printf("\r%i FPS    ", g->state.frame_count);
		g->state.frame_count = 0;
	}
	return NULL;
}

void	*render_thread(t_game *g)
{
	while (g->state.is_running)
	{
		ft_usleep(1000000UL / g->opt.fps);
		g->state.should_render = 1;
	}
	return (NULL);
}

void	launch_threads(t_game *g)
{
	pthread_create(&g->threads.counter, NULL, (t_thread)counter_thread, g);
	pthread_create(&g->threads.render, NULL, (t_thread)render_thread, g);
}
