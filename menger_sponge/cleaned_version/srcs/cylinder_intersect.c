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

//int	is_within_cylinder_height(t_ray ray, t_cylinder cylinder, double t)
//{
//	t_vec3	intersection;
//	t_vec3	v;
//	double	projected_length;

//	//Calculate interesection point : P = o + td;
//	intersection = vec3_add(ray.origin, vec3_scale(ray.direction, t));

//	//Calculate vector from cylinder center to the intersection point : v = P - C
//	v = vec3_subtract(intersection, cylinder.center);

//	// Project this vector into the axis : proj = v.A
//	projected_length = vec3_dot(v, cylinder.axis);

//	if (fabs(projected_length) <= cylinder.height / 2.0)
//		return (1);
//	else
//		return (0);
//}

//int	ray_cylinder_caps_intersect(t_ray ray, t_cylinder cylinder, double *t)
//{
//	double	t_min;
//	int		hit;
//	t_vec3	top_center;
//	t_vec3	bottom_center;
//	double	denom;
//	double	t_top;
//	double	t_bottom;
//	t_vec3	hit_point;

//	t_min = INFINITY;
//	hit = 0;

//	//Calculate cylinder top center and bottom center
//	top_center = vec3_add(cylinder.center, vec3_scale(cylinder.axis, cylinder.height / 2.0));
//	bottom_center = vec3_add(cylinder.center, vec3_scale(cylinder.axis, -cylinder.height / 2.0));

//	//Check top cap
//	denom = vec3_dot(ray.direction, cylinder.axis); //dot product between ray dir and cap normal
//	if (fabs(denom) > 0.0001) //Not parallel
//	{
//		//Find t from the ray plane intersection formula: t = ((Po - o).N) / (d.N)
//		//Po = any point on the plane, in this case cylinder top center
//		//o = ray origin
//		//N = plane normal, in this case the cylinder axis
//		//d = ray direction
//		//denom already = d.N
//		t_top = vec3_dot(vec3_subtract(top_center, ray.origin), cylinder.axis) / denom;
//		if (t_top > 0.0001)
//		{
//			//Check if the ray intersect within the boundary of top cap
//			//Calculate hit point - hit = o + td;
//			hit_point = vec3_add(ray.origin, vec3_scale(ray.direction, t_top));
//			// Check if the hit is within the cap circle
//			// Use squared distance for better precision
//			double dist_squared = vec3_length_squared(vec3_subtract(hit_point, top_center));
//			if (dist_squared <= (cylinder.radius * cylinder.radius) * 1.001) // Slight tolerance
//			{
//				t_min = t_top;
//				hit = 1;
//			}
//		}
//		//Check bottom cap
//		t_bottom = vec3_dot(vec3_subtract(bottom_center, ray.origin), cylinder.axis) / denom;
//		if (t_bottom > 0.001 && t_bottom < t_min)
//		{
//			hit_point = vec3_add(ray.origin, vec3_scale(ray.direction, t_bottom));
//			double dist_squared = vec3_length_squared(vec3_subtract(hit_point, bottom_center));
//			if (dist_squared <= (cylinder.radius * cylinder.radius) * 1.001) // Slight tolerance
//			{
//				t_min = t_bottom;
//				hit = 1;
//			}
//		}
//	}
//	if (hit)
//	{
//		*t = t_min;
//		return (1);
//	}
//	return (0);
//}

//int	ray_cylinder_intersect(t_ray ray, t_cylinder cylinder, double *t)
//{
//	double	a;
//	double	b;
//	double	c;
//	double	discriminant;
//	t_vec3	oc;

//	//calculate vector from sphere center to ray origin
//	oc = vec3_subtract(ray.origin, cylinder.center);

//	// Calculate coefficients for quadratic equation
//	// Optimized version that better handles numerical precision
//	t_vec3 ray_cross_axis = vec3_cross(ray.direction, cylinder.axis);
//	t_vec3 oc_cross_axis = vec3_cross(oc, cylinder.axis);

//	a = vec3_dot(ray_cross_axis, ray_cross_axis);
//	b = 2.0 * vec3_dot(ray_cross_axis, oc_cross_axis);
//	c = vec3_dot(oc_cross_axis, oc_cross_axis) - cylinder.radius * cylinder.radius;

//	//calculate the discrimant to know if equation has solutions
//	discriminant = b * b - (4 * a * c);

//	if (discriminant < 0)
//		return (0); //no real solution

//	// Find the nearest solution
//	double t1 = (-b - sqrt(discriminant)) / (2.0 * a);
//	double t2 = (-b + sqrt(discriminant)) / (2.0 * a);
//	double t_min = fmin(t1, t2);
//	double t_max = fmax(t1, t2);

//	////Check if intersection point is within cylinder height
//	if (t_min > 0.001)
//	{
//		if (is_within_cylinder_height(ray, cylinder, t_min))
//		{
//			*t = t_min;
//			return (1);
//		}
//	}
//	if (t_max > 0.001)
//	{
//		if (is_within_cylinder_height(ray, cylinder, t_max))
//		{
//			*t = t_max;
//			return (1);
//		}
//	}

//	//Check for cap intersections
//	double t_cap = INFINITY;
//	if (ray_cylinder_caps_intersect(ray, cylinder, &t_cap) && t_cap < *t)
//	{
//		*t = t_cap;
//		return (1);
//	}
//	return (0);
//}

//t_vec3	cylinder_normal_at_point(t_vec3 point, t_cylinder cylinder)
//{
//	{
//		t_vec3 normal;
//		t_vec3 cp;
//		double projection;
//		t_vec3 axis_projection;
//		t_vec3 top_center, bottom_center;

//		// Calculate caps positions
//		top_center = vec3_add(cylinder.center, vec3_scale(cylinder.axis, cylinder.height / 2.0));
//		bottom_center = vec3_add(cylinder.center, vec3_scale(cylinder.axis, -cylinder.height / 2.0));

//		// Check if we're on top cap
//		double dist_to_top_plane = fabs(vec3_dot(vec3_subtract(point, top_center), cylinder.axis));
//		if (dist_to_top_plane < 0.01) // More forgiving epsilon
//		{
//			// Check if point is within the cap circle
//			double dist_to_top_center = vec3_length(vec3_subtract(point, top_center));
//			if (dist_to_top_center <= cylinder.radius * 1.001) // Slight tolerance
//				return cylinder.axis;
//		}

//		// Check if we're on bottom cap
//		double dist_to_bottom_plane = fabs(vec3_dot(vec3_subtract(point, bottom_center), cylinder.axis));
//		if (dist_to_bottom_plane < 0.01) // More forgiving epsilon
//		{
//			// Check if point is within the cap circle
//			double dist_to_bottom_center = vec3_length(vec3_subtract(point, bottom_center));
//			if (dist_to_bottom_center <= cylinder.radius * 1.001) // Slight tolerance
//				return vec3_negate(cylinder.axis);
//		}

//		// If not on caps, calculate normal for the side of the cylinder
//		// 1. Calculate hit point to cylinder center vector
//		cp = vec3_subtract(point, cylinder.center);

//		// 2. Calculate scalar projection along axis
//		projection = vec3_dot(cp, cylinder.axis);

//		// 3. Calculate axis projection vector
//		axis_projection = vec3_scale(cylinder.axis, projection);

//		// 4. Calculate normal vector (cp - axis_projection)
//		normal = vec3_subtract(cp, axis_projection);

//		return vec3_normalize(normal);
//	}
//}

int	find_closest_intersection(t_scene *scene, t_ray ray, double *t, t_object **hit_object)
{
	t_object	*current;
	double		t_closest;
	double		t_temp;
	int			hit_something;

	current = scene->objects;
	t_closest = INFINITY;
	hit_something = 0;
	*hit_object = NULL;
	while (current)
	{
		if (current->type == SPHERE)
		{
			t_sphere *sphere = (t_sphere *)(current->data);
			if (ray_sphere_intersect(ray, *sphere, &t_temp) && t_temp < t_closest)
			{
				t_closest = t_temp;
				hit_something = 1;
				*hit_object = current;
			}
		}
		else if (current->type == CYLINDER)
		{
			t_cylinder *cylinder = (t_cylinder *)(current->data);
			if (ray_cylinder_intersect(ray, *cylinder, &t_temp) && t_temp < t_closest)
			{
				t_closest = t_temp;
				hit_something = 1;
				*hit_object = current;
			}
		}
		current = current->next;
	}
	if (hit_something)
		*t = t_closest;
	return (hit_something);
}


/*
 * Complete rewrite of cylinder intersection functions to ensure no holes appear
 * when viewing the cylinder from any angle.
 */

// Helper function to check if a point is within a disc (for caps)
static int is_point_in_disc(t_vec3 point, t_vec3 center, t_vec3 normal, double radius)
{
    // Project point onto plane
    t_vec3 to_point = vec3_subtract(point, center);
    double dist_to_plane = vec3_dot(to_point, normal);

    // If point is too far from plane, not on disc
    if (fabs(dist_to_plane) > 0.01)
        return 0;

    // Project point onto plane
    t_vec3 point_on_plane = vec3_subtract(point, vec3_scale(normal, dist_to_plane));

    // Check if point is within radius
    double dist_squared = vec3_length_squared(vec3_subtract(point_on_plane, center));
    return (dist_squared <= radius * radius * 1.01); // 1% tolerance
}

// Helper function for disc (cap) intersection
static int ray_disc_intersect(t_ray ray, t_vec3 center, t_vec3 normal,
                             double radius, double *t)
{
    double denom = vec3_dot(ray.direction, normal);

    // If ray is parallel to disc plane, no intersection
    if (fabs(denom) < 0.0001)
        return 0;

    // Calculate intersection with plane
    double t_plane = vec3_dot(vec3_subtract(center, ray.origin), normal) / denom;

    // If intersection is behind ray origin, no valid intersection
    if (t_plane <= 0.0001)
        return 0;

    // Calculate intersection point
    t_vec3 point = vec3_add(ray.origin, vec3_scale(ray.direction, t_plane));

    // Check if point is within disc
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
    t_vec3 top_center, bottom_center;
    double t_body, t_top, t_bottom;
    int hit_body, hit_top, hit_bottom;

    // Prepare top and bottom center points
    top_center = vec3_add(cylinder.center, vec3_scale(cylinder.axis, cylinder.height / 2.0));
    bottom_center = vec3_add(cylinder.center, vec3_scale(cylinder.axis, -cylinder.height / 2.0));

    // Test caps intersection first
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

    // Calculate body intersection
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

// Improved normal calculation
t_vec3 cylinder_normal_at_point(t_vec3 point, t_cylinder cylinder)
{
    t_vec3 top_center, bottom_center;

    // Calculate caps positions
    top_center = vec3_add(cylinder.center, vec3_scale(cylinder.axis, cylinder.height / 2.0));
    bottom_center = vec3_add(cylinder.center, vec3_scale(cylinder.axis, -cylinder.height / 2.0));

    // Check if point is on top cap with generous threshold
    if (is_point_in_disc(point, top_center, cylinder.axis, cylinder.radius))
    {
        return cylinder.axis; // Normal points up
    }

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
