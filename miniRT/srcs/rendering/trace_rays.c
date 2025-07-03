/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trace_rays.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 14:40:00 by abillote          #+#    #+#             */
/*   Updated: 2025/07/02 23:02:41 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

/**
 * @brief Primary ray tracing function that recursively traces rays through a 3D scene
 *
 * This function implements the core ray tracing algorithm with support for:
 * - Primary ray-object intersection testing
 * - Phong shading model (ambient, diffuse, specular lighting)
 * - Hard shadow calculations
 * - Recursive reflections with material-based reflectivity
 * - Checkerboard texture mapping for enhanced visual appeal
 * - Proper color blending and accumulation from multiple light sources
 *
 * The algorithm follows these main steps:
 * 1. Test for ray-object intersections and find the closest hit
 * 2. Calculate intersection point and surface normal
 * 3. Apply ambient lighting as base illumination
 * 4. Iterate through all light sources and calculate their contributions:
 *    - Compute diffuse and specular lighting components
 *    - Perform shadow testing if hard shadows are enabled
 *    - Apply checkerboard texturing if the material supports it
 * 5. Handle recursive reflections if the material is reflective
 * 6. Blend reflection contributions with the base color
 *
 * Recursion is controlled by MAX_RAY_DEPTH to prevent infinite loops in
 * scenes with multiple reflective surfaces.
 *
 * @param scene Pointer to the scene structure containing:
 *              - All geometric objects (spheres, planes, cylinders)
 *              - Light sources with positions, colors, and intensities
 *              - Camera configuration and ambient lighting settings
 *              - Application settings (shadows, reflections, etc.)
 * @param ray   The ray to trace, containing:
 *              - origin: Starting point of the ray in world coordinates
 *              - direction: Normalized direction vector of the ray
 * @param depth Current recursion depth (0 for primary rays, incremented for reflections)
 *              Used to limit recursive reflection calculations and prevent stack overflow
 *
 * @return Integer color value in RGB format (0xRRGGBB) representing the final
 *         computed color along the ray path. Returns background_color if no
 *         intersection is found or maximum depth is exceeded.
 *
 * @note This function is computationally intensive and is typically called
 *       millions of times per rendered frame. Performance optimizations include
 *       early termination conditions and efficient intersection algorithms.
 */
int trace_ray(t_scene *scene, t_ray ray, int depth)
{
	double t;
	t_hit_record hit_record;
	t_light *current_light;
	t_light_result light_info;
	int in_shadow;
	int base_color;
	int reflected_color = 0;

	// if (depth > MAX_RAY_DEPTH || !find_closest_intersection(scene, ray, &t, &hit_record.object, &hit_record))
	// 	return scene->background_color;
	// compute_ray_intersection(ray, hit_record.object, t, &hit_record);

	if (depth > MAX_RAY_DEPTH || !find_closest_intersection(scene, ray, &t, &hit_record.object, &hit_record))
		return scene->background_color;
	compute_ray_intersection(ray, hit_record.object, t, &hit_record);

	// Start with ambient light
	base_color = get_pixel_color(&hit_record, scene->ambient.ratio, scene->ambient.color, 0.0);

	// Add contribution from all lights
	current_light = scene->lights;
	while (current_light)
	{
		light_info = compute_light(scene, hit_record, current_light);

		// Check if the hit point is in shadow for this light
		if (scene->graphic_settings.enable_hard_shadows)
			in_shadow = is_in_shadow(scene, hit_record.point, light_info.light_dir, light_info.light_distance, hit_record);
		else
			in_shadow = 0;
		if (!in_shadow)
		{
			// Calculate light intensity
			double diffuse_intensity = current_light->intensity * light_info.diffuse;
			double specular_intensity = current_light->intensity * light_info.specular_intensity;

			// Get color with both diffuse and specular components
			int light_color = get_pixel_color(
				&hit_record,
				diffuse_intensity, // Diffuse + ambient
				current_light->color,
				specular_intensity
			);

            // Add light contribution to base color
            base_color = add_colors(base_color, light_color);
        }

        current_light = current_light->next;
    }

    // Handle reflections if enabled and not at max depth
    if (depth < MAX_RAY_DEPTH)
    {
        // Calculate reflection if object is reflective
        if (scene->graphic_settings.enable_reflections && hit_record.object->material.reflectivity > 0.0)
        {
            t_vec3 reflect_dir = reflect_ray(ray.direction, hit_record.normal);
            t_ray reflect_ray;

            // Create a reflection ray slightly offset from the hit point
            reflect_ray.origin = vec3_add(hit_record.point, vec3_scale(hit_record.normal, 0.001));
            reflect_ray.direction = reflect_dir;

            // Trace the reflection ray
            reflected_color = trace_ray(scene, reflect_ray, depth + 1);
        }

        // Blend base color, reflection and refraction based on material properties
        if (reflected_color)
        {
            double reflection_contribution = hit_record.object->material.reflectivity;
            double base_contribution = 1.0 - reflection_contribution;

            // Blend base color with reflection using the new utility function
            return blend_colors(base_color, reflected_color, base_contribution, reflection_contribution);
        }
    }

    return base_color;
}
