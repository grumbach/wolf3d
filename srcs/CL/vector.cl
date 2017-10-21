/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: agrumbac <agrumbac@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/07/10 15:43:38 by agrumbac          #+#    #+#             */
/*   Updated: 2017/07/10 15:43:40 by agrumbac         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "vector.h.cl"

t_vector			unit_vector(const t_vector v)
{
	return (v / length(v));
}

t_vector			vector(const t_vector a, const t_vector b)
{
	return (b - a);
}
