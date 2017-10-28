/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sdl_events.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/12 12:49:22 by agrumbac          #+#    #+#             */
/*   Updated: 2017/10/28 18:08:36 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "easy_sdl.h"
#include "wolf3d.h"

static void			turn_cam(t_cam *cam, const float speed)
{
	const float		oldy = cam->direction.y;
	const float		oldplaney = cam->plane.y;

	cam->direction = (t_vector)
	{
		cam->direction.x * sin(speed) + oldy * cos(speed),
		cam->direction.x * cos(speed) - oldy * sin(speed)
	};
	cam->plane = (t_vector)
	{
		cam->plane.x * sin(speed) + oldplaney * cos(speed),
		cam->plane.x * cos(speed) - oldplaney * sin(speed)
	};
}

static int			sdl_keyboard(t_cam *cam)
{
	const Uint8		*state = SDL_GetKeyboardState(NULL);
	t_vector		move;

	if (state[SDL_SCANCODE_W])
		move = (t_vector)
		{
			cam->direction.y * MOVE_SPEED,
			cam->direction.x * MOVE_SPEED
		};
	if (state[SDL_SCANCODE_S])
		move = (t_vector)
		{
			cam->direction.y * -MOVE_SPEED,
			cam->direction.x * -MOVE_SPEED
		};
	cam->origin.x += move.x;
	cam->origin.y += move.y;
	if (state[SDL_SCANCODE_A])
		turn_cam(cam, -TURN_SPEED);
	if (state[SDL_SCANCODE_D])
		turn_cam(cam, TURN_SPEED);
	return (state[SDL_SCANCODE_S] || state[SDL_SCANCODE_W] || \
		state[SDL_SCANCODE_A] || state[SDL_SCANCODE_D]);
}

static int			sdl_mouse(t_sdl *sdl, t_cam *cam)
{
	int				x;

	SDL_GetMouseState(&x, NULL);
	x -= sdl->size.x / 2;
	if (x > 0)
		turn_cam(cam, x * TURN_SPEED / (float)42);
	else
		turn_cam(cam, x * TURN_SPEED / (float)42);
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
