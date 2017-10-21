/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wolf3d.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/18 23:52:41 by agrumbac          #+#    #+#             */
/*   Updated: 2017/10/21 20:05:13 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "wolf3d.h"

/*
** cl_run has 4 args
**     - [r--] the map
**     - [r--] the camera
**     - [-w-] wall_heights (written in the first line of pixeltab, for memory efficiency)
**     - [-w-] wall_colors (written in the middle of pixeltab, as they won't need to change color...)
*/

static void	put_pixel(t_sdl *sdl, const t_yx i, uint color)
{
	sdl->pixels[sdl->size.x * i.y + i.x] = color;// TODO change this to sexy pointer
}

/*
** -----------------------------------------  win size -> UP
**       -----------------------------        wallheight
** ++++++                             ++++++  diff
** ======                                     GROUND
**       =============================        WALL
** 	                                  ======  SKYBOX
*/

static void	map_redraw(t_sdl *sdl)
{
	t_yx		i;
	uint		wall_color;
	int			height;
	int			step;

	i = (t_yx){sdl->size.y, sdl->size.x};
	while (i.x--)
	{
		i.y = sdl->size.y;
		wall_color = *(sdl->pixels + (((sdl->size.y % 2 ? \
			sdl->size.y - 1 : sdl->size.y) / 2) * sdl->size.x));
		height = sdl->pixels[i.x];
		if (height > sdl->size.y) {ft_printf("OUT OF BOUNDS height TOO HIGH!!"); break;}//

		step = sdl->size.y - ((sdl->size.y - height) / 2);
		while (--i.y > step)
			put_pixel(sdl, i, GROUND);
		step = sdl->size.y - height - ((sdl->size.y - height) / 2);
		i.y++;
		while (--i.y > step)
			put_pixel(sdl, i, wall_color);
		i.y++;
		while (--i.y >= 0)
			put_pixel(sdl, i, SKYBOX);
	}
}

static void	main_loop(t_cl *cl, t_sdl *sdl)
{
	t_cam		cam;
	int			loop;

	cam = (t_cam){(t_vector){22, 12}, (t_vector){-1, 0}, (t_vector){0, 0.66}};
	loop = EVENT_UPDATE;
	while (loop)
	{
		if (loop == EVENT_UPDATE)
		{
			cl_run(cl, (size_t[WORK_DIM]){sdl->size.x}, 3, \
				(t_arg){&cam, sizeof(t_cam), CL_MEM_READ_ONLY}, \
				(t_arg){sdl->pixels, sizeof(uint) * sdl->size.x, CL_MEM_WRITE_ONLY}, \
				(t_arg){(sdl->pixels + (((sdl->size.y % 2 ? \
					sdl->size.y - 1 : sdl->size.y) / 2) * sdl->size.x)), \
					sizeof(uint) * sdl->size.x, CL_MEM_WRITE_ONLY});
			map_redraw(sdl);
			sdl_run(sdl);
		}
		loop = sdl_events(sdl, &cam);
	}
}

int			main(void)
{
	char	map[MAP_H][MAP_W] =
	{
		{1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,1,1,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
		{1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
		{1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
	};

	t_cl	cl;
	t_sdl	sdl;

	ft_bzero(&cl, sizeof(cl));
	ft_bzero(&sdl, sizeof(sdl));

	cl_init(&cl, 1, (t_arg){map, sizeof(map), CL_MEM_READ_ONLY});
	sdl_init(&sdl, PROGRAM_NAME);

	main_loop(&cl, &sdl);

	cl_end(&cl);
	sdl_end(&sdl);
	return (EXIT_SUCCESS);
}
