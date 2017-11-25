/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sdl.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/21 15:24:34 by agrumbac          #+#    #+#             */
/*   Updated: 2017/11/25 20:42:10 by angavrel         ###   ########.fr       */
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
	if (!(sdl->window = SDL_CreateWindow(window_name, SDL_WINDOWPOS_CENTERED, \
	SDL_WINDOWPOS_CENTERED, sdl->size.x, sdl->size.y, SDL_WINDOW_RESIZABLE)))
		errors(ERR_SDL, "SDL_CreateWindow failed --");
	SDL_SetWindowMinimumSize(sdl->window, WIN_W, WIN_H);
	sdl_init_window(sdl);
	SDL_WarpMouseInWindow(sdl->window, sdl->size.x / 2, sdl->size.y / 2);
	SDL_ShowCursor(SDL_DISABLE);
	sdl->radius = 18;
}

void				sdl_run(t_sdl *sdl)
{
	SDL_WarpMouseInWindow(sdl->window, sdl->size.x / 2, sdl->size.y / 2);
	if (SDL_BlitSurface(sdl->draw_surface, NULL, sdl->screen, NULL))
		errors(ERR_SDL, "SDL_BlitSurface failed --");
	if (SDL_UpdateWindowSurface(sdl->window))
		errors(ERR_SDL, "SDL_UpdateWindowSurface failed --");
}

void				sdl_end(t_sdl *sdl)
{
	free(sdl->pixels);
	SDL_FreeSurface(sdl->screen);
	SDL_FreeSurface(sdl->draw_surface);
	SDL_DestroyWindow(sdl->window);
	SDL_Quit();
}
