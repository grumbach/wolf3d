/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sdl.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/21 15:24:34 by agrumbac          #+#    #+#             */
/*   Updated: 2017/12/27 18:31:01 by Anselme          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

int					sdl_init_window(t_sdl *sdl)
{
	SDL_GetWindowSize(sdl->window, &sdl->size.x, &sdl->size.y);
	SDL_FreeSurface(sdl->screen);
	if (!(sdl->screen = SDL_GetWindowSurface(sdl->window)))
		errors(ERR_SDL, "SDL_GetWindowSurface failed --");
	SDL_FreeSurface(sdl->draw_surface);
	free(sdl->pixels);
	if (!(sdl->pixels = ft_memalloc(BPP * sdl->size.x * sdl->size.y)))
		errors(ERR_SYS, 0);
	if (!(sdl->draw_surface = SDL_CreateRGBSurfaceFrom((void*)sdl->pixels, \
		sdl->size.x, sdl->size.y, BPP, 4 * sdl->size.x, \
		0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000)))
		errors(ERR_SDL, "SDL_CreateRGBSurface failed --");
	return (EVENT_UPDATE);
}

void				sdl_init(t_sdl *sdl, const char *window_name)
{
	if (TTF_Init() || SDL_Init(SDL_INIT_VIDEO))
		errors(ERR_SDL, "Init failed --");
	if (!(sdl->texture[0] = IMG_Load(WALL0)) || \
		!(sdl->texture[1] = IMG_Load(WALL1)) || \
		!(sdl->texture[2] = IMG_Load(WALL2)) || \
		!(sdl->texture[3] = IMG_Load(WALL3)))
		errors(ERR_SDL, "Failed to load texture");
	if (!(sdl->window = SDL_CreateWindow(window_name, SDL_WINDOWPOS_CENTERED, \
	SDL_WINDOWPOS_CENTERED, sdl->size.x, sdl->size.y, SDL_WINDOW_RESIZABLE)))
		errors(ERR_SDL, "SDL_CreateWindow failed --");
	SDL_SetWindowMinimumSize(sdl->window, WIN_W, WIN_H);
	sdl_init_window(sdl);
	SDL_WarpMouseInWindow(sdl->window, sdl->size.x / 2, sdl->size.y / 2);
	SDL_ShowCursor(SDL_DISABLE);
	sdl->settings = WOLF_TOGGLE;
}

void				sdl_run(t_sdl *sdl, const char map[MAP_SIZE][MAP_SIZE], \
						t_cam *cam)
{
	if (WOLF_LOCK_MOUSE(sdl->settings))
	{
		SDL_ShowCursor(SDL_DISABLE);
		SDL_WarpMouseInWindow(sdl->window, sdl->size.x / 2, sdl->size.y / 2);
	}
	else
		SDL_ShowCursor(SDL_ENABLE);
	if (SDL_BlitSurface(sdl->draw_surface, NULL, sdl->screen, NULL))
		errors(ERR_SDL, "SDL_BlitSurface failed --");
	if (WOLF_SHOW_MAP(sdl->settings))
		display_minimap(sdl, map, cam->origin);
	if (SDL_UpdateWindowSurface(sdl->window))
		errors(ERR_SDL, "SDL_UpdateWindowSurface failed --");
}

/*
** -- sdl_end() --
** sdl->texture is of type SDL_Surface* [NB_TEXTURES + 1]
** It is NULL terminated.
** Since sdl->texture is not to be used again (freed), it is safe to modify it
*/

void				sdl_end(t_sdl *sdl)
{
	int			i;

	i = 0;
	free(sdl->pixels);
	SDL_FreeSurface(sdl->screen);
	SDL_FreeSurface(sdl->draw_surface);
	while (sdl->texture[i])
		SDL_FreeSurface(sdl->texture[i++]);
	SDL_DestroyWindow(sdl->window);
	SDL_Quit();
}
