/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checkerboard.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 14:40:00 by abillote          #+#    #+#             */
/*   Updated: 2025/05/22 11:16:31 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "platform.h"
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/**
 * Creates a checkerboard pattern specifically for planes
 *
 * @param plane The plane definition
 * @param point The point to check
 * @param checker_size Size of the checker squares
 * @return 1 if should use checker color, 0 if should use primary color
 */
int is_checker_point_plane(t_plane plane, t_vec3 point, double checker_size)
{
    t_vec3 normal = plane.normal;
    t_vec3 u_axis, v_axis;

    // Find two basis vectors that form a coordinate system on the plane
    // First, find one vector perpendicular to the normal
    if (fabs(normal.z) > 0.1 || fabs(normal.y) > 0.1)
        u_axis = vec3_normalize(vec3_cross(normal, vec3_create(1, 0, 0)));
    else
        u_axis = vec3_normalize(vec3_cross(normal, vec3_create(0, 1, 0)));

    // Find a second vector perpendicular to both normal and u_axis
    v_axis = vec3_normalize(vec3_cross(normal, u_axis));

    // Project the point onto the plane's coordinate system
    // First, get a vector from the plane point to our hit point
    t_vec3 to_point = vec3_subtract(point, plane.point);

    // Project onto the two axes
    double u = vec3_dot(to_point, u_axis) / checker_size;
    double v = vec3_dot(to_point, v_axis) / checker_size;

    // Determine the checker pattern
    int iu = (int)floor(u);
    int iv = (int)floor(v);

    return ((iu + iv) % 2 == 0) ? 1 : 0;
}

/**
 * Calculates the reflection of an incident ray around a normal vector
 *
 * @param incident The incoming ray direction (should be normalized)
 * @param normal The surface normal at the intersection point (should be normalized)
 * @return The direction of the reflected ray
 */
t_vec3 reflect_ray(t_vec3 incident, t_vec3 normal)
{
    // Formula: R = I - 2 * (I·N) * N
    // Where I is the incident ray direction, N is the normal, and R is the reflected ray direction

    // Calculate dot product between incident and normal
    double dot = vec3_dot(incident, normal);

    // Calculate reflection vector
    t_vec3 reflection;
    reflection.x = incident.x - 2.0 * dot * normal.x;
    reflection.y = incident.y - 2.0 * dot * normal.y;
    reflection.z = incident.z - 2.0 * dot * normal.z;

    // Normalize the reflection vector
    return vec3_normalize(reflection);
}

/**
 * Determines whether a point should use the primary or checker color
 *
 * @param point The intersection point in 3D space
 * @param checker_size The size of each checker square
 * @return 1 if should use checker color, 0 if should use primary color
 */
int is_checker_point(t_vec3 point, double checker_size)
{
	// Scale the point by the checker size
	double scale = 1.0 / checker_size;
	double x = point.x * scale;
	double y = point.y * scale;
	double z = point.z * scale;

	// Get the integer parts
	int ix = (int)floor(x);
	int iy = (int)floor(y);
	int iz = (int)floor(z);

	// The sum of the integer parts being even or odd determines the pattern
	// This creates a 3D checkerboard pattern
	return ((ix + iy + iz) % 2 == 0) ? 1 : 0;
}

/**
 * Gets the appropriate color at a point for a material with potential checkerboard
 *
 * @param material The material properties
 * @param point The intersection point
 * @return The color at that point
 */
t_color get_color_at_point(t_material material, t_object *object, t_vec3 point)
{
    // If this material doesn't have a checkerboard pattern, just return the primary color
    if (!material.has_checker)
        return material.color;

    // Choose the appropriate checker pattern function based on object type
    if (object->type == PLANE)
    {
        t_plane *plane = (t_plane *)object->data;
        if (is_checker_point_plane(*plane, point, material.checker_size))
            return material.checker_color;
        else
            return material.color;
    }
    else
    {
        // For other objects, use the general 3D checker pattern
        if (is_checker_point(point, material.checker_size))
            return material.checker_color;
        else
            return material.color;
    }
}

/**
 * Convert t_color to an integer RGB value
 *
 * @param color The color structure
 * @return Integer representation (0xRRGGBB)
 */
int color_to_int(t_color color)
{
	int r = (int)color.r;
	int g = (int)color.g;
	int b = (int)color.b;

	return (r << 16) | (g << 8) | b;
}

/**
 * Get pixel color for a material at a specific point with lighting applied
 * Enhanced version that supports checkerboard textures
 *
 * @param hit_object The object that was hit
 * @param light_intensity The amount of light at the point
 * @param light_color The color of the light
 * @param hit_point The point of intersection
 * @return The final pixel color
 */
int get_pixel_color_with_texture(t_object *hit_object, double light_intensity,
                               t_color light_color, t_vec3 hit_point,
                               double specular_intensity)
{
    // Get the base color from the material, considering checkerboard pattern
    t_color base_color = get_color_at_point(hit_object->material, hit_object, hit_point);

    // Calculate diffuse component (subtract specular from total light)
    double diffuse_component = light_intensity - specular_intensity;
    if (diffuse_component < 0)
        diffuse_component = 0;

    // Apply diffuse lighting calculations
    int r = (int)(base_color.r * diffuse_component * light_color.r / 255.0);
    int g = (int)(base_color.g * diffuse_component * light_color.g / 255.0);
    int b = (int)(base_color.b * diffuse_component * light_color.b / 255.0);

    // Add specular component (white highlight)
    int specular_r = (int)(255.0 * specular_intensity * light_color.r / 255.0);
    int specular_g = (int)(255.0 * specular_intensity * light_color.g / 255.0);
    int specular_b = (int)(255.0 * specular_intensity * light_color.b / 255.0);

    // Combine diffuse and specular
    r += specular_r;
    g += specular_g;
    b += specular_b;

    // Clamp values to valid range
    r = r > 255 ? 255 : r;
    g = g > 255 ? 255 : g;
    b = b > 255 ? 255 : b;

    // Convert to integer
    return (r << 16) | (g << 8) | b;
}

/**
 * Traces a ray through the scene, handling reflections, refractions,
 * and checkerboard textures recursively up to a maximum depth.
 *
 * @param scene The scene containing all objects
 * @param ray The ray to trace
 * @param depth The current recursion depth
 * @return The color observed along this ray
 */
int trace_ray_with_checkerboard(t_scene *scene, t_ray ray, int depth)
{
    double t;
    t_object *hit_object;
    t_vec3 hit_point;
    t_vec3 normal;
    t_light *current_light;
    t_light_result light_info;
    int in_shadow;
    int base_color;
    int reflected_color = 0;
    int refracted_color = 0;
    double fresnel_factor = 0.0;

    // Base case: maximum recursion depth reached or no intersection
    if (depth > MAX_RAY_DEPTH || !find_closest_intersection(scene, ray, &t, &hit_object))
    {
        return scene->background_color;
    }

    // Calculate hit point and normal
    compute_ray_intersection(ray, hit_object, t, &hit_point, &normal);

    // Get the color at the hit point, considering checkerboard if applicable
    // t_color object_color = get_color_at_point(hit_object->material, hit_point);

    // Start with ambient light
    base_color = get_pixel_color_with_texture(hit_object, scene->ambient.ratio, scene->ambient.color, hit_point, 0.0);

    // Add contribution from all lights
    current_light = scene->lights;
    while (current_light)
    {
        light_info = compute_light(scene, hit_object, hit_point, normal, current_light);

        // Check if the hit point is in shadow for this light
        if (scene->app.enable_hard_shadows)
            in_shadow = is_in_shadow(scene, hit_point, light_info.light_dir, light_info.light_distance);
        else
            in_shadow = 0;

        if (!in_shadow)
        {

            // Calculate light intensity
			double diffuse_intensity = current_light->intensity * light_info.diffuse;
			double specular_intensity = current_light->intensity * light_info.specular_intensity;

			// Get color with both diffuse and specular components
			int light_color = get_pixel_color_with_texture(
				hit_object,
				diffuse_intensity + scene->ambient.ratio, // Diffuse + ambient
				current_light->color,
				hit_point,
				specular_intensity
			);

            // Extract RGB components
            int r = (light_color >> 16) & 0xFF;
            int g = (light_color >> 8) & 0xFF;
            int b = light_color & 0xFF;

            // Add to base color
            int base_r = ((base_color >> 16) & 0xFF) + r;
            int base_g = ((base_color >> 8) & 0xFF) + g;
            int base_b = (base_color & 0xFF) + b;

            base_color = (valid_color_range(base_r) << 16) |
                         (valid_color_range(base_g) << 8) |
                         valid_color_range(base_b);
        }

        current_light = current_light->next;
    }

    // Handle reflections and refractions if enabled and not at max depth
    if (depth < MAX_RAY_DEPTH)
    {
        // Calculate reflection if object is reflective
        if (scene->app.enable_reflections && hit_object->material.reflectivity > 0.0)
        {
            t_vec3 reflect_dir = reflect_ray(ray.direction, normal);
            t_ray reflect_ray;

            // Create a reflection ray slightly offset from the hit point
            reflect_ray.origin = vec3_add(hit_point, vec3_scale(normal, 0.001));
            reflect_ray.direction = reflect_dir;

            // Trace the reflection ray
            reflected_color = trace_ray_with_checkerboard(scene, reflect_ray, depth + 1);
        }

        // Calculate refraction if object is transparent
        if (scene->app.enable_refraction && hit_object->material.transparency > 0.0)
        {
            // Determine if we're entering or exiting the medium
            int is_entering = is_entering_medium(ray.direction, normal);
            double ior_ratio;

            // Calculate index of refraction ratio based on whether we're entering or exiting
            if (is_entering)
            {
                // Air to medium
                ior_ratio = 1.0 / hit_object->material.refractive_index;
            }
            else
            {
                // Medium to air
                ior_ratio = hit_object->material.refractive_index;
            }

            // Calculate Fresnel factor for realistic reflection/refraction balance
            fresnel_factor = calculate_fresnel(ray.direction, normal,
                                              is_entering ? 1.0 : hit_object->material.refractive_index,
                                              is_entering ? hit_object->material.refractive_index : 1.0);

            // Calculate the refracted direction
            t_vec3 refract_dir = refract_ray(ray.direction, normal, ior_ratio);

            // If refraction occurs (not total internal reflection)
            if (refract_dir.x != 0.0 || refract_dir.y != 0.0 || refract_dir.z != 0.0)
            {
                t_ray refract_ray;

                // Create a refraction ray slightly offset from the hit point in the opposite direction
                // of the normal to ensure it starts inside the object
                refract_ray.origin = vec3_add(hit_point, vec3_scale(vec3_negate(normal), 0.001));
                refract_ray.direction = refract_dir;

                // Trace the refraction ray
                refracted_color = trace_ray_with_checkerboard(scene, refract_ray, depth + 1);
            }
            else
            {
                // Total internal reflection - use reflection instead
                fresnel_factor = 1.0;
                if (!reflected_color)
                {
                    t_vec3 reflect_dir = reflect_ray(ray.direction, normal);
                    t_ray reflect_ray;

                    reflect_ray.origin = vec3_add(hit_point, vec3_scale(normal, 0.001));
                    reflect_ray.direction = reflect_dir;

                    reflected_color = trace_ray_with_checkerboard(scene, reflect_ray, depth + 1);
                }
            }
        }

        // Blend base color, reflection and refraction based on material properties
        if (reflected_color || refracted_color)
        {
            double reflection_contribution;
            double refraction_contribution;

            // Adjust reflection contribution based on Fresnel factor for transparent objects
            if (hit_object->material.transparency > 0.0)
            {
                reflection_contribution = fresnel_factor;
                refraction_contribution = hit_object->material.transparency * (1.0 - fresnel_factor);
            }
            else
            {
                reflection_contribution = hit_object->material.reflectivity;
                refraction_contribution = 0.0;
            }

            // Ensure the base material still contributes to opaque parts
            double base_contribution = 1.0 - (reflection_contribution + refraction_contribution);

            // Extract RGB components
            int base_r = (base_color >> 16) & 0xFF;
            int base_g = (base_color >> 8) & 0xFF;
            int base_b = base_color & 0xFF;

            int reflect_r = reflected_color ? ((reflected_color >> 16) & 0xFF) : 0;
            int reflect_g = reflected_color ? ((reflected_color >> 8) & 0xFF) : 0;
            int reflect_b = reflected_color ? (reflected_color & 0xFF) : 0;

            int refract_r = refracted_color ? ((refracted_color >> 16) & 0xFF) : 0;
            int refract_g = refracted_color ? ((refracted_color >> 8) & 0xFF) : 0;
            int refract_b = refracted_color ? (refracted_color & 0xFF) : 0;

            // Blend the colors
            int final_r = (base_r * base_contribution) +
                          (reflect_r * reflection_contribution) +
                          (refract_r * refraction_contribution);
            int final_g = (base_g * base_contribution) +
                          (reflect_g * reflection_contribution) +
                          (refract_g * refraction_contribution);
            int final_b = (base_b * base_contribution) +
                          (reflect_b * reflection_contribution) +
                          (refract_b * refraction_contribution);

            return (valid_color_range(final_r) << 16) |
                   (valid_color_range(final_g) << 8) |
                   valid_color_range(final_b);
        }
    }

    return base_color;
}

/**
 * Main rendering thread function that handles ray tracing with reflections, refractions,
 * and checkerboard textures
 *
 * @param arg Thread data containing scene and thread-specific info
 * @return NULL when completed
 */
void *render_thread_with_checkerboard(void *arg)
{
    t_thread_data *data = (t_thread_data *)arg;
    t_scene *scene = data->scene;
    double fov_scale = tan(scene->camera.fov * M_PI / 360.0);
    t_ray ray;
    int color;

    for (int y = data->start_row; y < data->end_row; y += scene->app.resolution_factor)
    {
        for (int x = 0; x < scene->width; x += scene->app.resolution_factor)
        {
            // Initialize ray direction
            compute_ray_direction(scene, &ray, fov_scale, x, y);

            // Trace the ray
            color = trace_ray_with_checkerboard(scene, ray, 0);

            // Fill the entire block with this color
            for (int by = 0; by < scene->app.resolution_factor && y + by < HEIGHT; by++)
            {
                for (int bx = 0; bx < scene->app.resolution_factor && x + bx < WIDTH; bx++)
                {
                    pixel_put(x + bx, y + by, &scene->img, color);
                }
            }
        }
    }

    return NULL;
}
