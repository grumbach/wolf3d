/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wolf3d.cl                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: angavrel <angavrel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/18 23:53:40 by angavrel          #+#    #+#             */
/*   Updated: 2017/06/27 21:51:41 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "wolf3d.h.cl"

static float			get_height(const t_vector origin, __constant char *map, \
									const float rayDir[2], int *side)
{
	char			(*maps)[MAP_SIZE][MAP_SIZE] = (void*)map;
	const t_vector		deltaDist =
	{
		sqrt(1 + (rayDir[1] * rayDir[1]) / (float)(rayDir[0] * rayDir[0])),
		sqrt(1 + (rayDir[0] * rayDir[0]) / (float)(rayDir[1] * rayDir[1]))
	};
	t_xy				mapPos =
	{
		(int)origin.x,
		(int)origin.y
	};
	const t_xy			step =
	{
		(rayDir[0] < 0) ? -1 : 1,
		(rayDir[1] < 0) ? -1 : 1
	};
	t_vector			sideDist =
	{
		step.x * (mapPos.x - origin.x + (1 + step.x) / (float)2) * deltaDist.x,
		step.y * (mapPos.y - origin.y + (1 + step.y) / (float)2) * deltaDist.y
	};

	while (42)
	{
		*side = !((int)(sideDist.y / sideDist.x));
		((float *)&sideDist)[*side] += ((float *)&deltaDist)[*side];
		((int *)&mapPos)[*side] += ((int *)&step)[*side];
		if ((*maps)[mapPos.x][mapPos.y] != '0')
  			break;
	}
	return ((float)((((int *)&mapPos)[*side] - \
		((float *)&origin)[*side] + (1 - ((int *)&step)[*side]) \
	 	/ (float)2) / rayDir[*side]));
}

__kernel void		core(__constant char *map, __global uint *textures, \
						__constant t_cam *cam, __global uint *wall_height, \
						__global float *wall_index, __global uint *wall_dir)
{
	const int 		x = get_global_id(0) ;
	const float		cameraX = 2 * x / (float)get_global_size(0) - 1;
	const float		rayDir[2] =
	{
		cam->direction.x + cam->plane.x * cameraX,
		cam->direction.y + cam->plane.y * cameraX
	};
	int				side;
	const float 	wallDist = get_height(cam->origin, map, rayDir, &side);
	const float 	wallX = ((float *)&cam->origin)[1 - side] \
						+ wallDist * rayDir[1 - side];

	wall_height[x] = cam->screen_height / wallDist;
	wall_index[x] = wallX - (int)wallX;
	wall_dir[x] = (((rayDir)[side] < 0) + side * 2);
}
