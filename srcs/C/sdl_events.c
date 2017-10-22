/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sdl_events.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/12 12:49:22 by agrumbac          #+#    #+#             */
/*   Updated: 2017/10/22 14:11:06 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "easy_sdl.h"
#include "wolf3d.h"

static int			sdl_keyboard(t_cam *cam)
{
	const Uint8		*state = SDL_GetKeyboardState(NULL);

	if (state[SDL_SCANCODE_W])
		cam->origin.y -= 0.1;
	if (state[SDL_SCANCODE_S])
		cam->origin.y += 0.1;
	if (state[SDL_SCANCODE_A])
		cam->origin.x += 0.1;
	if (state[SDL_SCANCODE_D])
		cam->origin.x -= 0.1;
	if (state[SDL_SCANCODE_S] || state[SDL_SCANCODE_W] || \
		state[SDL_SCANCODE_A] || state[SDL_SCANCODE_D])
		return (1);
	return (0);
}

static int			sdl_mouse(t_sdl *sdl, t_cam *cam)
{
	int				x;
	int				dx;
	float			oldy;

	SDL_GetMouseState(&x, NULL);
	dx = (sdl->size.x / 2) - x;
	oldy = cam->direction.y;
	if (x > 0)
	{
		cam->direction = (t_vector)
		{
			cam->direction.x * sin(-dx) + oldy * cos(-dx),
			cam->direction.x * cos(-dx) - oldy * sin(-dx)
		};
	}
	else
	{
		cam->direction = (t_vector)
		{
			cam->direction.x * sin(dx) + oldy * cos(dx),
			cam->direction.x * cos(dx) - oldy * sin(dx)
		};
	}
	return (EVENT_UPDATE);
}

int					sdl_events(t_sdl *sdl, t_cam *cam)
{
	if (!(SDL_WaitEvent(&sdl->event)))
		errors(ERR_SDL, "SDL_WaitEvent failed --");
	if (SDL_PollEvent(&sdl->event))
	{
		if (sdl->event.window.type == SDL_WINDOWEVENT_CLOSE || \
			sdl->event.key.keysym.sym == SDLK_ESCAPE || \
			sdl->event.type == SDL_QUIT)
			return (0);
		if (sdl->event.type == SDL_WINDOWEVENT &&
			sdl->event.window.event == SDL_WINDOWEVENT_RESIZED)
		{
			sdl_init_window(sdl);
			return (EVENT_UPDATE);
		}
		if (sdl_keyboard(cam))
			return (EVENT_UPDATE);
		if (sdl->event.type == SDL_MOUSEMOTION)
			return (sdl_mouse(sdl, cam));
		SDL_WarpMouseInWindow(sdl->window, sdl->size.x / 2, sdl->size.y / 2);
	}
	return (EVENT_IDLE);
}
