/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wolf3d.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/18 23:53:40 by agrumbac          #+#    #+#             */
/*   Updated: 2018/01/30 15:59:36 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WOLF3D_H
# define WOLF3D_H

/*
** ********************************** Global ***********************************
*/

# define PROGRAM_NAME		"wolf3d"

# include <math.h>
# include <fcntl.h>
# include <errno.h>

/*
** ********************************** Local ************************************
*/

# include "libft.h"
# include "easy_cl.h"
# include "easy_sdl.h"
# include "wolf3d_c_cl.h"

# define WIN_H				640
# define WIN_W				1024

# define WALL0				"textures/sci.jpg"
# define WALL1				"textures/wall8.jpg"
# define WALL2				"textures/doom.png"
# define WALL3				"textures/door.jpg"

/*
** ********************************** errors ***********************************
*/

# define ERR_SYS			0
# define ERR_CL				1
# define ERR_SDL			2
# define ERR_USAGE			3

/*
** ********************************** camera ***********************************
** int		sdl_events(t_sdl *sdl, );
** 					fetch sdl events returns a status from :
** 					EVENT_STOP, EVENT_IDLE, EVENT_UPDATE
** *****************************************************************************
*/

# define EVENT_STOP			0
# define EVENT_IDLE			1
# define EVENT_UPDATE		2

# define MOVE_SPEED			0.2
# define TURN_SPEED			0.1
# define MINIMAP_RADIUS		18

typedef struct			s_vector
{
	float				x;
	float				y;
}						t_vector;

typedef struct			s_cam
{
	t_vector			origin;
	t_vector			direction;
	t_vector			plane;
	uint				screen_height;
	uint				padding;
}						t_cam;

void					sdl_run(t_sdl *sdl, const char map[MAP_SIZE][MAP_SIZE],
							t_cam *cam);
int						sdl_events(const char map[MAP_SIZE][MAP_SIZE], \
							t_sdl *sdl, t_cam *cam);

/*
** *****************************************************************************
*/

long					errors(const int err, const char *comment);
void					parse_map(const char *filename, void *map);
void					display_minimap(t_sdl *sdl, \
							const char map[MAP_SIZE][MAP_SIZE], t_vector pos);

#endif
