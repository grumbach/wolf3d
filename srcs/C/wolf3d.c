/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wolf3d.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/18 23:52:41 by agrumbac          #+#    #+#             */
/*   Updated: 2017/11/19 17:08:59 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

/*
** cl_run has 4 args
**     - [r--] the map
**     - [r--] the camera
**     - [-w-] wall_heights
**       (written in the first line of pixeltab, for memory efficiency)
**     - [-w-] wall_colors
**       (written in the middle of pixeltab, they won't need to change color..)
*/

static void	map_redraw(t_sdl *sdl)
{
	t_xy		i;
	uint		wall_color;
	int			height;
	int			step;

	uint32_t(*pixels)[sdl->size.y][sdl->size.x];
	pixels = (void*)sdl->pixels;
	i = (t_xy){sdl->size.x, sdl->size.y};
	while (i.x--)
	{
		i.y = sdl->size.y;
		wall_color = (*pixels)[sdl->size.y / 2][i.x];
		height = (*pixels)[0][i.x] > (uint)sdl->size.y ? sdl->size.y : \
			(int)(*pixels)[0][i.x];
		step = sdl->size.y - ((sdl->size.y - height) / 2);
		while (--i.y > step)
			(*pixels)[i.y][i.x] = GROUND;
		step = sdl->size.y - height - ((sdl->size.y - height) / 2);
		i.y++;
		while (--i.y > step)
			(*pixels)[i.y][i.x] = wall_color;
		i.y++;
		while (--i.y >= 0)
			(*pixels)[i.y][i.x] = SKYBOX;
	}
}

static void	main_loop(t_cl *cl, t_sdl *sdl)
{
	t_cam		cam;
	int			loop;

	cam = (t_cam){(t_vector){22, 12}, (t_vector){-1, 0}, \
				(t_vector){0, 0.66}, sdl->size.y, 0};
	loop = EVENT_UPDATE;
	while (loop)
	{
		if (loop & EVENT_UPDATE)
		{
			cl_run(cl, (size_t[WORK_DIM]) {sdl->size.x}, 3, (t_arg[3])
			{
				(t_arg) {&cam, sizeof(t_cam), CL_MEM_READ_ONLY}, \
				(t_arg) {sdl->pixels, sizeof(uint) * sdl->size.x, CL_MEM_WRITE_ONLY}, \
				(t_arg) {(sdl->pixels + ((sdl->size.y / 2) * sdl->size.x)), \
					sizeof(uint) * sdl->size.x, CL_MEM_WRITE_ONLY}
			});
			map_redraw(sdl);
			sdl_run(sdl);
		}
		loop = sdl_events(sdl, &cam);
	}
}

static void	parse_map(const char *filename, void *map)
{
	int		fd;

	if ((fd = open(filename, O_RDONLY)) < 0)
		errors(ERR_SYS, 0);
	if (read(fd, (void*)map, MAP_SIZE * MAP_SIZE) != MAP_SIZE * MAP_SIZE)
		errors(ERR_USAGE, "Map too small");
	close(fd);
}

int			main(int ac, char **av)
{
	char	map[MAP_SIZE][MAP_SIZE];
	t_cl	cl;
	t_sdl	sdl;

	if (ac != 2)
		errors(ERR_USAGE, "bad number of args");

	ft_bzero(&cl, sizeof(cl));
	ft_bzero(&sdl, sizeof(sdl));
	ft_bzero(&map, sizeof(map));

	parse_map(av[1], (void*)map);

	cl_init(&cl, 1, (t_arg[1]) {(t_arg) {map, sizeof(map), CL_MEM_READ_ONLY}});
	sdl_init(&sdl, PROGRAM_NAME);

	main_loop(&cl, &sdl);

	cl_end(&cl);
	sdl_end(&sdl);

	return (EXIT_SUCCESS);
}
