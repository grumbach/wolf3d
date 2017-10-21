/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/10 15:47:51 by agrumbac          #+#    #+#             */
/*   Updated: 2017/07/10 15:47:55 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VECTOR_H_CL
# define VECTOR_H_CL

typedef float2		t_vector;

t_vector			unit_vector(const t_vector v);
t_vector			vector(const t_vector a, const t_vector b);

#endif
