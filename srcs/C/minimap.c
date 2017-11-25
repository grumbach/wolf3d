/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: angavrel <angavrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/25 15:21:14 by angavrel          #+#    #+#             */
/*   Updated: 2017/11/25 20:54:07 by angavrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"


int			toggle_minimap(int *display_mm, int *radius, int key)
{
	if (key == SDLK_m)
		*display_mm = !*display_mm;
	else if (key == SDLK_KP_PLUS && *radius > 3)
	{
		printf("heeherh\n");
			--*radius;
	}

	else if (key == SDLK_KP_MINUS && *radius < 30)
		++*radius;
	return (EVENT_UPDATE);
}

void		display_minimap(t_sdl *sdl, const char map[MAP_SIZE][MAP_SIZE],\
								t_vector dir, t_vector pos)
{
	const int mm_size = 180;
	const int wall_block = mm_size / sdl->radius;

	SDL_Surface*    minimap;
	SDL_Surface*    wall;
	SDL_Surface*    player;

	t_vector osef = pos;//to be removed
	osef = dir;//to be removed

	minimap = SDL_CreateRGBSurface(0, mm_size, mm_size, 32, 0, 0xff, 0x50, 0);
	SDL_FillRect(minimap, NULL, SDL_MapRGB(minimap->format, 0, 0xff, 0x50));

	wall = SDL_CreateRGBSurface(0, wall_block, wall_block, 32, 0, 0, 0, 0);
	SDL_FillRect(wall, NULL, SDL_MapRGB(wall->format, 15, 15, 15));

	SDL_Rect offset;
	offset.x = mm_size / 2 - wall_block / 2;
	offset.y = mm_size / 2 - wall_block / 2;
	player = SDL_CreateRGBSurface(0, wall_block / 2, wall_block / 2, 32, 0, 0, 0, 0);
	SDL_BlitSurface(player, 0, minimap, &offset);

	SDL_Rect i;
	const int a = mm_size / 2 / wall_block;
	i.y = 0;
	while (i.y < mm_size / wall_block)
	{
		i.x = 0;
		while (i.x < mm_size / wall_block)
		{
			if (pos.x + i.x - a > -1 && pos.y + i.y - a > -1 \
				&& pos.x + i.x - a < MAP_SIZE && pos.y + i.y - a < MAP_SIZE \
				&& map[(int)pos.x + i.x - a][(int)pos.y + i.y - a] != '0')
				{
					SDL_Rect j;
					j.x = i.x * wall_block;
					j.y = i.y * wall_block;
					SDL_BlitSurface(wall, 0, minimap, &j);
				}
			++i.x;
		}
		++i.y;
	}
	SDL_BlitSurface(player, 0, minimap, &offset);
	SDL_BlitSurface(minimap, 0, sdl->screen, 0);
	SDL_UpdateWindowSurface(sdl->window);
}
