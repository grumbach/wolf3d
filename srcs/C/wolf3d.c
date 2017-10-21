/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wolf3d.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/18 23:52:41 by agrumbac          #+#    #+#             */
/*   Updated: 2017/10/21 14:46:54 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

static void	main_loop(t_cl *cl, t_sdl *sdl)
{
	t_cam		cam;
	t_vector	col_hei;
	int			loop;

	cam = (t_cam){(t_vector){0, 0}, (t_vector){0, 0}, 90};
	loop = EVENT_UPDATE;
	while (loop)
	{
		if (loop == EVENT_UPDATE)
		{
			cl_run(cl, (size_t[WORK_DIM]){sdl->size.x}, 2, \
				(t_arg){&cam, sizeof(t_cam), CL_MEM_READ_ONLY}, \
				(t_arg){&col_hei, sizeof(t_vector), CL_MEM_WRITE_ONLY});
			sdl_run(sdl);
		}
		loop = sdl_events(sdl, &cam);
	}
}

int			main(void)
{
	char		map[2][2] = {{0, 1}, {0, 0}};


	t_cl	cl;
	t_sdl	sdl;

	ft_bzero(&cl, sizeof(cl));
	ft_bzero(&sdl, sizeof(sdl));

	cl_init(&cl, 1, (t_arg){map, 4, CL_MEM_READ_ONLY});
	sdl_init(&sdl, PROGRAM_NAME);

	main_loop(&cl, &sdl);

	cl_end(&cl);
	sdl_end(&sdl);
	return (EXIT_SUCCESS);
}
