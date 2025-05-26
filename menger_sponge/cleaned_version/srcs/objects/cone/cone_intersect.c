/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone_intersect.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 11:16:07 by abillote          #+#    #+#             */
/*   Updated: 2025/05/26 14:34:34 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "platform.h"

/*
 * FIXED ray_disc_intersect for cone base - ensures exact positioning
 */
static int ray_disc_intersect_cone(t_ray ray, t_vec3 center, t_vec3 normal, double radius, double *t)
{
    double denom = vec3_dot(ray.direction, normal);

    // If ray is parallel to disc plane, no intersection
    if (fabs(denom) < 0.0001)
        return 0;

    // Calculate intersection with the plane containing the disc
    double t_plane = vec3_dot(vec3_subtract(center, ray.origin), normal) / denom;

    // If intersection is behind ray origin, no valid intersection
    if (t_plane <= 0.001)
        return 0;

    // Calculate the actual intersection point
    t_vec3 intersection_point = vec3_add(ray.origin, vec3_scale(ray.direction, t_plane));

    // Calculate distance from intersection point to disc center
    t_vec3 center_to_point = vec3_subtract(intersection_point, center);
    double distance_squared = vec3_length_squared(center_to_point);

    // Check if point is within the disc radius
    if (distance_squared <= radius * radius)
    {
        *t = t_plane;
        return 1;
    }

    return 0;
}

int ray_cone_intersect(t_ray ray, t_cone cone, double *t)
{
    t_vec3  base_center;
    t_vec3  apex_to_origin;
    double  cos_squared;
    double  dot_dir_axis;
    double  dot_origin_axis;
    double  a, b, c;
    double  discriminant;
    double  t_body = INFINITY;
    int     hit_body = 0;
    int     hit_base = 0;
    double  t1, t2;
    double  t_base;

    // CRITICAL: Calculate base center consistently
    // The base should be EXACTLY at apex + axis * height
    base_center = vec3_add(cone.apex, vec3_scale(cone.axis, cone.height));

    // Vector from apex to ray origin
    apex_to_origin = vec3_subtract(ray.origin, cone.apex);

    // Precompute cos squared of cone angle
    cos_squared = cos(cone.angle) * cos(cone.angle);

    // Calculate dot products needed for quadratic coefficients
    dot_dir_axis = vec3_dot(ray.direction, cone.axis);
    dot_origin_axis = vec3_dot(apex_to_origin, cone.axis);

    // Calculate quadratic coefficients for cone surface intersection
    a = dot_dir_axis * dot_dir_axis - cos_squared * vec3_dot(ray.direction, ray.direction);
    b = 2.0 * (dot_origin_axis * dot_dir_axis - cos_squared * vec3_dot(apex_to_origin, ray.direction));
    c = dot_origin_axis * dot_origin_axis - cos_squared * vec3_dot(apex_to_origin, apex_to_origin);

    // Handle degenerate case (ray parallel to cone surface)
    if (fabs(a) < 0.0001)
    {
        if (fabs(b) < 0.0001)
            goto check_base;

        // Linear case: one intersection point
        t_body = -c / b;
        if (t_body > 0.001)
        {
            t_vec3 hit = vec3_add(ray.origin, vec3_scale(ray.direction, t_body));
            t_vec3 hit_to_apex = vec3_subtract(hit, cone.apex);
            double projection = vec3_dot(hit_to_apex, cone.axis);

            // CRITICAL: Surface must end EXACTLY where base begins
            if (projection > 0.001 && projection <= cone.height)
            {
                hit_body = 1;
            }
        }
    }
    else
    {
        // Solve quadratic equation
        discriminant = b * b - 4 * a * c;

        if (discriminant >= 0)
        {
            double sqrt_discriminant = sqrt(discriminant);
            t1 = (-b - sqrt_discriminant) / (2.0 * a);
            t2 = (-b + sqrt_discriminant) / (2.0 * a);

            // Check both intersection points
            for (int i = 0; i < 2; i++)
            {
                double t_candidate = (i == 0) ? t1 : t2;

                if (t_candidate > 0.001)
                {
                    t_vec3 hit = vec3_add(ray.origin, vec3_scale(ray.direction, t_candidate));
                    t_vec3 hit_to_apex = vec3_subtract(hit, cone.apex);
                    double projection = vec3_dot(hit_to_apex, cone.axis);

                    // CRITICAL: Cone surface goes from 0 to EXACTLY height
                    // No gap between surface and base
                    if (projection > 0.001 && projection <= cone.height)
                    {
                        // Validate point is on cone surface
                        double radius_at_height = (projection / cone.height) * cone.radius;
                        t_vec3 point_on_axis = vec3_add(cone.apex, vec3_scale(cone.axis, projection));
                        double dist_from_axis = vec3_length(vec3_subtract(hit, point_on_axis));

                        if (dist_from_axis <= radius_at_height * 1.001)
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
        }
    }

check_base:
    // Check for intersection with the base disc
    // Base is at EXACTLY the same position as surface end
    hit_base = ray_disc_intersect_cone(ray, base_center, cone.axis, cone.radius, &t_base);

    // Choose the closest intersection
    double closest_t = INFINITY;

    if (hit_body && t_body < closest_t)
        closest_t = t_body;
    if (hit_base && t_base < closest_t)
        closest_t = t_base;

    if (closest_t < INFINITY)
    {
        *t = closest_t;
        return 1;
    }

    return 0;
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
