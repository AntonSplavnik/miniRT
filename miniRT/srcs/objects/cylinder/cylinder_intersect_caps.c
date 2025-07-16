/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder_intersect_caps.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 15:00:00 by abillote          #+#    #+#             */
/*   Updated: 2025/06/19 11:36:36 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/miniRT.h"

/*
 * Calculates ray intersection with a disc (cylinder cap)
 *
 * This function determines if a ray intersects a disc in
 * 3D space and where.
 * The disc is defined by its center point, normal vector, and radius.
 *
 * @param ray     The ray to test for intersection
 * @param center  Center point of the disc
 * @param normal  Normal vector of the disc (must be normalized)
 * @param radius  Radius of the disc
 *
 * @return        Distance along ray to intersection point,
 * or 0.0 if no intersection
 */
double	ray_disc_intersect(t_ray ray, t_vec3 center, t_vec3 normal,
							double radius)
{
	double	t;
	double	denom;
	double	t_plane;
	t_vec3	point;
	double	distance_squared;

	t = 0.0;
	denom = vec3_dot(ray.direction, normal);
	if (fabs(denom) < 0.0001)
		return (0.0);
	t_plane = vec3_dot(vec3_subtract(center, ray.origin), normal) / denom;
	if (t_plane <= 0.0001)
		return (0.0);
	point = vec3_add(ray.origin, vec3_scale(ray.direction, t_plane));
	distance_squared = vec3_length_squared(vec3_subtract(point, center));
	if (distance_squared <= radius * radius)
		t = t_plane;
	return (t);
}

/*
 * Calculates ray intersections with both caps of a cylinder
 *
 * Determines the intersection points of a ray with both the top and bottom
 * caps of a cylinder. The caps are represented as discs.
 *
 * @param ray       The ray to test for intersection
 * @param cylinder  The cylinder to test against
 * @param t_top     Pointer to store distance to top cap intersection
 * @param t_bottom  Pointer to store distance to bottom cap intersection
 */
void	check_cylinder_caps_intersection(t_ray ray, \
					t_cylinder cylinder, double *t_top, double *t_bottom)
{
	t_vec3	top_center;
	t_vec3	bottom_center;

	top_center = vec3_add(cylinder.center, \
					vec3_scale(cylinder.axis, cylinder.height / 2.0));
	bottom_center = vec3_subtract(cylinder.center, \
					vec3_scale(cylinder.axis, cylinder.height / 2.0));
	*t_top = ray_disc_intersect(ray, top_center, \
					cylinder.axis, cylinder.radius);
	*t_bottom = ray_disc_intersect(ray, bottom_center, \
					vec3_negate(cylinder.axis), cylinder.radius);
}
