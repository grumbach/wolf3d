/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wolf3d.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/18 23:52:41 by agrumbac          #+#    #+#             */
/*   Updated: 2017/12/10 20:30:58 by agrumbac         ###   ########.fr       */
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

static void	game_loop(const char map[MAP_SIZE][MAP_SIZE], t_cl *cl, t_sdl *sdl)
{
	t_cam		cam;
	int			loop;

	cam = (t_cam){(t_vector){22, 12}, (t_vector){-1, 0}, (t_vector){0, 0.66}};
	loop = EVENT_UPDATE;
	while (loop)
	{
		if (loop & EVENT_UPDATE)
		{
			cl_run(cl, (size_t[WORK_DIM]) {sdl->size.x, sdl->size.y}, 2, \
			(t_arg[2])
			{
				(t_arg) {&cam, sizeof(t_cam), CL_MEM_READ_ONLY},
				(t_arg) {sdl->pixels, \
					sizeof(uint) * sdl->size.x * sdl->size.y, CL_MEM_WRITE_ONLY}
			});
			sdl_run(sdl, map, &cam);
		}
		loop = sdl_events(map, sdl, &cam);
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

static void	game_init(const char map[MAP_SIZE][MAP_SIZE], t_cl *cl, t_sdl *sdl)
{
	uint32_t		textures[NB_TEXTURES][TEXTURE_SIZE * TEXTURE_SIZE];
	int				i;
	const size_t	map_size = MAP_SIZE * MAP_SIZE * sizeof(char);
	const size_t	texture_size = \
					TEXTURE_SIZE * TEXTURE_SIZE * sizeof(uint32_t);

	ft_bzero(textures, sizeof(textures));
	sdl_init(sdl, PROGRAM_NAME);
	i = -1;
	while (++i < NB_TEXTURES)
		ft_memcpy(&(textures[i][0]), sdl->texture[i]->pixels, texture_size);
	cl_init(cl, 2, (t_arg[2])
	{
		(t_arg) {(void *)map, map_size, CL_MEM_READ_ONLY},
		(t_arg) {textures, NB_TEXTURES * texture_size, CL_MEM_READ_ONLY}
	});
}

int			main(int ac, char **av)
{
	char	map[MAP_SIZE][MAP_SIZE];
	t_cl	cl;
	t_sdl	sdl;

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
