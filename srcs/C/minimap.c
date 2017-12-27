/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: angavrel <angavrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/25 15:21:14 by angavrel          #+#    #+#             */
/*   Updated: 2017/12/27 18:06:41 by Anselme          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static void	draw_walls(const char map[MAP_SIZE][MAP_SIZE], SDL_Surface *minimap,
	t_vector pos, const int block)
{
	const int		mm_size = 180;
	const int		a = mm_size / 2 / block;
	SDL_Surface		*wall;
	SDL_Rect		i;
	SDL_Rect		j;

	wall = SDL_CreateRGBSurface(0, block, block, 32, 0, 0, 0, 0);
	SDL_FillRect(wall, NULL, SDL_MapRGB(wall->format, 15, 15, 15));
	i.y = mm_size / block;
	while (i.y--)
	{
		i.x = mm_size / block;
		while (i.x--)
		{
			if ((int)pos.x + i.x - a > -1 && (int)pos.x + i.x - a < MAP_SIZE \
			&& (int)pos.y + i.y - a > -1 && (int)pos.y + i.y - a < MAP_SIZE \
			&& map[(int)pos.x + i.x - a][(int)pos.y + i.y - a] != '0')
			{
				j.x = i.x * block;
				j.y = i.y * block;
				SDL_BlitSurface(wall, 0, minimap, &j);
			}
		}
	}
}

void		display_minimap(t_sdl *sdl, const char map[MAP_SIZE][MAP_SIZE], \
								t_vector pos)
{
	const int		mm_size = 180;
	const int		block = mm_size / MINIMAP_RADIUS;
	SDL_Surface		*minimap;
	SDL_Surface		*me;
	SDL_Rect		offset;

	minimap = SDL_CreateRGBSurface(0, mm_size, mm_size, 32, \
		0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
	draw_walls(map, minimap, pos, block);
	offset.x = (mm_size - block) / 2;
	offset.y = (mm_size - block) / 2;
	me = SDL_CreateRGBSurface(0, block / 2, block / 2, 32, 0, 0, 0, 0);
	SDL_BlitSurface(me, 0, minimap, &offset);
	SDL_BlitSurface(minimap, 0, sdl->screen, 0);
	SDL_UpdateWindowSurface(sdl->window);
}
