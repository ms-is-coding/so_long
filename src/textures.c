/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smamalig <smamalig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 14:13:32 by smamalig          #+#    #+#             */
/*   Updated: 2025/03/27 18:07:14 by smamalig         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"
#include "mlx.h"
#include "libft.h"

static char	*g_texture_files[TEX_COUNT] = {
[TEX_EMPTY] = "assets/empty.xpm",
[TEX_WALL_TOP] = "assets/wall-top.xpm",
[TEX_WALL_BOTTOM] = "assets/wall-bottom.xpm",
[TEX_WALL_LEFT] = "assets/wall-left.xpm",
[TEX_WALL_RIGHT] = "assets/wall-right.xpm",
[TEX_CORNER_TL] = "assets/corner-tl.xpm",
[TEX_CORNER_TR] = "assets/corner-tr.xpm",
[TEX_CORNER_BL] = "assets/corner-bl.xpm",
[TEX_CORNER_BR] = "assets/corner-br.xpm",
[TEX_CORNER_EXT_TL] = "assets/corner-ext-tl.xpm",
[TEX_CORNER_EXT_TR] = "assets/corner-ext-tr.xpm",
[TEX_CORNER_EXT_BL] = "assets/corner-ext-bl.xpm",
[TEX_CORNER_EXT_BR] = "assets/corner-ext-br.xpm",
[TEX_CORNER_TJUNC_T] = "assets/corner-tjunc-t.xpm",
[TEX_CORNER_TJUNC_B] = "assets/corner-tjunc-b.xpm",
[TEX_CORNER_TJUNC_L] = "assets/corner-tjunc-l.xpm",
[TEX_CORNER_TJUNC_R] = "assets/corner-tjunc-r.xpm",
[TEX_JUNC_CROSS] = "assets/junc-cross.xpm",
[TEX_TJUNC_T] = "assets/tjunc-t.xpm",
[TEX_TJUNC_B] = "assets/tjunc-b.xpm",
[TEX_TJUNC_L] = "assets/tjunc-l.xpm",
[TEX_TJUNC_R] = "assets/tjunc-r.xpm",
[TEX_LJUNC_TL] = "assets/ljunc-tl.xpm",
[TEX_LJUNC_TR] = "assets/ljunc-tr.xpm",
[TEX_LJUNC_BL] = "assets/ljunc-bl.xpm",
[TEX_LJUNC_BR] = "assets/ljunc-br.xpm",
[TEX_PLATFORM] = "assets/platform.xpm",
[TEX_PLATFORM_T] = "assets/platform-t.xpm",
[TEX_PLATFORM_B] = "assets/platform-b.xpm",
[TEX_PLATFORM_L] = "assets/platform-l.xpm",
[TEX_PLATFORM_R] = "assets/platform-r.xpm",
[TEX_PLATFORM_H] = "assets/platform-h.xpm",
[TEX_PLATFORM_V] = "assets/platform-v.xpm",
[TEX_HLJUNC_TL] = "assets/hljunc-tl.xpm",
[TEX_HLJUNC_TR] = "assets/hljunc-tr.xpm",
[TEX_HLJUNC_BL] = "assets/hljunc-bl.xpm",
[TEX_HLJUNC_BR] = "assets/hljunc-br.xpm",
[TEX_VLJUNC_TL] = "assets/vljunc-tl.xpm",
[TEX_VLJUNC_TR] = "assets/vljunc-tr.xpm",
[TEX_VLJUNC_BL] = "assets/vljunc-bl.xpm",
[TEX_VLJUNC_BR] = "assets/vljunc-br.xpm",
[TEX_CORNERS_XTL] = "assets/corners-xtl.xpm",
[TEX_CORNERS_XTR] = "assets/corners-xtr.xpm",
[TEX_CORNERS_XBL] = "assets/corners-xbl.xpm",
[TEX_CORNERS_XBR] = "assets/corners-xbr.xpm",
[TEX_DIAG_TLBR] = "assets/diag-tlbr.xpm",
[TEX_DIAG_TRBL] = "assets/diag-trbl.xpm",
[TEX_PLAYER] = "assets/player.xpm",
[TEX_EXIT] = "assets/exit.xpm",
[TEX_LOADING_0] = "assets/loading_00.xpm",
[TEX_LOADING_1] = "assets/loading_01.xpm",
[TEX_LOADING_2] = "assets/loading_02.xpm",
[TEX_LOADING_3] = "assets/loading_03.xpm",
[TEX_LOADING_4] = "assets/loading_04.xpm",
[TEX_LOADING_5] = "assets/loading_05.xpm",
[TEX_LOADING_6] = "assets/loading_06.xpm",
[TEX_LOADING_7] = "assets/loading_07.xpm",
[TEX_LOADING_8] = "assets/loading_08.xpm",
[TEX_LOADING_9] = "assets/loading_09.xpm",
[TEX_LOADING_10] = "assets/loading_10.xpm",
[TEX_LOADING_11] = "assets/loading_11.xpm",
[TEX_LOADING_12] = "assets/loading_12.xpm",
[TEX_LOADING_13] = "assets/loading_13.xpm",
[TEX_COLLECTIBLE] = "assets/collectible.xpm",
};

static int	ft_load_texture(t_renderer *renderer, char *name, int index)
{
	int			width;
	int			height;
	void		*image;

	if (!name)
	{
		ft_printf("\e[91m[ERR]\e[m Missing texture [%i]\n", index);
		return (1);
	}
	image = mlx_xpm_file_to_image(renderer->mlx, name, &width, &height);
	if (!image)
	{
		ft_printf("\e[91m[ERR]\e[0m Failed to load texture \'%s\'\n", name);
		return (1);
	}
	if (width != TILE_SIZE || height != TILE_SIZE)
	{
		ft_printf("\e[93m[WRN]\e[m Texture size mismatch \'%s\': %ix%i\n",
			name, width, height);
	}
	renderer->textures[index] = image;
	return (0);
}

int	ft_load_textures(t_renderer *renderer)
{
	int	index;

	index = 0;
	while (index < TEX_COUNT)
	{
		if (ft_load_texture(renderer, g_texture_files[index], index))
			return (1);
		index++;
	}
	return (0);
}
