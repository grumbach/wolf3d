/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt.cl                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/18 23:53:40 by agrumbac          #+#    #+#             */
/*   Updated: 2017/06/27 21:51:41 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "wolf3d.h.cl"

static uint			get_color(const t_vector direction, const int side)
{
	const int bright = 1;

	if (direction.y)
		return ((direction.x > 0) ? WALLPINK: WALLCRIMSONRED);
	return ((direction.x > 0) ? WALLSKYBLUE: WALLMIDNIGHTBLUE);
}

static uint			get_height(__constant t_cam *cam, __constant char *map, \
									const float cameraX, int *side)
{
	const char (*maps)[MAP_SIZE][MAP_SIZE] = (void*)map;
	const t_vector		rayDir = (t_vector)
	{
		cam->direction.x + cam->plane.x * cameraX,
		cam->direction.y + cam->plane.y * cameraX
	};
	const t_vector		deltaDist = (t_vector)
	{
		sqrt(1 + (rayDir.y * rayDir.y) / (rayDir.x * rayDir.x)),
		sqrt(1 + 1 / (float)(rayDir.y * rayDir.y) / (rayDir.x * rayDir.x))
	};
	t_xy				mapPos = (t_xy)
	{
		(int)cam->origin.x,
		(int)cam->origin.y
	};
	const t_vector		step = (t_vector)
	{
		(rayDir.x < 0) ? -1 : 1,
		(rayDir.y < 0) ? -1 : 1
	};

	const t_vector		sideDist = (t_vector)
	{
		step.x * (mapPos.x - cam->origin.x + (1 + step.x) / 2) * deltaDist.x,
		step.y * (mapPos.y - cam->origin.y + (1 + step.y) / 2) * deltaDist.y
	};
	int					hit = 0;

	while (!hit)
	{
		if (sideDist.x < sideDist.y)
		{
			sideDist.x += deltaDist.x;
			mapPos.x += step.x;
			*side = 0;
		}
		else
		{
			sideDist.y += deltaDist.y;
			mapPos.y += step.y;
			*side = 1;
		}
		if ((*maps)[mapPos.x][mapPos.y] > 0)
			hit = 1;
	}
	if (!*side)
		return (cam->screen_height / (mapPos.x - cam->origin.x + (1 - step.x) / 2) / rayDir.x);
	return (cam->screen_height / (mapPos.y - cam->origin.y + (1 - step.y) / 2) / rayDir.y);
}

__kernel void		core(__constant char *map, __constant t_cam *cam, \
					__global uint *wall_height, __global uint *wall_color)
{
	const int 		x = get_global_id(0);
	const int 		w = get_global_size(0);
	const float		cameraX = 2 * x / float(w) - 1;
	int				side;

	wall_height[x] = get_height(cam, map, cameraX, &side);
	wall_color[x] = get_color(cam->direction, side);
}
