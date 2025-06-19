/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone_intersect.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 13:51:15 by abillote          #+#    #+#             */
/*   Updated: 2025/06/19 13:51:19 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/miniRT.h"

/*
 * Calculate base center of the cone
 */
static t_vec3 calculate_base_center(t_cone cone)
{
	return vec3_add(cone.apex, vec3_scale(cone.axis, cone.height));
}

/*
 * Calculate quadratic equation coefficients for cone-ray intersection
 */
static void calculate_cone_equation_coeffs(t_ray ray, t_cone cone,
									   t_vec3 apex_to_origin, double *coeffs)
{
	double cos_squared;
	double dot_dir_axis;
	double dot_origin_axis;

	// Precompute cos squared of cone angle
	cos_squared = cos(cone.angle) * cos(cone.angle);

	// Calculate dot products needed for quadratic coefficients
	dot_dir_axis = vec3_dot(ray.direction, cone.axis);
	dot_origin_axis = vec3_dot(apex_to_origin, cone.axis);

	// Store coefficients a, b, c in the array
	coeffs[0] = dot_dir_axis * dot_dir_axis -
			   cos_squared * vec3_dot(ray.direction, ray.direction);
	coeffs[1] = 2.0 * (dot_origin_axis * dot_dir_axis -
					 cos_squared * vec3_dot(apex_to_origin, ray.direction));
	coeffs[2] = dot_origin_axis * dot_origin_axis -
			   cos_squared * vec3_dot(apex_to_origin, apex_to_origin);
}


/*
 * Check if point is on cone surface at given height
 */
static int is_on_cone_surface(t_vec3 hit, t_cone cone, double projection)
{
	double radius_at_height;
	t_vec3 point_on_axis;
	double dist_from_axis;

	radius_at_height = (projection / cone.height) * cone.radius;
	point_on_axis = vec3_add(cone.apex, vec3_scale(cone.axis, projection));
	dist_from_axis = vec3_length(vec3_subtract(hit, point_on_axis));

	return (dist_from_axis <= radius_at_height * 1.001);
}

/*
 * Check a candidate intersection distance for cone body
 */
static int check_t_candidate(t_ray ray, t_cone cone, double t_candidate, double *t_body)
{
	t_vec3 hit;
	t_vec3 hit_to_apex;
	double projection;

	if (t_candidate <= 0.001)
		return 0;

	hit = vec3_add(ray.origin, vec3_scale(ray.direction, t_candidate));
	hit_to_apex = vec3_subtract(hit, cone.apex);
	projection = vec3_dot(hit_to_apex, cone.axis);

	if (projection > 0.001 && projection <= cone.height)
	{
		if (is_on_cone_surface(hit, cone, projection))
		{
			*t_body = t_candidate;
			return 1;
		}
	}

	return 0;
}

/*
 * Check if ray is parallel to cone surface
 */
static int check_parallel_ray(double b, double *t_body)
{
	if (fabs(b) < 0.0001)
	{
		*t_body = INFINITY;
		return 0;
	}
	return 1;
}

/*
 * Check linear case intersection point
 */
static int check_linear_hit(t_ray ray, t_cone cone, double t_candidate, double *t_body)
{
	t_vec3 hit;
	t_vec3 hit_to_apex;
	double projection;

	if (t_candidate <= 0.001)
	{
		*t_body = INFINITY;
		return 0;
	}

	hit = vec3_add(ray.origin, vec3_scale(ray.direction, t_candidate));
	hit_to_apex = vec3_subtract(hit, cone.apex);
	projection = vec3_dot(hit_to_apex, cone.axis);

	if (projection > 0.001 && projection <= cone.height)
	{
		*t_body = t_candidate;
		return 1;
	}

	*t_body = INFINITY;
	return 0;
}

/*
 * Handle linear intersection case (when a is near zero)
 */
static int handle_linear_case(t_ray ray, t_cone cone, double *coeffs, double *t_body)
{
	double b = coeffs[1];
	double c = coeffs[2];
	double t_candidate;

	if (!check_parallel_ray(b, t_body))
		return 0;

	t_candidate = -c / b;
	return check_linear_hit(ray, cone, t_candidate, t_body);
}

/*
 * Calculate discriminant for quadratic equation
 */
static int calculate_discriminant(double *coeffs, double *t_body, double *result)
{
	*result = coeffs[1] * coeffs[1] - 4 * coeffs[0] * coeffs[2];

	if (*result < 0)
	{
		*t_body = INFINITY;
		return 0;
	}

	return 1;
}

/*
 * Check one quadratic solution
 */
static int check_one_solution(t_ray ray, t_cone cone, double t_val, double *t_body)
{
	if (check_t_candidate(ray, cone, t_val, t_body))
		return 1;
	return 0;
}

/*
 * Check both quadratic solutions
 */
static int check_quadratic_solutions(t_ray ray, t_cone cone, double *t_vals, double *t_body)
{
	int hit_found = 0;
	double temp;

	*t_body = INFINITY;

	// Check first solution (t_vals[0] = t1)
	if (check_one_solution(ray, cone, t_vals[0], t_body))
		hit_found = 1;

	// Check second solution (t_vals[1] = t2)
	if (!hit_found || t_vals[1] < *t_body)
	{
		temp = *t_body;
		if (check_one_solution(ray, cone, t_vals[1], t_body))
			hit_found = 1;
		else if (hit_found)
			*t_body = temp;
	}

	if (!hit_found)
		*t_body = INFINITY;

	return hit_found;
}

/*
 * Solve quadratic equation for cone body intersection
 */
static int solve_cone_quadratic(t_ray ray, t_cone cone, double *coeffs, double *t_body)
{
	double discriminant;
	double sqrt_discriminant;
	double t_vals[2]; // Store both solutions

	if (!calculate_discriminant(coeffs, t_body, &discriminant))
		return 0;

	sqrt_discriminant = sqrt(discriminant);
	t_vals[0] = (-coeffs[1] - sqrt_discriminant) / (2.0 * coeffs[0]);
	t_vals[1] = (-coeffs[1] + sqrt_discriminant) / (2.0 * coeffs[0]);

	return check_quadratic_solutions(ray, cone, t_vals, t_body);
}

/*
 * Find closest intersection among body and base
 */
static int find_closest_hit(double t_body, double t_base, double *t)
{
	double closest_t = INFINITY;

	// Only consider values greater than threshold
	if (t_body > 0.001 && t_body < closest_t)
		closest_t = t_body;

	if (t_base > 0.001 && t_base < closest_t)
		closest_t = t_base;

	if (closest_t < INFINITY)
	{
		*t = closest_t;
		return 1;
	}

	return 0;
}

/*
 * Ray-cone intersection test
 */
int ray_cone_intersect(t_ray ray, t_cone cone, double *t)
{
	t_vec3 base_center, apex_to_origin;
	double coeffs[3]; // a, b, c
	double t_body = INFINITY;
	// int hit_body = 0;
	double t_base;
	// int hit_base;

	base_center = calculate_base_center(cone);
	apex_to_origin = vec3_subtract(ray.origin, cone.apex);

	calculate_cone_equation_coeffs(ray, cone, apex_to_origin, coeffs);

	// Handle different cases based on coefficient values
	if (fabs(coeffs[0]) < 0.0001)
		handle_linear_case(ray, cone, coeffs, &t_body);
	else
		solve_cone_quadratic(ray, cone, coeffs, &t_body);

	// Check for intersection with the base disc
	t_base = ray_disc_intersect(ray, base_center, cone.axis, cone.radius);
	// hit_base = (t_base > 0.001);

	// Return the closest valid hit
	return find_closest_hit(t_body, t_base, t);
}

/*
** FIXED cone_normal_at_point - ensures proper base detection
*/
t_vec3 cone_normal_at_point(t_vec3 point, t_cone cone)
{
	t_vec3 base_center;
	t_vec3 to_point;
	double projection;

	// Calculate base center - EXACTLY same as in intersection
	base_center = vec3_add(cone.apex, vec3_scale(cone.axis, cone.height));

	// Vector from apex to point
	to_point = vec3_subtract(point, cone.apex);

	// Project onto cone axis
	projection = vec3_dot(to_point, cone.axis);

	// Determine if point is on base or surface
	// Use distance from base center as primary criteria
	t_vec3 point_to_base = vec3_subtract(point, base_center);
	double dist_to_base_plane = fabs(vec3_dot(point_to_base, cone.axis));
	double dist_to_base_center = vec3_length(point_to_base);

	// If point is very close to the base plane AND within base radius
	if (dist_to_base_plane < 0.01 && dist_to_base_center <= cone.radius * 1.01)
	{
		// This is definitely on the base
		return vec3_normalize(cone.axis);
	}

	// If projection is at or very near the height, it's on the base edge
	if (projection >= cone.height - 0.005)
	{
		return vec3_normalize(cone.axis);
	}

	// Point is on the conical surface
	// Clamp projection to valid range
	if (projection <= 0.001)
		projection = 0.001;
	if (projection >= cone.height)
		projection = cone.height - 0.001;

	// Calculate surface normal
	t_vec3 axis_point = vec3_add(cone.apex, vec3_scale(cone.axis, projection));
	t_vec3 radial = vec3_subtract(point, axis_point);

	// Handle degenerate case (point on axis)
	double radial_length = vec3_length(radial);
	if (radial_length < 0.0001)
	{
		// Create a perpendicular vector
		t_vec3 perp;
		if (fabs(cone.axis.x) < 0.9)
			perp = vec3_cross(cone.axis, vec3_create(1, 0, 0));
		else
			perp = vec3_cross(cone.axis, vec3_create(0, 1, 0));
		radial = vec3_normalize(perp);
	}
	else
	{
		radial = vec3_scale(radial, 1.0 / radial_length);
	}

	// Calculate cone slope
	double cone_slope = cone.radius / cone.height;

	// Surface normal: radial component minus axial component scaled by slope
	t_vec3 normal = vec3_subtract(radial, vec3_scale(cone.axis, cone_slope));

	return vec3_normalize(normal);
}
