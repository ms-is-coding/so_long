/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smamalig <smamalig@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/22 14:13:32 by smamalig          #+#    #+#             */
/*   Updated: 2025/04/13 11:23:51 by smamalig         ###   ########.fr       */
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
[TEX_PLAYER_IDLE_0] = "assets/player_idle_0.xpm",
[TEX_PLAYER_IDLE_1] = "assets/player_idle_1.xpm",
[TEX_PLAYER_IDLE_2] = "assets/player_idle_2.xpm",
[TEX_PLAYER_IDLE_3] = "assets/player_idle_3.xpm",
[TEX_PLAYER_IDLE_4] = "assets/player_idle_4.xpm",
[TEX_PLAYER_IDLE_5] = "assets/player_idle_5.xpm",
[TEX_PLAYER_IDLE_6] = "assets/player_idle_6.xpm",
[TEX_PLAYER_IDLE_7] = "assets/player_idle_7.xpm",
[TEX_PLAYER_IDLE_8] = "assets/player_idle_8.xpm",
[TEX_PLAYER_IDLE_9] = "assets/player_idle_9.xpm",
[TEX_PLAYER_IDLE_10] = "assets/player_idle_10.xpm",
[TEX_PLAYER_IDLE_11] = "assets/player_idle_11.xpm",
[TEX_PLAYER_IDLE_12] = "assets/player_idle_12.xpm",
[TEX_PLAYER_IDLE_13] = "assets/player_idle_13.xpm",
[TEX_PLAYER_IDLE_14] = "assets/player_idle_14.xpm",
[TEX_PLAYER_IDLE_15] = "assets/player_idle_15.xpm",
[TEX_PLAYER_IDLE_16] = "assets/player_idle_16.xpm",
[TEX_PLAYER_IDLE_17] = "assets/player_idle_17.xpm",
[TEX_PLAYER_IDLE_18] = "assets/player_idle_18.xpm",
[TEX_PLAYER_IDLE_19] = "assets/player_idle_19.xpm",
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
[TEX_SNACK_0] = "assets/snack0.xpm",
[TEX_SNACK_1] = "assets/snack1.xpm",
[TEX_SNACK_2] = "assets/snack2.xpm",
[TEX_SNACK_3] = "assets/snack3.xpm",
[TEX_SNACK_4] = "assets/snack4.xpm",
[TEX_SNACK_5] = "assets/snack5.xpm",
[TEX_SNACK_6] = "assets/snack6.xpm",
[TEX_SNACK_7] = "assets/snack7.xpm",
[TEX_FONT] = "assets/font.xpm"
};

static char	g_texture_lookup[0x100] = {
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
	//
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
	//
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
	//
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
	//
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
	//
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
	//
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
	//
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
	//
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
	//
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
	//
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
	//
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
	//
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
	//
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
	//
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
	//
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

static int	ft_load_texture(t_game *g, char *name, int index)
{
	int			width;
	int			height;
	void		*image;

	if (!name)
	{
		ft_printf("\e[91m[ERR]\e[m Missing texture [%i]\n", index);
		return (1);
	}
	image = mlx_xpm_file_to_image(g->mlx, name, &width, &height);
	if (!image)
	{
		ft_printf("\e[91m[ERR]\e[0m Failed to load texture \'%s\'\n", name);
		return (1);
	}
	g->textures[index] = image;
	return (0);
}

int	ft_load_textures(t_game *g)
{
	int	index;

	index = 0;
	while (index < TEX_COUNT)
	{
		if (ft_load_texture(g, g_texture_files[index], index))
			return (1);
		index++;
	}
	return (0);
}

int	get_texture_index(int mask)
{
	return (g_texture_lookup[mask]);
}

int	compute_texture_mask(t_game *g, int x, int y)
{
	int	mask;

	mask = 0;
	mask |= is_wall(g, x, y - 1) << 7;
	mask |= is_wall(g, x, y + 1) << 6;
	mask |= is_wall(g, x - 1, y) << 5;
	mask |= is_wall(g, x + 1, y) << 4;
	mask |= is_wall(g, x - 1, y - 1) << 3;
	mask |= is_wall(g, x + 1, y - 1) << 2;
	mask |= is_wall(g, x - 1, y + 1) << 1;
	mask |= is_wall(g, x + 1, y + 1) << 0;
	return (mask);
}
