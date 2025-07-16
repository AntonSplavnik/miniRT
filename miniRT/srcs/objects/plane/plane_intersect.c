/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane_intersect.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 10:25:20 by abillote          #+#    #+#             */
/*   Updated: 2025/06/19 11:45:14 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

/*
 * Calculates the intersection of a ray with an infinite plane
 *
 * This function determines if and where a ray intersects a plane in 3D space.
 * The calculation is based on the parametric equation of
 * a ray and the implicit equation of a plane.
 *
 * Special cases handled:
 * 1. When ray is parallel to the plane (dot product near zero):
 *    a. If ray origin is on/very near the plane, returns a small positive value
 *    b. Otherwise, returns no intersection
 * 2. When intersection point is behind the ray origin (negative t),
 * returns no intersection
 *
 * A small offset (0.0001) is subtracted from intersection distance to prevent
 * self-shadowing and numerical precision issues in subsequent computations.
 *
 * @param ray    The ray being tested for intersection
 * @param plane  The plane to test against
 * @param t      Pointer to store the intersection distance if found
 *
 * @return       1 if intersection occurred, 0 otherwise
 */

int	ray_plane_intersect(t_ray ray, t_plane plane, double *t)
{
	double	denom;
	double	t_plane;
	double	distance_to_plane;

	denom = vec3_dot(ray.direction, plane.normal);
	if (fabs(denom) < 0.0001)
	{
		distance_to_plane = vec3_dot(vec3_subtract(plane.point, ray.origin), \
							plane.normal);
		if (fabs(distance_to_plane) < 0.001)
		{
			*t = 0.001;
			return (1);
		}
		return (0);
	}
	t_plane = vec3_dot(vec3_subtract(plane.point, ray.origin), \
				plane.normal) / denom;
	if (t_plane < 0.00001)
		return (0);
	*t = t_plane - 0.0001;
	return (1);
}
