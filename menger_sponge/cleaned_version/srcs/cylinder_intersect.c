///* ************************************************************************** */
///*                                                                            */
///*                                                        :::      ::::::::   */
///*   objects_intersections.c                            :+:      :+:    :+:   */
///*                                                    +:+ +:+         +:+     */
///*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
///*                                                +#+#+#+#+#+   +#+           */
///*   Created: 2025/04/30 11:16:07 by abillote          #+#    #+#             */
///*   Updated: 2025/05/08 13:00:40 by abillote         ###   ########.fr       */
///*                                                                            */
///* ************************************************************************** */

#include "platform.h"

// Helper function for disc (=cap) intersection
static int ray_disc_intersect(t_ray ray, t_vec3 center, t_vec3 normal,
								double radius, double *t)
{
	double denom = vec3_dot(ray.direction, normal);

	// If ray is parallel to disc plane, no intersection
	if (fabs(denom) < 0.0001)
		return 0;

	/*Calculate intersection with plane
	Ray-plane formula: t = ((Po - o).N)/(d.N)
	Po > a point on the plane (here, cap center)
	o > ray origin
	N > plane normal
	d > ray direction
	*/
	double t_plane = vec3_dot(vec3_subtract(center, ray.origin), normal) / denom;

	// If intersection is behind ray origin, no valid intersection
	if (t_plane <= 0.0001)
		return 0;

	// Calculate intersection point (point = o + td)
	t_vec3 point = vec3_add(ray.origin, vec3_scale(ray.direction, t_plane));

	// Check if point is within disc with error margin
	if (vec3_length_squared(vec3_subtract(point, center)) <= radius * radius * 1.01)
	{
		*t = t_plane;
		return 1;
	}
	return 0;
}

// Complete ray-cylinder intersection including caps and body
int ray_cylinder_intersect(t_ray ray, t_cylinder cylinder, double *t)
{
	t_vec3	top_center;
	t_vec3	bottom_center;
	double	t_body;
	double	t_top;
	double	t_bottom;
	int 	hit_body;
	int		hit_top;
	int		hit_bottom;

	// Prepare top and bottom center points
	top_center = vec3_add(cylinder.center, vec3_scale(cylinder.axis, cylinder.height / 2.0));
	bottom_center = vec3_add(cylinder.center, vec3_scale(cylinder.axis, -cylinder.height / 2.0));

	// Test caps intersection first
	//To rework > too many args
	hit_top = ray_disc_intersect(ray, top_center, cylinder.axis, cylinder.radius, &t_top);
	hit_bottom = ray_disc_intersect(ray, bottom_center, vec3_negate(cylinder.axis), cylinder.radius, &t_bottom);

	// Setup for body intersection
	t_vec3 oc = vec3_subtract(ray.origin, cylinder.center);
	t_vec3 cross_dir_axis = vec3_cross(ray.direction, cylinder.axis);
	t_vec3 cross_oc_axis = vec3_cross(oc, cylinder.axis);

	double a = vec3_length_squared(cross_dir_axis);

	// If ray is parallel to cylinder axis, only caps matter
	if (a < 0.0001)
	{
		if (hit_top && hit_bottom)
			*t = fmin(t_top, t_bottom);
		else if (hit_top)
			*t = t_top;
		else if (hit_bottom)
			*t = t_bottom;
		else
			return 0;
		return 1;
	}

	// Otherwise continue with body intersection
	double b = 2.0 * vec3_dot(cross_dir_axis, cross_oc_axis);
	double c = vec3_length_squared(cross_oc_axis) - cylinder.radius * cylinder.radius;

	double discriminant = b * b - 4 * a * c;

	// If no real solutions, no intersection with body
	if (discriminant < 0)
	{
		if (hit_top && hit_bottom)
			*t = fmin(t_top, t_bottom);
		else if (hit_top)
			*t = t_top;
		else if (hit_bottom)
			*t = t_bottom;
		else
			return 0;
		return 1;
	}

	// Calculate body intersection (= check if the ray intersect with the infinite cylinder)
	double sqrt_discriminant = sqrt(discriminant);
	double t1 = (-b - sqrt_discriminant) / (2 * a);
	double t2 = (-b + sqrt_discriminant) / (2 * a);

	// Select closest positive intersection
	if (t1 > 0.0001)
		t_body = t1;
	else if (t2 > 0.0001)
		t_body = t2;
	else
		t_body = INFINITY; // No valid body intersection

	// Check if body intersection is within cylinder height
	if (t_body < INFINITY)
	{
		t_vec3 point = vec3_add(ray.origin, vec3_scale(ray.direction, t_body));
		t_vec3 to_point = vec3_subtract(point, cylinder.center);
		double projection = vec3_dot(to_point, cylinder.axis);

		// If outside height bounds, discard body intersection
		if (fabs(projection) > cylinder.height / 2.0)
			hit_body = 0;
		else
			hit_body = 1;
	}
	else
	{
		hit_body = 0;
	}

	// Find closest valid intersection
	double t_min = INFINITY;

	if (hit_body && t_body < t_min)
		t_min = t_body;
	if (hit_top && t_top < t_min)
		t_min = t_top;
	if (hit_bottom && t_bottom < t_min)
		t_min = t_bottom;

	if (t_min < INFINITY)
	{
		*t = t_min;
		return 1;
	}

	return 0;
}

// Helper function to check if a point is within a disc (for caps) - normal calculation
static int is_point_in_disc(t_vec3 point, t_vec3 center, t_vec3 normal, double radius)
{
	t_vec3	to_point;
	double	dist_to_plane;
	t_vec3	point_on_plane;
	double	dist_squared;

	// Project point onto plane
	to_point = vec3_subtract(point, center);
	dist_to_plane = vec3_dot(to_point, normal);

	// If point is too far from plane, not on disc
	if (fabs(dist_to_plane) > 0.01)
		return 0;

	// Project point onto plane
	point_on_plane = vec3_subtract(point, vec3_scale(normal, dist_to_plane));

	// Check if point is within radius
	dist_squared = vec3_length_squared(vec3_subtract(point_on_plane, center));
	return (dist_squared <= radius * radius * 1.01); // 1% tolerance
}

// cylinder normal calculation
t_vec3	cylinder_normal_at_point(t_vec3 point, t_cylinder cylinder)
{
	t_vec3	top_center;
	t_vec3	bottom_center;

	// Calculate caps positions
	top_center = vec3_add(cylinder.center, vec3_scale(cylinder.axis, cylinder.height / 2.0));
	bottom_center = vec3_add(cylinder.center, vec3_scale(cylinder.axis, -cylinder.height / 2.0));

	// Check if point is on top cap with generous threshold
	if (is_point_in_disc(point, top_center, cylinder.axis, cylinder.radius))
		return cylinder.axis; // Normal points up

	// Check if point is on bottom cap with generous threshold
	if (is_point_in_disc(point, bottom_center, vec3_negate(cylinder.axis), cylinder.radius))
	{
		return vec3_negate(cylinder.axis); // Normal points down
	}

	// Otherwise calculate side normal
	// Project the point onto the cylinder axis
	t_vec3 to_point = vec3_subtract(point, cylinder.center);
	double projection = vec3_dot(to_point, cylinder.axis);
	t_vec3 closest_on_axis = vec3_add(cylinder.center, vec3_scale(cylinder.axis, projection));

	// Normal is from axis to point
	return vec3_normalize(vec3_subtract(point, closest_on_axis));
}
