/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   triangle_intersect.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 14:30:00 by abillote          #+#    #+#             */
/*   Updated: 2025/05/10 14:45:00 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "platform.h"

// Calculate the normal of a triangle (counter-clockwise winding)
t_vec3	triangle_normal(t_triangle triangle)
{
	t_vec3	edge1;
	t_vec3	edge2;
	t_vec3	normal;

	// Calculate two edges of the triangle
	edge1 = vec3_subtract(triangle.v1, triangle.v0);
	edge2 = vec3_subtract(triangle.v2, triangle.v0);

	// Calculate the cross product to get the normal
	normal = vec3_cross(edge1, edge2);

	// Normalize and return
	return (vec3_normalize(normal));
}

// Ray-triangle intersection using Möller–Trumbore algorithm
int	ray_triangle_intersect(t_ray ray, t_triangle triangle, double *t)
{
	t_vec3	edge1, edge2;
	t_vec3	h, s, q;
	double	a, f, u, v;

	// Calculate two edges of the triangle
	edge1 = vec3_subtract(triangle.v1, triangle.v0);
	edge2 = vec3_subtract(triangle.v2, triangle.v0);

	// Begin calculating determinant (cross product of ray direction and one edge)
	h = vec3_cross(ray.direction, edge2);
	
	// Calculate determinant
	a = vec3_dot(edge1, h);

	// If determinant is near zero, ray lies in plane of triangle or ray is parallel to plane
	if (fabs(a) < 0.00001)
		return (0);

	// Calculate inverse determinant
	f = 1.0 / a;
	
	// Calculate vector from v0 to ray origin
	s = vec3_subtract(ray.origin, triangle.v0);
	
	// Calculate u parameter and test bounds
	u = f * vec3_dot(s, h);
	
	// Check if u is within bounds [0,1]
	if (u < 0.0 || u > 1.0)
		return (0);

	// Calculate q vector
	q = vec3_cross(s, edge1);
	
	// Calculate v parameter and test bounds
	v = f * vec3_dot(ray.direction, q);
	
	// Check if v is within bounds [0,1] and u+v <= 1
	if (v < 0.0 || u + v > 1.0)
		return (0);

	// Calculate t parameter (distance along ray)
	*t = f * vec3_dot(edge2, q);

	// Check if intersection point is in front of ray origin
	if (*t > 0.001)
		return (1);
	return (0);
} 