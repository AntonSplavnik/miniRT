/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reflection.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 16:20:54 by abillote          #+#    #+#             */
/*   Updated: 2025/05/23 17:08:09 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"




/**
 * Calculates the reflection of an incident ray around a normal vector
 *
 * @param incident The incoming ray direction (should be normalized)
 * @param normal The surface normal at the intersection point (should be normalized)
 * @return The direction of the reflected ray
 */
t_vec3 reflect_ray(t_vec3 incident, t_vec3 normal)
{
    // Formula: R = I - 2 * (I·N) * N
    // Where I is the incident ray direction, N is the normal, and R is the reflected ray direction

    // Calculate dot product between incident and normal
    double dot = vec3_dot(incident, normal);

    // Calculate reflection vector
    t_vec3 reflection;
    reflection.x = incident.x - 2.0 * dot * normal.x;
    reflection.y = incident.y - 2.0 * dot * normal.y;
    reflection.z = incident.z - 2.0 * dot * normal.z;

    // Normalize the reflection vector
    return vec3_normalize(reflection);
}
