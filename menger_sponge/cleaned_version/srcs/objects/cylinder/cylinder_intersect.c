/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder_intersect.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 15:00:00 by abillote          #+#    #+#             */
/*   Updated: 2025/05/26 12:44:23 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

// Helper function for disc (cap) intersection
int ray_disc_intersect(t_ray ray, t_vec3 center, t_vec3 normal,
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

    // Check if point is within disc radius - NO extra tolerance
    double distance_squared = vec3_length_squared(vec3_subtract(point, center));
    if (distance_squared <= radius * radius)
    {
        *t = t_plane;
        return 1;
    }
    return 0;
}

// Complete ray-cylinder intersection
int ray_cylinder_intersect(t_ray ray, t_cylinder cylinder, double *t)
{
    t_vec3 top_center;
    t_vec3 bottom_center;
    double t_body = INFINITY;
    double t_top = INFINITY;
    double t_bottom = INFINITY;
    int hit_body = 0;
    int hit_top = 0;
    int hit_bottom = 0;

    // Calculate cap centers EXACTLY at the ends of the cylinder
    top_center = vec3_add(cylinder.center, vec3_scale(cylinder.axis, cylinder.height / 2.0));
    bottom_center = vec3_subtract(cylinder.center, vec3_scale(cylinder.axis, cylinder.height / 2.0));

    // Test caps intersection - using EXACT same radius as cylinder
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
        // Find the closest cap intersection
        double closest_t = INFINITY;
        if (hit_top && t_top < closest_t)
            closest_t = t_top;
        if (hit_bottom && t_bottom < closest_t)
            closest_t = t_bottom;

        if (closest_t < INFINITY)
        {
            *t = closest_t;
            return 1;
        }
        return 0;
    }

    // Calculate body intersection (infinite cylinder)
    double b = 2.0 * vec3_dot(cross_dir_axis, cross_oc_axis);
    double c = vec3_length_squared(cross_oc_axis) - cylinder.radius * cylinder.radius;

    double discriminant = b * b - 4 * a * c;

    if (discriminant >= 0)
    {
        double sqrt_discriminant = sqrt(discriminant);
        double t1 = (-b - sqrt_discriminant) / (2 * a);
        double t2 = (-b + sqrt_discriminant) / (2 * a);

        // Check both intersection points
        for (int i = 0; i < 2; i++)
        {
            double t_candidate = (i == 0) ? t1 : t2;

            if (t_candidate > 0.0001)
            {
                t_vec3 point = vec3_add(ray.origin, vec3_scale(ray.direction, t_candidate));
                t_vec3 to_point = vec3_subtract(point, cylinder.center);
                double projection = vec3_dot(to_point, cylinder.axis);

                // Check if intersection is within cylinder height bounds
                // Use EXACT height bounds - no tolerance
                if (projection >= -cylinder.height / 2.0 && projection <= cylinder.height / 2.0)
                {
                    if (!hit_body || t_candidate < t_body)
                    {
                        t_body = t_candidate;
                        hit_body = 1;
                    }
                }
            }
        }
    }

    // Find the closest valid intersection among body and caps
    double closest_t = INFINITY;

    if (hit_body && t_body < closest_t)
        closest_t = t_body;
    if (hit_top && t_top < closest_t)
        closest_t = t_top;
    if (hit_bottom && t_bottom < closest_t)
        closest_t = t_bottom;

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

// Helper function to check if a point is within a disc - CORRECTED
int is_point_in_disc(t_vec3 point, t_vec3 center, t_vec3 normal, double radius)
{
    t_vec3 to_point;
    double dist_to_plane;
    t_vec3 point_on_plane;
    double dist_squared;

    // Project point onto plane
    to_point = vec3_subtract(point, center);
    dist_to_plane = vec3_dot(to_point, normal);

    // If point is too far from plane, not on disc
    if (fabs(dist_to_plane) > 0.0001)
        return 0;

    // Project point onto plane
    point_on_plane = vec3_subtract(point, vec3_scale(normal, dist_to_plane));

    // Check if point is within radius - NO extra tolerance
    dist_squared = vec3_length_squared(vec3_subtract(point_on_plane, center));
    return (dist_squared <= radius * radius);
}
