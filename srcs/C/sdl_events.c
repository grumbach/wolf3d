/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sdl_events.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/12 12:49:22 by agrumbac          #+#    #+#             */
/*   Updated: 2017/11/01 18:18:05 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "easy_sdl.h"
#include "wolf3d.h"

static void			turn_cam(t_cam *cam, const float speed)
{
	cam->direction = (t_vector)
	{
		cam->direction.x * cos(speed) - cam->direction.y * sin(speed),
		cam->direction.x * sin(speed) + cam->direction.y * cos(speed)
	};
	cam->plane = (t_vector)
	{
		cam->plane.x * cos(speed) - cam->plane.y * sin(speed),
		cam->plane.x * sin(speed) + cam->plane.y * cos(speed)
	};
}

static void			move_cam(t_cam *cam, const float speed)
{
	t_vector		new = (t_vector)
	{
		cam->origin.x + cam->direction.x * speed,
		cam->origin.y + cam->direction.y * speed
	};

	if (new.x > MAP_SIZE || new.x < 0)
		new.x = cam->origin.x;
	if (new.y > MAP_SIZE || new.y < 0)
		new.y = cam->origin.y;
	cam->origin = (t_vector){new.x, new.y};
}

static int			sdl_keyboard(t_cam *cam)
{
	const Uint8		*state = SDL_GetKeyboardState(NULL);

	if (state[SDL_SCANCODE_W])
		move_cam(cam, MOVE_SPEED);
	if (state[SDL_SCANCODE_S])
		move_cam(cam, -MOVE_SPEED);
	if (state[SDL_SCANCODE_A])
		turn_cam(cam, TURN_SPEED);
	if (state[SDL_SCANCODE_D])
		turn_cam(cam, -TURN_SPEED);
	if (state[SDL_SCANCODE_S] || state[SDL_SCANCODE_W] || \
		state[SDL_SCANCODE_A] || state[SDL_SCANCODE_D])
		return (EVENT_UPDATE);
	return (EVENT_IDLE);
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
	SDL_WarpMouseInWindow(sdl->window, sdl->size.x / 2, sdl->size.y / 2);
	return (EVENT_UPDATE);
}

int					sdl_events(t_sdl *sdl, t_cam *cam)
{
	if (SDL_PollEvent(&sdl->event))
	{
		if (sdl->event.window.type == SDL_WINDOWEVENT_CLOSE || \
			sdl->event.key.keysym.sym == SDLK_ESCAPE || \
			sdl->event.type == SDL_QUIT)
			return (EVENT_STOP);
		if (sdl->event.type == SDL_WINDOWEVENT &&
			sdl->event.window.event == SDL_WINDOWEVENT_RESIZED)
			return (sdl_init_window(sdl));
		return (sdl_keyboard(cam) | sdl_mouse(sdl, cam));
	}
	return (EVENT_IDLE);
}
