/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sdl_events.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/12 12:49:22 by agrumbac          #+#    #+#             */
/*   Updated: 2017/11/26 17:59:49 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "easy_sdl.h"
#include "wolf3d.h"

static void			turn_cam(t_cam *cam, const float speed)
{
	const float		s = sin(speed);
	const float		c = cos(speed);

	cam->direction = (t_vector)
	{
		cam->direction.x * c - cam->direction.y * s,
		cam->direction.x * s + cam->direction.y * c
	};
	cam->plane = (t_vector)
	{
		cam->plane.x * c - cam->plane.y * s,
		cam->plane.x * s + cam->plane.y * c
	};
}

static void			move_cam(const char map[MAP_SIZE][MAP_SIZE], t_cam *cam, \
								const float speed, const int a)
{
	const t_vector	new = cam->origin;

	cam->origin.x += (a * cam->direction.x + (1 - a) * cam->plane.x) * speed;
	cam->origin.y += (a * cam->direction.y + (1 - a) * cam->plane.y) * speed;
	if (map[(int)cam->origin.x][(int)cam->origin.y] != '0')
	{
		if (map[(int)new.x][(int)cam->origin.y] != '0')
			cam->origin.y = new.y;
		else if (map[(int)cam->origin.x][(int)new.y] != '0')
			cam->origin.x = new.x;
	}
	if (cam->origin.x > MAP_SIZE || cam->origin.x < 0)
		cam->origin.x = new.x;
	if (cam->origin.y > MAP_SIZE || cam->origin.y < 0)
		cam->origin.y = new.y;
}

static void			sdl_keyboard(SDL_Event *event, \
	const char map[MAP_SIZE][MAP_SIZE], t_cam *cam, t_minimap *minimap)
{
	const Uint8		*state = SDL_GetKeyboardState(NULL);

	if (state[SDL_SCANCODE_W])
		move_cam(map, cam, MOVE_SPEED, 1);
	if (state[SDL_SCANCODE_S])
		move_cam(map, cam, -MOVE_SPEED, 1);
	if (state[SDL_SCANCODE_Q])
		turn_cam(cam, TURN_SPEED);
	if (state[SDL_SCANCODE_E])
		turn_cam(cam, -TURN_SPEED);
	if (state[SDL_SCANCODE_A])
		move_cam(map, cam, -MOVE_SPEED / 2, 0);
	if (state[SDL_SCANCODE_D])
		move_cam(map, cam, MOVE_SPEED / 2, 0);
	if (state[SDL_SCANCODE_ESCAPE])
		event->type = EVENT_STOP;
	if (state[SDL_SCANCODE_KP_PLUS] && minimap->radius > 5)
		--minimap->radius;
	else if (state[SDL_SCANCODE_KP_MINUS] && minimap->radius < 20)
		++minimap->radius;
	if (state[SDL_SCANCODE_M] && event->type == SDL_KEYDOWN)
		minimap->display = !minimap->display;
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

int					sdl_events(const char map[MAP_SIZE][MAP_SIZE], t_sdl *sdl,
						t_cam *cam)
{
	t_xy			window_size;
	SDL_Event		event;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT || (event.type == SDL_WINDOWEVENT && \
			event.window.type == SDL_WINDOWEVENT_CLOSE))
			return (EVENT_STOP);
		SDL_GetWindowSize(sdl->window, &window_size.x, &window_size.y);
		if (window_size.x != sdl->size.x || window_size.y != sdl->size.y)
		{
			window_size = sdl->size;
			return (sdl_init_window(sdl));
		}
		sdl_keyboard(&event, map, cam, &sdl->minimap);
		return (event.type |= (event.type == EVENT_STOP) ?
						0 : sdl_mouse(sdl, cam));
	}
	return (EVENT_IDLE);
}
