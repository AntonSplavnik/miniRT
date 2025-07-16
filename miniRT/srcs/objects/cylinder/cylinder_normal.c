/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder_normal.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 15:00:00 by abillote          #+#    #+#             */
/*   Updated: 2025/06/19 11:35:40 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/miniRT.h"

/*
 * Checks if the point is on one of the cylinder caps
 * and returns the appropriate normal
 *
 * @param point         The intersection point to check
 * @param top_center    Center point of the top cap
 * @param bottom_center Center point of the bottom cap
 * @param cylinder      The cylinder being checked
 *
 * @return              The cap normal if point is on a cap,
 * NULL_VEC3 otherwise
 */
static t_vec3	check_cylinder_cap_normal(t_vec3 point, t_vec3 top_center, \
							t_vec3 bottom_center, t_cylinder cylinder)
{
	double	dist_from_top_center;
	double	dist_from_bottom_center;
	t_vec3	to_point;
	double	projection;

	to_point = vec3_subtract(point, cylinder.center);
	projection = vec3_dot(to_point, cylinder.axis);
	if (projection > (cylinder.height / 2.0 - 0.0001))
	{
		dist_from_top_center = vec3_length(vec3_subtract(point, top_center));
		if (dist_from_top_center <= cylinder.radius + 0.0001)
			return (cylinder.axis);
	}
	if (projection < -(cylinder.height / 2.0 - 0.0001))
	{
		dist_from_bottom_center = vec3_length(vec3_subtract(point, \
						bottom_center));
		if (dist_from_bottom_center <= cylinder.radius + 0.0001)
			return (vec3_negate(cylinder.axis));
	}
	return ((t_vec3){0, 0, 0});
}

/*
 * Calculates the surface normal vector at a point on a cylinder
 *
 * Determines the correct normal vector for a point on a
 * cylinder's surface, handling different cases for points
 * on the curved body, top cap, or bottom cap.
 *
 * @param point     The point on the cylinder surface
 * @param cylinder  The cylinder object
 *
 * @return          Normalized vector perpendicular to the
 * surface at the point
 */
t_vec3	cylinder_normal_at_point(t_vec3 point, t_cylinder cylinder)
{
	t_vec3	top_center;
	t_vec3	bottom_center;
	t_vec3	cap_normal;
	t_vec3	to_point;
	double	projection;

	top_center = vec3_add(cylinder.center, \
						vec3_scale(cylinder.axis, cylinder.height / 2.0));
	bottom_center = vec3_subtract(cylinder.center, vec3_scale(cylinder.axis, \
									cylinder.height / 2.0));
	cap_normal = check_cylinder_cap_normal(point, top_center, \
										bottom_center, cylinder);
	if (!(cap_normal.x == 0 && cap_normal.y == 0 && cap_normal.z == 0))
		return (cap_normal);
	to_point = vec3_subtract(point, cylinder.center);
	projection = vec3_dot(to_point, cylinder.axis);
	return (vec3_normalize(vec3_subtract(point, vec3_add(cylinder.center, \
									vec3_scale(cylinder.axis, projection)))));
}
