/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wolf3d.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/18 23:53:40 by agrumbac          #+#    #+#             */
/*   Updated: 2017/10/22 18:32:27 by agrumbac         ###   ########.fr       */
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

# define WIN_H				640
# define WIN_W				1024
# define MAP_H				24
# define MAP_W				24

# define SKYBOX				0xff333333
# define GROUND				0xff666666

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

# define MOVE_SPEED			0.1
# define TURN_SPEED			0.1

typedef struct			s_vector
{
	float				x;
	float				y;
}						t_vector;

typedef struct			s_cam
{
	t_vector			origin;
	t_vector			direction;
	uint				fov;
	uint				screen_height;
}						t_cam;

int						sdl_events(t_sdl *sdl, t_cam *cam);

/*
** *****************************************************************************
*/

long					errors(const int err, const char *comment);

#endif
