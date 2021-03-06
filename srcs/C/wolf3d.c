/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wolf3d.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/18 23:52:41 by agrumbac          #+#    #+#             */
/*   Updated: 2018/01/30 16:35:15 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

/*
** cl_run has 4 args
**     - const[r--] the map
**     - const[r--] the textures
**     -      [r--] the camera
**     -      [-w-] the pixels
*/

static uint			get_wall_color(t_sdl *sdl, const uint dir, \
							const float wall_x, const uint wall_y)
{
	uint			(*texture)[TEXTURE_SIZE][TEXTURE_SIZE];

	texture = (void *)sdl->texture[dir]->pixels;
	return (*texture)[(int)(wall_x * TEXTURE_SIZE)][wall_y];
}

static void			map_redraw(t_sdl *sdl, t_xy i)
{
	float			wall_index;
	int				wall_dir;
	int				h;
	int				step;
	uint32_t		(*pixels)[sdl->size.y][sdl->size.x];

	pixels = (void*)sdl->pixels;
	while (i.x--)
	{
		i.y = sdl->size.y;
		wall_index = *((float *)&((*pixels)[sdl->size.y / 2][i.x]));
		wall_dir = (*pixels)[sdl->size.y - 1][i.x];
		h = (*pixels)[0][i.x] > (uint)i.y ? i.y : (int)(*pixels)[0][i.x];
		step = sdl->size.y - ((sdl->size.y - h) / 2);
		while (--i.y > step)
			(*pixels)[i.y][i.x] = GROUND;
		step -= h;
		i.y++;
		while (--i.y > step)
			(*pixels)[i.y][i.x] = get_wall_color(sdl, wall_dir, wall_index, \
			(abs(i.y - sdl->size.y / 2 - h / 2) / (float)h) * TEXTURE_SIZE);
		i.y++;
		while (--i.y >= 0)
			(*pixels)[i.y][i.x] = SKYBOX;
	}
}

/*
** NB sdl->pixels used in a wicked way : some lines used as exchange buffers
** for cl_run() kernel :
**     first line for uint * wall_h (in px)
**     mid line for float * wall_index (hit index ratio 0 > x > 1)
**     last line for uint* wall_dir (NSWE direction 0, 1, 2, 3)
*/

static void			game_loop(const char map[MAP_SIZE][MAP_SIZE], \
						t_cl *cl, t_sdl *sdl)
{
	t_cam			cam;
	int				loop;
	t_arg			arg[4];

	cam = (t_cam){(t_vector){22, 12}, (t_vector){-1, 0}, \
				(t_vector){0, 0.66}, sdl->size.y, 42};
	loop = EVENT_UPDATE;
	while (loop)
	{
		if (loop & EVENT_UPDATE)
		{
			arg[0] = (t_arg){&cam, sizeof(t_cam), CL_MEM_READ_ONLY};
			arg[1] = (t_arg){sdl->pixels, sizeof(uint) * sdl->size.x,
				CL_MEM_WRITE_ONLY};
			arg[2] = (t_arg){(sdl->pixels + ((sdl->size.y / 2) * sdl->size.x)),
				sizeof(uint) * sdl->size.x, CL_MEM_WRITE_ONLY};
			arg[3] = (t_arg){sdl->pixels + sdl->size.y * sdl->size.x -
				sdl->size.x, sizeof(uint) * sdl->size.x, CL_MEM_WRITE_ONLY};
			cl_run(cl, (size_t[WORK_DIM]) {sdl->size.x}, 4, arg);
			map_redraw(sdl, (t_xy){sdl->size.x, sdl->size.y});
			sdl_run(sdl, map, &cam);
		}
		loop = sdl_events(map, sdl, &cam);
	}
}

static void			game_init(const char map[MAP_SIZE][MAP_SIZE], \
						t_cl *cl, t_sdl *sdl)
{
	uint32_t		textures[NB_TEXTURES][TEXTURE_SIZE * TEXTURE_SIZE];
	int				i;
	const size_t	map_size = MAP_SIZE * MAP_SIZE * sizeof(char);
	const size_t	texture_size = TEXTURE_SIZE * TEXTURE_SIZE * sizeof(uint);
	const t_arg		args[2] = {
		(t_arg) {(void *)map, map_size, CL_MEM_READ_ONLY},
		(t_arg) {textures, NB_TEXTURES * texture_size, CL_MEM_READ_ONLY}};

	ft_bzero(textures, sizeof(textures));
	sdl_init(sdl, PROGRAM_NAME);
	i = -1;
	while (++i < NB_TEXTURES)
		ft_memcpy(&(textures[i][0]), sdl->texture[i]->pixels, texture_size);
	cl_init(cl, 2, args);
}

int					main(int ac, char **av)
{
	char			map[MAP_SIZE][MAP_SIZE];
	t_cl			cl;
	t_sdl			sdl;

	if (ac != 2)
		errors(ERR_USAGE, "bad number of args");
	ft_bzero(&map, sizeof(map));
	ft_bzero(&cl, sizeof(cl));
	ft_bzero(&sdl, sizeof(sdl));
	parse_map(av[1], (void*)map);
	game_init(map, &cl, &sdl);
	game_loop(map, &cl, &sdl);
	cl_end(&cl);
	sdl_end(&sdl);
	return (EXIT_SUCCESS);
}
