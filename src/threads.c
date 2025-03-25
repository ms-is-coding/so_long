/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smamalig <smamalig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 16:49:49 by smamalig          #+#    #+#             */
/*   Updated: 2025/03/25 15:50:37 by smamalig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

void	*counter_thread(t_renderer *r)
{
	while (r->is_running)
	{
		ft_sleep(1);
		ft_printf("\r%i FPS    ", r->frame_count);
		r->frame_count = 0;
	}
	return NULL;
}

void	*render_thread(t_renderer *r)
{
	while (r->is_running)
	{
		ft_usleep(1000000UL / FRAMES_PER_SECOND);
		r->should_render = 1;
	}
	return (NULL);
}

