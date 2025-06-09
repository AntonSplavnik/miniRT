/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3_length.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 17:40:12 by abillote          #+#    #+#             */
/*   Updated: 2025/05/16 15:32:23 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

// Calculate squared length of a vector
double	vec3_length_squared(t_vec3 v)
{
	return (v.x * v.x + v.y * v.y + v.z * v.z);
}

// Calculate length of a vector
double	vec3_length(t_vec3 v)
{
	return (sqrt(vec3_length_squared(v)));
}

// Normalize a vector (create a unit vector)
t_vec3	vec3_normalize(t_vec3 v)
{
	double	len;

	len = vec3_length(v);
	if (len > 0)
		return (vec3_divide(v, len));
	return (v);
}
