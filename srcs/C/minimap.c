/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: angavrel <angavrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/11/25 15:21:14 by angavrel          #+#    #+#             */
/*   Updated: 2017/11/25 16:59:16 by angavrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

/*
int		refresh_minimap()
{

}
*/

int		display_minimap(SDL_Window* window, const char map[MAP_SIZE][MAP_SIZE],\
								t_vector dir, t_vector pos)
{
	const int width = 200;
	const int height = 200;
	SDL_Surface*    minimap;
	#define RADIUS = 6;
	//SDL_Renderer *renderer = NULL;
	t_vector osef = pos;
	osef = dir;

	if (map[0][0] == 1)
		printf("ok");
	//renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
	minimap = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
	SDL_FillRect(minimap, NULL, SDL_MapRGB(minimap->format, 0, 0xff, 0x20));
//	wall = SDL_CreateRGBSurface(0, width, height, 32, 0, 0, 0, 0);
//	SDL_FillRect(wall, NULL, SDL_MapRGB(wall->format, 0, 0, 0));
	minimap = SDL_GetWindowSurface(window);

//	SDL_BlitSurface( gHelloWorld, &srcrect, gScreenSurface, &dstrect );
//	SDL_SetSurfaceAlphaMod(minimap, 0); //minimap transparency

	SDL_UpdateWindowSurface(window);
	//while (i <)

	return (EVENT_UPDATE);
}
