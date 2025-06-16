/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3_basic_op.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/28 17:34:18 by abillote          #+#    #+#             */
/*   Updated: 2025/04/28 17:45:58 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

// Create a new vector with given components
t_vec3	vec3_create(double x, double y, double z)
{
	t_vec3	result;

	result.x = x;
	result.y = y;
	result.z = z;
	return (result);
}

// Negate a vector
t_vec3	vec3_negate(t_vec3 v)
{
	return (vec3_create(-v.x, -v.y, -v.z));
}

// Add two vectors
t_vec3	vec3_add(t_vec3 u, t_vec3 v)
{
	return (vec3_create(u.x + v.x, u.y + v.y, u.z + v.z));
}

// Subtract vector v from vector u
t_vec3	vec3_subtract(t_vec3 u, t_vec3 v)
{
	return (vec3_create(u.x - v.x, u.y - v.y, u.z - v.z));
}

// Multiply two vectors component-wise
t_vec3	vec3_multiply(t_vec3 u, t_vec3 v)
{
	return (vec3_create(u.x * v.x, u.y * v.y, u.z * v.z));
}
