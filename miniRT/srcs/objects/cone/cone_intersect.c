/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone_intersect.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 13:51:15 by abillote          #+#    #+#             */
/*   Updated: 2025/06/20 11:04:11 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/miniRT.h"

/**
 * Validates intersection when ray is nearly parallel to cone surface
 *
 * This function checks if a linear intersection (when quadratic term
 * is near zero) produces a valid hit point on the cone surface,
 * within height constraints.
 *
 * @param ray: The ray being tested
 * @param cone: The cone object to test against
 * @param t_candidate: The potential intersection distance to validate
 * @param t_body: Output parameter to store valid intersection distance
 *
 * @return: 1 if intersection is valid, 0 otherwise
 *
 * @details: Nearly identical to check_t_candidate but specifically
 * for the linear case.
 * Validates that the intersection point is within the cone's height limits.
 */
static int	check_linear_hit(t_ray ray, t_cone cone, \
				double t_candidate, double *t_body)
{
	t_vec3	hit;
	t_vec3	hit_to_apex;
	double	projection;

	hit = vec3_add(ray.origin, vec3_scale(ray.direction, t_candidate));
	hit_to_apex = vec3_subtract(hit, cone.apex);
	projection = vec3_dot(hit_to_apex, cone.axis);
	if (projection > 0.001 && projection <= cone.height)
	{
		*t_body = t_candidate;
		return (1);
	}
	*t_body = INFINITY;
	return (0);
}

/**
 * Manages ray intersection when ray is nearly parallel to cone surface
 *
 * This function handles the special case where the quadratic coefficient (a)
 * is approximately zero, meaning the ray might be nearly
 * parallel to the cone surface.
 *
 * @param ray: The ray being tested
 * @param cone: The cone object to test against
 * @param coeffs: The quadratic equation coefficients [a, b, c]
 * @param t_body: Output parameter to store valid intersection distance
 *
 * @return: 1 if a valid intersection was found, 0 otherwise
 *
 * @details: When a ≈ 0, the equation becomes linear (bT + c = 0).
 * The function checks:
 *	1. If b is also near zero (ray parallel to axis), no intersection exists
 *	2. Otherwise, calculates linear solution T = -c/b
 *	3. Validates that the solution produces a valid intersection point
 */
static int	handle_linear_case(t_ray ray, t_cone cone, \
				double *coeffs, double *t_body)
{
	double	b;
	double	c;
	double	t_candidate;

	b = coeffs[1];
	c = coeffs[2];
	if (fabs(b) < 0.0001)
	{
		*t_body = INFINITY;
		return (0);
	}
	t_candidate = -c / b;
	if (t_candidate <= 0.001)
	{
		*t_body = INFINITY;
		return (0);
	}
	return (check_linear_hit(ray, cone, t_candidate, t_body));
}

/**
 * Determines closest valid intersection among body and base hits
 *
 * This function selects the closest valid intersection point between
 * the cone body and cone base.
 *
 * @param t_body: Distance to intersection with cone body (or INFINITY if none)
 * @param t_base: Distance to intersection with cone base (or INFINITY if none)
 * @param t: Output parameter to store the closest valid intersection distance
 *
 * @return: 1 if a valid intersection was found, 0 otherwise
 *
 * @details: The function:
 *	1. Compares both intersection distances
 *	2. Selects the smallest positive distance (greater than the min threshold)
 *	3. Returns 0 if neither intersection is valid
 */
static int	find_closest_hit(double t_body, double t_base, double *t)
{
	double	closest_t;

	closest_t = INFINITY;
	if (t_body > 0.001 && t_body < closest_t)
		closest_t = t_body;
	if (t_base > 0.001 && t_base < closest_t)
		closest_t = t_base;
	if (closest_t < INFINITY)
	{
		*t = closest_t;
		return (1);
	}
	return (0);
}

/**
 * Main entry point for ray-cone intersection test
 *
 * This function tests if a ray intersects with a cone and finds the
 * closest intersection point.
 *
 * @param ray: The ray to test for intersection
 * @param cone: The cone object to test against
 * @param t: Output parameter to store the closest intersection distance
 *
 * @return: 1 if a valid intersection was found, 0 otherwise
 *
 * @details: The function:
 *	1. Calculates the cone base center position
 *	2. Computes the vector from apex to ray origin
 *	3. Calculates quadratic equation coefficients
 *	4. Handles different solution methods based on coefficient values
 *	5. Tests intersection with cone body and cone base
 *	6. Returns the closest valid intersection
 */
int	ray_cone_intersect(t_ray ray, t_cone cone, double *t)
{
	t_vec3	base_center;
	t_vec3	apex_to_origin;
	double	coeffs[3];
	double	t_body;
	double	t_base;

	t_body = INFINITY;
	base_center = vec3_add(cone.apex, vec3_scale(cone.axis, cone.height));
	apex_to_origin = vec3_subtract(ray.origin, cone.apex);
	calculate_cone_equation_coeffs(ray, cone, apex_to_origin, coeffs);
	if (fabs(coeffs[0]) < 0.0001)
		handle_linear_case(ray, cone, coeffs, &t_body);
	else
		solve_cone_quadratic(ray, cone, coeffs, &t_body);
	t_base = ray_disc_intersect(ray, base_center, cone.axis, cone.radius);
	return (find_closest_hit(t_body, t_base, t));
}
