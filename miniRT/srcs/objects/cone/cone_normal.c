/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone_normal.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 10:50:45 by abillote          #+#    #+#             */
/*   Updated: 2025/06/20 11:18:31 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/miniRT.h"


/**
 * Check if point is on the cone base
 */
static int	is_on_cone_base(t_vec3 point, t_cone cone, t_vec3 base_center)
{
	t_vec3	point_to_base;
	double	dist_to_base_plane;
	double	dist_to_base_center;

	point_to_base = vec3_subtract(point, base_center);
	dist_to_base_plane = fabs(vec3_dot(point_to_base, cone.axis));
	dist_to_base_center = vec3_length(point_to_base);
	if (dist_to_base_plane < 0.01 && dist_to_base_center <= cone.radius * 1.01)
		return (1);
	point_to_base = vec3_subtract(point, cone.apex);
	if (calc_projection(point, cone, &point_to_base) >= cone.height - 0.005)
		return (1);
	return (0);
}

/**
 * Clamp projection value to valid range
 */
static double	clamp_projection(double projection, double height)
{
	double	result;

	result = projection;
	if (result <= 0.001)
		result = 0.001;
	if (result >= height)
		result = height - 0.001;
	return (result);
}

/**
 * Create perpendicular vector for degenerate cases
 */
static t_vec3	create_perpendicular(t_vec3 axis)
{
	t_vec3	perp;

	if (fabs(axis.x) < 0.9)
		perp = vec3_cross(axis, vec3_create(1, 0, 0));
	else
		perp = vec3_cross(axis, vec3_create(0, 1, 0));
	return (vec3_normalize(perp));
}

/**
 * Calculate radial component for surface normal
 */
static t_vec3	calc_radial(t_vec3 point, t_vec3 axis_point, t_cone cone)
{
	t_vec3	radial;
	double	radial_length;

	radial = vec3_subtract(point, axis_point);
	radial_length = vec3_length(radial);
	if (radial_length < 0.0001)
		return (create_perpendicular(cone.axis));
	return (vec3_scale(radial, 1.0 / radial_length));
}

/**
 * Calculate surface normal for cone body
 */
static t_vec3	calc_surface_normal(t_vec3 point, t_cone cone, double projection)
{
	t_vec3	axis_point;
	t_vec3	radial;
	t_vec3	normal;
	double	cone_slope;

	projection = clamp_projection(projection, cone.height);
	axis_point = vec3_add(cone.apex, vec3_scale(cone.axis, projection));
	radial = calc_radial(point, axis_point, cone);
	cone_slope = cone.radius / cone.height;
	normal = vec3_subtract(radial, vec3_scale(cone.axis, cone_slope));
	return (vec3_normalize(normal));
}

/**
 * Calculate normal vector at a point on a cone
 */
t_vec3	cone_normal_at_point(t_vec3 point, t_cone cone)
{
	t_vec3	base_center;
	t_vec3	to_point;
	double	projection;

	base_center = vec3_add(cone.apex, vec3_scale(cone.axis, cone.height));
	to_point = vec3_subtract(point, cone.apex);
	projection = vec3_dot(to_point, cone.axis);
	if (is_on_cone_base(point, cone, base_center))
		return (vec3_normalize(cone.axis));
	return (calc_surface_normal(point, cone, projection));
}
