/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reflection.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 20:05:13 by antonsplavn       #+#    #+#             */
/*   Updated: 2025/07/20 20:07:43 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

/**
 * Calculates the reflection of an incident ray around a normal vector
 *
 * @param incident The incoming ray direction (should be normalized)
 * @param normal The surface normal at the intersection point
 * 														(should be normalized)
 * @return The direction of the reflected ray
 */
t_vec3	reflect_ray(t_vec3 incident, t_vec3 normal)
{
	double	dot;
	t_vec3	reflection;

	dot = vec3_dot(incident, normal);
	reflection.x = incident.x - 2.0 * dot * normal.x;
	reflection.y = incident.y - 2.0 * dot * normal.y;
	reflection.z = incident.z - 2.0 * dot * normal.z;
	return (vec3_normalize(reflection));
}
