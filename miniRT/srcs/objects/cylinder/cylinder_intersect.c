/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder_intersect.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 15:00:00 by abillote          #+#    #+#             */
/*   Updated: 2025/06/19 11:36:46 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/miniRT.h"

/*
 * Calculates the quadratic equation coefficients and
 discriminant for cylinder-ray intersection
 *
 * This function computes the coefficients (a,b,c) of the quadratic equation
 * that determines where a ray intersects a cylinder's body,
 * and returns the discriminant.
 * The discriminant indicates if there are intersections (>0),
 * one tangent point (=0), or no intersections (<0).
 *
 * @param ray       The ray to test for intersection
 * @param cylinder  The cylinder to test against
 * @param a         Pointer to store quadratic coefficient a
 * @param b         Pointer to store quadratic coefficient b
 *
 * @return          The discriminant of the quadratic equation
 */
double	calculate_cylinder_equation_discriminant(t_ray ray, \
				t_cylinder cylinder, double *a, double *b)
{
	t_vec3	oc;
	t_vec3	cross_dir_axis;
	t_vec3	cross_oc_axis;
	double	c;
	double	discriminant;

	oc = vec3_subtract(ray.origin, cylinder.center);
	cross_dir_axis = vec3_cross(ray.direction, cylinder.axis);
	cross_oc_axis = vec3_cross(oc, cylinder.axis);
	*a = vec3_length_squared(cross_dir_axis);
	*b = 2.0 * vec3_dot(cross_dir_axis, cross_oc_axis);
	c = vec3_length_squared(cross_oc_axis) - cylinder.radius * cylinder.radius;
	discriminant = *b * *b - 4 * *a * c;
	return (discriminant);
}

/*
 * Validates a potential cylinder body intersection point
 *
 * For a given ray intersection distance, this function
 * checks if the intersection point falls within the
 * height bounds of the cylinder (between the caps). If valid
 * and closer than previous intersections,
 * updates the intersection distance.
 *
 * @param ray          The ray to test for intersection
 * @param cylinder     The cylinder to test against
 * @param t_candidate  Potential intersection distance to validate
 * @param t            Pointer to update with the new
 * intersection distance if valid
 */
void	calculate_body_intersection_point(t_ray ray, \
					t_cylinder cylinder, double t_candidate, double *t)
{
	t_vec3	point;
	t_vec3	to_point;
	double	projection;

	if (t_candidate > 0.0001)
	{
		point = vec3_add(ray.origin, vec3_scale(ray.direction, \
						t_candidate));
		to_point = vec3_subtract(point, cylinder.center);
		projection = vec3_dot(to_point, cylinder.axis);
		if (projection >= -cylinder.height / 2.0 && \
					projection <= cylinder.height / 2.0)
		{
			if (t_candidate < *t)
				*t = t_candidate;
		}
	}
}

/*
 * Calculates ray intersections with the curved body of a cylinder
 *
 * Determines if and where a ray intersects the cylindrical body (not the caps)
 * by solving the quadratic intersection equation.
 *
 * @param ray       The ray to test for intersection
 * @param cylinder  The cylinder to test against
 * @param t         Pointer to store closest intersection distance
 * @param a         Pointer to store quadratic coefficient a for later use
 */
static void	check_cylinder_body_intersection(t_ray ray, \
				t_cylinder cylinder, double *t, double *a)
{
	double	discriminant;
	double	b;
	double	t1;
	double	t2;

	discriminant = calculate_cylinder_equation_discriminant(ray, \
						cylinder, *(&a), &b);
	if (discriminant >= 0)
	{
		t1 = (-b - sqrt(discriminant)) / (2 * *a);
		t2 = (-b + sqrt(discriminant)) / (2 * *a);
		calculate_body_intersection_point(ray, cylinder, t1, *(&t));
		calculate_body_intersection_point(ray, cylinder, t2, *(&t));
	}
}

/*
 * Determines the closest valid intersection point with a cylinder
 *
 * Compares the distances to potential intersection points with the cylinder's
 * body, top cap, and bottom cap, accounting for special cases when the ray
 * is parallel to the cylinder axis.
 *
 * @param t_top     Distance to top cap intersection or INFINITY
 * @param t_bottom  Distance to bottom cap intersection or INFINITY
 * @param t_body    Distance to body intersection or INFINITY
 * @param a         Coefficient a from body intersection calculation
 *
 * @return          The closest valid intersection distance
 * or INFINITY if none
 */
double	find_cylinder_closest_intersection(double t_top, \
						double t_bottom, double t_body, double a)
{
	double	closest_t;

	closest_t = INFINITY;
	if (a < 0.0001)
	{
		if (t_top > 0.0 && t_top < closest_t)
			closest_t = t_top;
		if (t_bottom > 0.0 && t_bottom < closest_t)
			closest_t = t_bottom;
	}
	else
	{
		if (t_body > 0.0 && t_body < closest_t)
			closest_t = t_body;
		if (t_top > 0.0 && t_top < closest_t)
			closest_t = t_top;
		if (t_bottom > 0.0 && t_bottom < closest_t)
			closest_t = t_bottom;
	}
	return (closest_t);
}

/*
 * Main cylinder intersection function - tests if a ray hits a cylinder
 *
 * Coordinates the testing of ray intersection with all parts of a cylinder
 * (curved body and both caps), finds the closest intersection point, and
 * returns whether an intersection was found.
 *
 * @param ray       The ray to test for intersection
 * @param cylinder  The cylinder to test against
 * @param t         Pointer to store the intersection distance if found
 *
 * @return          1 if intersection found, 0 otherwise
 */
int	ray_cylinder_intersect(t_ray ray, t_cylinder cylinder, double *t)
{
	double	t_body;
	double	t_top;
	double	t_bottom;
	double	a;
	double	closest_t;

	t_top = INFINITY;
	t_bottom = INFINITY;
	t_body = INFINITY;
	closest_t = INFINITY;
	check_cylinder_caps_intersection(ray, cylinder, &t_top, &t_bottom);
	check_cylinder_body_intersection(ray, cylinder, &t_body, &a);
	closest_t = find_cylinder_closest_intersection(t_top, t_bottom, t_body, a);
	if (closest_t < INFINITY)
	{
		*t = closest_t;
		return (1);
	}
	return (0);
}
