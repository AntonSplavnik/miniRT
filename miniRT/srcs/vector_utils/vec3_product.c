/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3_product.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 17:37:04 by abillote          #+#    #+#             */
/*   Updated: 2025/04/28 17:46:34 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

// Multiply a vector by a scalar
t_vec3	vec3_scale(t_vec3 v, double t)
{
	return (vec3_create(v.x * t, v.y * t, v.z * t));
}

// Divide a vector by a scalar
t_vec3	vec3_divide(t_vec3 v, double t)
{
	return (vec3_scale(v, 1.0 / t));
}

// Calculate the dot product of two vectors
double	vec3_dot(t_vec3 u, t_vec3 v)
{
	return (u.x * v.x + u.y * v.y + u.z * v.z);
}

// Calculate the cross product of two vectors
t_vec3	vec3_cross(t_vec3 u, t_vec3 v)
{
	return (vec3_create(
			u.y * v.z - u.z * v.y,
			u.z * v.x - u.x * v.z,
			u.x * v.y - u.y * v.x
		));
}
