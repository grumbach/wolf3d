/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wolf3d.h.cl                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/06/18 23:53:40 by agrumbac          #+#    #+#             */
/*   Updated: 2017/06/27 21:51:41 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef WOLF3D_H_CL
# define WOLF3D_H_CL

/*
** ********************************** Local ************************************
*/

# include "../../includes/wolf3d_c_cl.h"

/*
** ********************************** typedefs *********************************
*/

typedef float2			t_vector;

typedef struct			s_xy
{
	int					x;
	int					y;
}						t_xy;

typedef struct			s_cam
{
	t_vector			origin;
	t_vector			direction;
	t_vector			plane;
	uint				screen_height;
}						t_cam;

#endif
