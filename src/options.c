/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   options.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smamalig <smamalig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 19:03:13 by smamalig          #+#    #+#             */
/*   Updated: 2025/04/08 14:04:29 by smamalig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

void	options_init_default(struct s_options *opt)
{
	opt->velocity = 3;
	opt->gravity = 0.2;
	opt->friction = 0.9;
	opt->jump_force = 5.8;
	opt->dash_multiplier = 3.5;
	opt->dash_frames = 10;
	opt->fps = 60;
	opt->map_width = 32;
	opt->map_height = 32;
}
