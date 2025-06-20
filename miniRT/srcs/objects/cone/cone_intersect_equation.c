/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone_intersect_equation.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 10:47:49 by abillote          #+#    #+#             */
/*   Updated: 2025/06/20 11:04:31 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/miniRT.h"

/**
 * Validates if a point lies on the cone's surface
 *
 * This function determines whether a hit point is actually on the cone's
 * surface by calculating the expected radius at the specific height and
 * comparing it to the actual distance from the axis.
 *
 * @param hit: The potential intersection point to check
 * @param cone: The cone object being tested against
 * @param projection: The height along the cone axis where intersection occurs
 *
 * @return: 1 if point is on the cone surface, 0 otherwise
 *
 * @details: Uses a small epsilon (1.001 multiplier) to account for
 * floating-point precision issues when determining if the point
 * is on the surface.
 */
int	is_on_cone_surface(t_vec3 hit, t_cone cone, double projection)
{
	double	radius_at_height;
	t_vec3	point_on_axis;
	double	dist_from_axis;

	radius_at_height = (projection / cone.height) * cone.radius;
	point_on_axis = vec3_add(cone.apex, vec3_scale(cone.axis, projection));
	dist_from_axis = vec3_length(vec3_subtract(hit, point_on_axis));
	return (dist_from_axis <= radius_at_height * 1.001);
}

/**
 * Computes quadratic equation coefficients for cone-ray intersection
 *
 * This function calculates the coefficients (a, b, c) for the quadratic equation
 * that represents the intersection of a ray with a cone. These coefficients
 * are used to find the intersection points by solving the quadratic equation.
 *
 * @param ray: The ray being tested for intersection
 * @param cone: The cone object to test against
 * @param apex_to_origin: Vector from cone apex to ray origin (precomputed)
 * @param coeffs: Output array to store the three coefficients [a, b, c]
 *
 * @details: The quadratic equation is derived from the geometric definition
 * of a cone and the parametric equation of a ray. The cone is defined by its
 * apex, axis, and half-angle (stored as cos^2 for efficiency).
 */
void	calculate_cone_equation_coeffs(t_ray ray, t_cone cone, \
									t_vec3 apex_to_origin, double *coeffs)
{
	double	cos_squared;
	double	dot_dir_axis;
	double	dot_origin_axis;

	cos_squared = cos(cone.angle) * cos(cone.angle);
	dot_dir_axis = vec3_dot(ray.direction, cone.axis);
	dot_origin_axis = vec3_dot(apex_to_origin, cone.axis);
	coeffs[0] = dot_dir_axis * dot_dir_axis - \
			cos_squared * vec3_dot(ray.direction, ray.direction);
	coeffs[1] = 2.0 * (dot_origin_axis * dot_dir_axis - \
			cos_squared * vec3_dot(apex_to_origin, ray.direction));
	coeffs[2] = dot_origin_axis * dot_origin_axis - \
			cos_squared * vec3_dot(apex_to_origin, apex_to_origin);
}

/**
 * Validates a potential intersection distance with the cone body
 *
 * This function checks if a calculated intersection distance actually
 * produces a valid hit point on the cone surface, within the height limits.
 *
 * @param ray: The ray being tested
 * @param cone: The cone object to test against
 * @param t_candidate: The potential intersection distance to validate
 * @param t_body: Output parameter to store valid intersection distance
 *
 * @return: 1 if intersection is valid, 0 otherwise
 *
 * @details: For a valid intersection, the point must:
 *	1. Be in front of the ray (t > 0.001)
 *	2. Be within the height constraints of the cone (0 < projection <= height)
 * 	3. Be on the cone surface (validated by is_on_cone_surface)
 */
int	check_t_candidate(t_ray ray, t_cone cone, \
						double t_candidate, double *t_body)
{
	t_vec3	hit;
	t_vec3	hit_to_apex;
	double	projection;

	if (t_candidate <= 0.001)
		return (0);
	hit = vec3_add(ray.origin, vec3_scale(ray.direction, t_candidate));
	hit_to_apex = vec3_subtract(hit, cone.apex);
	projection = vec3_dot(hit_to_apex, cone.axis);
	if (projection > 0.001 && projection <= cone.height)
	{
		if (is_on_cone_surface(hit, cone, projection))
		{
			*t_body = t_candidate;
			return (1);
		}
	}
	return (0);
}

/**
 * Tests both potential intersections from quadratic equation
 *
 * This function examines both solutions from the quadratic equation to find
 * the closest valid intersection with the cone body.
 *
 * @param ray: The ray being tested
 * @param cone: The cone object to test against
 * @param t_sols: Array containing both solutions [t1, t2]
 * @param t_body: Output parameter to store closest valid intersection distance
 *
 * @return: 1 if at least one valid intersection was found, 0 otherwise
 *
 * @details: The function checks the smaller solution first, then the larger one.
 * When the second solution is valid and closer, it replaces the first.
 * When no valid intersection is found, t_body is set to INFINITY.
 */
int	check_quadratic_solutions(t_ray ray, t_cone cone, \
								double *t_sols, double *t_body)
{
	int		hit_found;
	double	temp;

	hit_found = 0;
	*t_body = INFINITY;
	if (check_t_candidate(ray, cone, t_sols[0], t_body))
		hit_found = 1;
	if (!hit_found || t_sols[1] < *t_body)
	{
		temp = *t_body;
		if (check_t_candidate(ray, cone, t_sols[1], t_body))
			hit_found = 1;
		else if (hit_found)
			*t_body = temp;
	}
	if (!hit_found)
		*t_body = INFINITY;
	return (hit_found);
}

/**
 * Solves the quadratic equation for cone-ray intersection
 *
 * This function solves the quadratic equation defined by the coefficients
 * to find the intersection points between the ray and cone body.
 *
 * @param ray: The ray being tested
 * @param cone: The cone object to test against
 * @param coeffs: The quadratic equation coefficients [a, b, c]
 * @param t_body: Output parameter to store closest valid intersection distance
 *
 * @return: 1 if a valid intersection was found, 0 otherwise
 *
 * @details: The function:
 *	1. Calculates the discriminant to determine if intersections exist
 *	2. If discriminant is negative, no intersections exist (ray misses cone)
 *	3. Otherwise, computes both intersection candidates and validates them
 */
int	solve_cone_quadratic(t_ray ray, t_cone cone, double *coeffs, double *t_body)
{
	double	discriminant;
	double	sqrt_discriminant;
	double	t_sols[2];

	discriminant = coeffs[1] * coeffs[1] - 4 * coeffs[0] * coeffs[2];
	if (discriminant < 0)
	{
		*t_body = INFINITY;
		return (0);
	}
	sqrt_discriminant = sqrt(discriminant);
	t_sols[0] = (-coeffs[1] - sqrt_discriminant) / (2.0 * coeffs[0]);
	t_sols[1] = (-coeffs[1] + sqrt_discriminant) / (2.0 * coeffs[0]);
	return (check_quadratic_solutions(ray, cone, t_sols, t_body));
}
