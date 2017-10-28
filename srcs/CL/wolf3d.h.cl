/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rt.h.cl                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/18 23:53:40 by agrumbac          #+#    #+#             */
/*   Updated: 2017/06/27 22:00:32 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RT_H_CL
# define RT_H_CL

/*
** ********************************** rt ***************************************
*/

# include "vector.h.cl"

/*
** ********************************** camera ***********************************
*/

typedef struct			s_cam
{
	t_vector			origin;
	t_vector			direction;
	t_vector			plane;
	uint				screen_height;
}						t_cam;

/*
** ********************************** misc *************************************
*/

# define GLOBAL_X			1
# define GLOBAL_Y			0

# define MAP_SIZE			24

# define NOCOLOR			0xff000000
# define WALLMIDNIGHTBLUE	0xff191970
# define WALLSKYBLUE		0xff51bdff
# define WALLPINK			0xffa90e64
# define WALLCRIMSONRED		0xffb00718
# define SCREEN_HEIGHT		640

typedef struct			s_xy
{
	int					x;
	int					y;
}						t_xy;

#endif
