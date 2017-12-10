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
									const t_vector rayDir, int *side)
{
	char			(*maps)[MAP_SIZE][MAP_SIZE] = (void*)map;
	const t_vector		deltaDist =
	{
		sqrt(1 + (rayDir.y * rayDir.y) / (float)(rayDir.x * rayDir.x)),
		sqrt(1 + (rayDir.x * rayDir.x) / (float)(rayDir.y * rayDir.y))
	};
	t_xy				mapPos =
	{
		(int)origin.x,
		(int)origin.y
	};
	const t_xy			step =
	{
		(rayDir.x < 0) ? -1 : 1,
		(rayDir.y < 0) ? -1 : 1
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
	 	/ (float)2) / (float)((float *)&rayDir)[*side]));
}

static void			put_pixel(__global uint *pixels, const int x, const int y,
	 					const uint color)
{
	const int 		size_x = get_global_size(0);
	const int 		size_y = get_global_size(1);
	uint			(*pixel)[size_y][size_x] = (void*)pixels;

	(*pixel)[y][x] = color;
}

static uint			get_color_on_texture(__global uint *textures, \
						const int texture_number, const int x, const int y)
{
	const uint		(*texture)[4][TEXTURE_SIZE * TEXTURE_SIZE] = (void*)textures;

	return ((*texture)[texture_number][y * TEXTURE_SIZE + x]);
}

static int			get_wall_direction()
{
	return (0);//0, 1, 2, 3 NSEW
}

__kernel void		core(__constant char *map, __global uint *textures,
						__constant t_cam *cam, __global uint *pixels)
{
	const int 		x = get_global_id(0);
	const int 		y = get_global_id(1);
	const int 		size_y = get_global_size(1);

	const float		cameraX = 2 * x / (float)x - 1;
	const t_vector	rayDir =
	{
		cam->direction.x + cam->plane.x * cameraX,
		cam->direction.y + cam->plane.y * cameraX
	};
	int				side;
	const float 	wallDist = get_height(cam->origin, map, rayDir, &side);
	const float 	wallX = ((float *)&cam->origin)[1 - side] \
						+ wallDist * ((float *)&rayDir)[1 - side];
	const uint		wall_height = size_y / wallDist;

	int				color;
	if (abs(y - size_y / 2) < wall_height / 2)
	{
		const int		wall_direction = get_wall_direction();
		const int		wall_x = (uint)((wallX - (int)wallX) * TEXTURE_SIZE);
		const int		wall_y = 42;
		color = get_color_on_texture(textures, wall_direction, wall_x, wall_y);
	}
	else if (y < size_y / 2)
		color = SKYBOX;
	else
		color = GROUND;
	put_pixel(pixels, x, y, color);
}
