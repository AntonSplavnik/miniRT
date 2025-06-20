/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone_normal.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 10:50:45 by abillote          #+#    #+#             */
/*   Updated: 2025/06/20 14:45:14 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/miniRT.h"

/**
 * Determines if a point lies on the cone's base or very close to the base edge
 *
 * This function performs two checks:
 * 1. If the point is on the base circular disc:
 *    - Measures distance from point to base plane
 *    - Measures radial distance from point to base center
 *    - If both are within thresholds, point is on the base
 * 2. If the point is at the top edge of the cone's body:
 *    - Calculates projection along cone's axis
 *    - If projection is very close to height, point is at the rim
 *
 * @param point       The point to check
 * @param cone        The cone object
 * @param base_center The pre-calculated center of the cone's base
 *
 * @return 1 if point is on base or rim, 0 otherwise
 */
static int	is_on_cone_base(t_vec3 point, t_cone cone, t_vec3 base_center)
{
	t_vec3	point_to_base;
	double	dist_to_base_plane;
	double	dist_to_base_center;
	double	projection;
	t_vec3	to_point;

	point_to_base = vec3_subtract(point, base_center);
	dist_to_base_plane = fabs(vec3_dot(point_to_base, cone.axis));
	dist_to_base_center = vec3_length(point_to_base);
	if (dist_to_base_plane < 0.00001 && dist_to_base_center \
				<= cone.radius * 1.01)
		return (1);
	to_point = vec3_subtract(point, cone.apex);
	projection = vec3_dot(to_point, cone.axis);
	if (projection >= cone.height - 0.005)
		return (1);
	return (0);
}

/**
 * Constrains a projection value to avoid numerical precision issues
 *
 * This function ensures that the projection value stays within safe
 * boundaries to avoid artifacts at the apex and base of the cone.
 * - Values too close to 0 are clamped to a minimum threshold
 * - Values too close to height are clamped to just below height
 *
 * @param projection The raw projection value along cone axis
 * @param height     The height of the cone
 *
 * @return The clamped projection value
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
 * Calculates the radial component for cone surface normal
 *
 * This function:
 * 1. Computes the vector from axis point to surface point
 * 2. Handles the degenerate case when point is too close to axis
 *    - Creates an arbitrary perpendicular vector using cross product
 * 3. Otherwise normalizes the radial vector
 *
 * @param point      The point on cone surface
 * @param axis_point The corresponding point on cone axis
 * @param cone       The cone object
 *
 * @return The normalized radial vector
 */
static t_vec3	calc_radial(t_vec3 point, t_vec3 axis_point, t_cone cone)
{
	t_vec3	radial;
	double	radial_length;

	radial = vec3_subtract(point, axis_point);
	radial_length = vec3_length(radial);
	if (radial_length < 0.0001)
	{
		if (fabs(cone.axis.x) < 0.9)
			return (vec3_normalize(vec3_cross(cone.axis, \
						vec3_create(1, 0, 0))));
		else
			return (vec3_normalize(vec3_cross(cone.axis, \
						vec3_create(0, 1, 0))));
	}
	return (vec3_scale(radial, 1.0 / radial_length));
}

/**
 * Computes the surface normal for a point on the cone body
 *
 * The normal calculation follows these steps:
 * 1. Clamp projection to avoid numerical issues
 * 2. Find the point on cone axis at the same height
 * 3. Calculate radial component (vector perpendicular to axis)
 * 4. Calculate cone slope (ratio of radius to height)
 * 5. Compute normal by subtracting scaled axis from radial vector
 *
 * @param point      The point on cone surface
 * @param cone       The cone object
 * @param projection The projection of point along cone axis
 *
 * @return The normalized surface normal vector
 */
static t_vec3	calc_surface_normal(t_vec3 point, \
					t_cone cone, double projection)
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
 * Main function to calculate normal vector at any point on a cone
 *
 * This function determines the appropriate normal vector based on
 * where the point lies on the cone:
 * - If point is on base or at rim: returns the cone axis
 * - Otherwise calculates surface normal for cone body
 *
 * The function handles all edge cases and ensures returned normals
 * are always properly normalized.
 *
 * @param point The intersection point on cone surface
 * @param cone  The cone object
 *
 * @return The normal vector at the given point, always normalized
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
