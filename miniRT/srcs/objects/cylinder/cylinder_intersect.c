/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder_intersect.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 15:00:00 by abillote          #+#    #+#             */
/*   Updated: 2025/06/19 10:55:44 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/miniRT.h"

// Helper function for disc (cap) intersection
// Return the intersection point with the caps if it exists
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

static void	check_cylinder_caps_intersection(t_ray ray, t_cylinder cylinder, double *t_top, double *t_bottom)
{
	t_vec3	top_center;
	t_vec3	bottom_center;

	top_center = vec3_add(cylinder.center, vec3_scale(cylinder.axis, cylinder.height / 2.0));
	bottom_center = vec3_subtract(cylinder.center, vec3_scale(cylinder.axis, cylinder.height / 2.0));
	*t_top = ray_disc_intersect(ray, top_center, cylinder.axis, cylinder.radius);
	*t_bottom = ray_disc_intersect(ray, bottom_center, vec3_negate(cylinder.axis), cylinder.radius);
}

double	calculate_cylinder_equation_discriminant(t_ray ray, t_cylinder cylinder, double *a, double *b)
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

void	calculate_body_intersection_point(t_ray ray, t_cylinder cylinder, double t_candidate, double *t)
{
	t_vec3	point;
	t_vec3	to_point;
	double	projection;

	if (t_candidate > 0.0001)
	{
		point = vec3_add(ray.origin, vec3_scale(ray.direction, t_candidate));
		to_point = vec3_subtract(point, cylinder.center);
		projection = vec3_dot(to_point, cylinder.axis);
		if (projection >= -cylinder.height / 2.0 && projection <= cylinder.height / 2.0)
		{
			if (t_candidate < *t)
				*t = t_candidate;
		}
	}
}

static void	check_cylinder_body_intersection(t_ray ray, t_cylinder cylinder, double *t, double *a)
{
	double	discriminant;
	double	b;
	double	t1;
	double	t2;

	discriminant = calculate_cylinder_equation_discriminant(ray, cylinder, *(&a), &b);
	if (discriminant >= 0)
	{
		t1 = (-b - sqrt(discriminant)) / (2 * *a);
		t2 = (-b + sqrt(discriminant)) / (2 * *a);
		calculate_body_intersection_point(ray, cylinder, t1, *(&t));
		calculate_body_intersection_point(ray, cylinder, t2, *(&t));
	}
}

double	find_cylinder_closest_intersection(double t_top, double t_bottom, double t_body, double a)
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

// Complete ray-cylinder intersection
int ray_cylinder_intersect(t_ray ray, t_cylinder cylinder, double *t)
{
	double t_body;
	double t_top;
	double t_bottom;
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
		return 1;
	}
	return 0;
}

// Normal calculation with precise cap detection
t_vec3 cylinder_normal_at_point(t_vec3 point, t_cylinder cylinder)
{
    t_vec3 top_center;
    t_vec3 bottom_center;
    t_vec3 to_point;
    double projection;

    // Calculate caps positions - EXACTLY same as in intersection
    top_center = vec3_add(cylinder.center, vec3_scale(cylinder.axis, cylinder.height / 2.0));
    bottom_center = vec3_subtract(cylinder.center, vec3_scale(cylinder.axis, cylinder.height / 2.0));

    // Project point onto cylinder axis
    to_point = vec3_subtract(point, cylinder.center);
    projection = vec3_dot(to_point, cylinder.axis);

    double half_height = cylinder.height / 2.0;

    // Use a very small threshold to determine if we're on a cap
    double cap_threshold = 0.0001;

    // Check if point is on top cap
    if (projection > (half_height - cap_threshold))
    {
        // Verify the point is actually within the cap radius
        double dist_from_top_center = vec3_length(vec3_subtract(point, top_center));
        if (dist_from_top_center <= cylinder.radius + cap_threshold)
        {
            return cylinder.axis; // Normal points up
        }
    }

    // Check if point is on bottom cap
    if (projection < -(half_height - cap_threshold))
    {
        // Verify the point is actually within the cap radius
        double dist_from_bottom_center = vec3_length(vec3_subtract(point, bottom_center));
        if (dist_from_bottom_center <= cylinder.radius + cap_threshold)
        {
            return vec3_negate(cylinder.axis); // Normal points down
        }
    }

    // Otherwise it's on the cylindrical side
    t_vec3 closest_on_axis = vec3_add(cylinder.center, vec3_scale(cylinder.axis, projection));
    return vec3_normalize(vec3_subtract(point, closest_on_axis));
}
