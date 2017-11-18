/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sdl_events.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/12 12:49:22 by agrumbac          #+#    #+#             */
/*   Updated: 2017/11/18 22:01:14 by angavrel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "easy_sdl.h"
#include "wolf3d.h"

static int			turn_cam(t_cam *cam, const float speed)
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
	return (EVENT_UPDATE);
}

static int			move_cam(t_cam *cam, const float speed, const int a)
{
	t_vector		new;

	new = (t_vector)
	{
		cam->origin.x + (a * cam->direction.x + (1 - a) * cam->plane.x) * speed,
		cam->origin.y + (a * cam->direction.y + (1 - a) * cam->plane.y) * speed
	};
	if (new.x > MAP_SIZE || new.x < 0)
		new.x = cam->origin.x;
	if (new.y > MAP_SIZE || new.y < 0)
		new.y = cam->origin.y;
	cam->origin = (t_vector){new.x, new.y};
	return (EVENT_UPDATE);
}

static int			sdl_keyboard(t_cam *cam)
{
	const Uint8		*state = SDL_GetKeyboardState(NULL);
	int				event;

	event = EVENT_IDLE;
	if (state[SDL_SCANCODE_W])
		event = move_cam(cam, MOVE_SPEED, 1);
	else if (state[SDL_SCANCODE_S])
		event = move_cam(cam, -MOVE_SPEED, 1);
	if (state[SDL_SCANCODE_A])
		event = turn_cam(cam, TURN_SPEED);
	else if (state[SDL_SCANCODE_D])
		event = turn_cam(cam, -TURN_SPEED);
	if (state[SDL_SCANCODE_Q])
		event = move_cam(cam, -MOVE_SPEED / 2, 0);
	else if (state[SDL_SCANCODE_E])
		event = move_cam(cam, MOVE_SPEED / 2, 0);
	return (event);
}

static int			sdl_mouse(t_sdl *sdl, t_cam *cam)
{
	int				x;

	SDL_GetMouseState(&x, NULL);
	x -= sdl->size.x / 2;
	turn_cam(cam, x * -TURN_SPEED / (float)42);
	SDL_WarpMouseInWindow(sdl->window, sdl->size.x / 2, sdl->size.y / 2);
	return (EVENT_UPDATE);
}

int					sdl_events(t_sdl *sdl, t_cam *cam)
{
	t_xy			window_size;

	while (SDL_PollEvent(&sdl->event))
	{
		if ((sdl->event.type == SDL_WINDOWEVENT && \
			sdl->event.window.type == SDL_WINDOWEVENT_CLOSE) || \
			sdl->event.key.keysym.sym == SDLK_ESCAPE || \
			sdl->event.type == SDL_QUIT)
			return (EVENT_STOP);
		SDL_GetWindowSize(sdl->window, &window_size.x, &window_size.y);
		if (window_size.x != sdl->size.x || window_size.y != sdl->size.y)
		{
			window_size = sdl->size;
			return (sdl_init_window(sdl));
		}
		return (sdl_keyboard(cam) | sdl_mouse(sdl, cam));
	}
	return (EVENT_IDLE);
}
