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
	if (side)
		return ((direction.x < 0) ? WALLPINK: WALLCRIMSONRED);
	return ((direction.y < 0) ? WALLSKYBLUE: WALLMIDNIGHTBLUE);
}

static uint			get_height(__constant t_cam *cam, __constant char *map, \
									const float cameraX, int *side)
{
	char 			(*maps)[MAP_SIZE][MAP_SIZE] = (void*)map;
	const t_vector		rayDir =
	{
		cam->direction.x + cam->plane.x * cameraX,
		cam->direction.y + cam->plane.y * cameraX
	};
	const t_vector		deltaDist =
	{
		sqrt(1 + (rayDir.y * rayDir.y) / (float)(rayDir.x * rayDir.x)),
		sqrt(1 + (rayDir.x * rayDir.x) / (float)(rayDir.y * rayDir.y))
	};
	t_xy				mapPos =
	{
		(int)cam->origin.x,
		(int)cam->origin.y
	};
	const t_xy			step =
	{
		(rayDir.x < 0) ? -1 : 1,
		(rayDir.y < 0) ? -1 : 1
	};
	t_vector			sideDist =
	{
		step.x * (mapPos.x - cam->origin.x + (1 + step.x) / 2) * deltaDist.x,
		step.y * (mapPos.y - cam->origin.y + (1 + step.y) / 2) * deltaDist.y
	};
	while (42)
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
			break;
	}
	if (*side == 0)
		return (cam->screen_height / (float)((mapPos.x - cam->origin.x + (1 - step.x) / (float)2) / (float)rayDir.x));
	return (cam->screen_height / (float)((mapPos.y - cam->origin.y + (1 - step.y) / (float)2) / (float)rayDir.y));
}

__kernel void		core(__constant char *map, __constant t_cam *cam, \
					__global uint *wall_height, __global uint *wall_color)
{
	const int 		x = get_global_id(0) ;
	const float		cameraX = 2 * x / (float)get_global_size(0) - 1;
	int				side;

	wall_height[x] = get_height(cam, map, cameraX, &side);
	wall_color[x] = get_color(cam->direction, side);
}
