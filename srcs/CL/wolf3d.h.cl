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
# include "hit.h.cl"

/*
** ********************************** camera ***********************************
*/

typedef struct			s_cam
{
	t_vector			origin;
	t_vector			direction;
	float				fov;
	float				aspect;
}						t_cam;

/*
** ********************************** misc *************************************
*/

# define GLOBAL_X	1
# define GLOBAL_Y	0

typedef struct			s_yx
{
	int					y;
	int					x;
}						t_yx;

#endif
