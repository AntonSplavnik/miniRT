/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trace_rays.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 14:40:00 by abillote          #+#    #+#             */
/*   Updated: 2025/06/20 12:50:49 by abillote         ###   ########.fr       */
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

// First, you need to add refraction properties to your material structure
// Add these fields to your t_material structure:
/*
typedef struct s_material {
    // ... existing fields ...
    double transparency;     // 0.0 = opaque, 1.0 = fully transparent
    double refractive_index; // e.g., 1.0 = air, 1.33 = water, 1.5 = glass
    bool has_refraction;     // enable/disable refraction for this material
} t_material;
*/


// Add these to colors.c or create a new fresnel.c file
double fresnel_reflectance(double cos_theta_i, double eta_i, double eta_t)
{
    double sin_theta_t_sq = (eta_i / eta_t) * (eta_i / eta_t) * (1.0 - cos_theta_i * cos_theta_i);

    // Total internal reflection
    if (sin_theta_t_sq >= 1.0)
        return 1.0;

    double cos_theta_t = sqrt(1.0 - sin_theta_t_sq);

    // Fresnel equations for s and p polarized light
    double rs = (eta_i * cos_theta_i - eta_t * cos_theta_t) / (eta_i * cos_theta_i + eta_t * cos_theta_t);
    double rp = (eta_t * cos_theta_i - eta_i * cos_theta_t) / (eta_t * cos_theta_i + eta_i * cos_theta_t);

    // Average of s and p polarized reflectance
    return 0.5 * (rs * rs + rp * rp);
}

double fresnel_schlick(double cos_theta, double ior)
{
    double r0 = pow((1.0 - ior) / (1.0 + ior), 2.0);
    return r0 + (1.0 - r0) * pow(1.0 - cos_theta, 5.0);
}

t_vec3 trace_ray(t_scene *scene, t_ray ray, int depth)
{
    // Prevent infinite recursion
    if (depth > scene->max_depth)
        return vec3_create(0, 0, 0);  // Return black as vec3

    t_hit_record hit_record;
    t_object *hit_object;
    double t;

    // Check for intersection
    if (!find_closest_intersection(scene, ray, &t, &hit_object, &hit_record))
    {
        // Convert background color to linear vec3
        t_color bg_color = int_to_color(scene->background_color);
        t_color_f bg_linear = color_to_linear(bg_color);
        return color_f_to_vec3(bg_linear);
    }

    // Compute intersection details
    compute_ray_intersection(ray, hit_object, t, &hit_record);

    // Initialize color accumulation
    t_vec3 final_color = vec3_create(0, 0, 0);
    double total_contribution = 0.0;

    // Calculate viewing angle for Fresnel effect
    double cos_theta = fabs(vec3_dot(ray.direction, hit_record.normal));

    // Handle materials with refraction AND Fresnel
    if (scene->graphic_settings.enable_refraction &&
        hit_record.material.has_refraction &&
        hit_record.material.transparency > 0.0)
    {
        // Determine refractive indices
        double eta_i = 1.0; // air
        double eta_t = hit_record.material.refractive_index;

        if (hit_record.inside)
        {
            eta_i = hit_record.material.refractive_index;
            eta_t = 1.0;
        }

        // Calculate Fresnel reflectance - use the full equation for refractive materials
        double fresnel = fresnel_reflectance(cos_theta, eta_i, eta_t);

        // Calculate reflection contribution with Fresnel
        if (scene->graphic_settings.enable_reflections && fresnel > 0.001)
        {
            t_vec3 reflect_dir = reflect_ray(ray.direction, hit_record.normal);
            t_ray reflect_ray_struct;
            reflect_ray_struct.origin = vec3_add(hit_record.point, vec3_scale(hit_record.normal, 0.001));
            reflect_ray_struct.direction = reflect_dir;

            t_vec3 reflect_color = trace_ray(scene, reflect_ray_struct, depth + 1);
            double reflect_contrib = fresnel * hit_record.material.reflectivity;
            final_color = vec3_add(final_color, vec3_scale(reflect_color, reflect_contrib));
            total_contribution += reflect_contrib;
        }

        // Calculate refraction contribution
        double transmission = 1.0 - fresnel; // Fresnel determines transmission too
        if (transmission > 0.001 && hit_record.material.transparency > 0.001)
        {
            double eta_ratio = eta_i / eta_t;
            bool total_internal_reflection = false;
            t_vec3 refract_dir = refract_ray(ray.direction, hit_record.normal, eta_ratio, &total_internal_reflection);

            if (!total_internal_reflection)
            {
                t_ray refract_ray_struct;
                refract_ray_struct.origin = vec3_add(hit_record.point, vec3_scale(refract_dir, 0.001));
                refract_ray_struct.direction = refract_dir;

                t_vec3 refract_color = trace_ray(scene, refract_ray_struct, depth + 1);
                double refract_contrib = transmission * hit_record.material.transparency;
                final_color = vec3_add(final_color, vec3_scale(refract_color, refract_contrib));
                total_contribution += refract_contrib;
            }
            else
            {
                // Total internal reflection - use perfect reflection
                t_vec3 reflect_dir = reflect_ray(ray.direction, hit_record.normal);
                t_ray reflect_ray_struct;
                reflect_ray_struct.origin = vec3_add(hit_record.point, vec3_scale(hit_record.normal, 0.001));
                reflect_ray_struct.direction = reflect_dir;

                t_vec3 reflect_color = trace_ray(scene, reflect_ray_struct, depth + 1);
                double reflect_contrib = transmission * hit_record.material.transparency;
                final_color = vec3_add(final_color, vec3_scale(reflect_color, reflect_contrib));
                total_contribution += reflect_contrib;
            }
        }
    }
    // Handle purely reflective materials (like metals) with Fresnel
    else if (scene->graphic_settings.enable_reflections &&
             hit_record.material.reflectivity > 0.001)
    {
        // For metals, use a simpler Fresnel model or material-specific IOR
        double material_ior = 1.5; // Default for glass-like materials

        // For metals, you might want different behavior:
        // Metals have complex refractive indices, but we can approximate
        if (hit_record.material.reflectivity > 0.7) // Assume metallic if highly reflective
        {
            // Metals have less pronounced Fresnel effect
            material_ior = 0.2; // This gives less dramatic Fresnel variation
        }

        double fresnel = fresnel_schlick(cos_theta, material_ior);

        // Apply the quick fix for purely reflective materials:
        // Reduce Fresnel impact - never let reflectivity drop below 50% of original
        double min_reflectivity = hit_record.material.reflectivity * 0.5;
        double max_reflectivity = hit_record.material.reflectivity;
        double final_reflectivity = min_reflectivity + (max_reflectivity - min_reflectivity) * fresnel;

        t_vec3 reflect_dir = reflect_ray(ray.direction, hit_record.normal);
        t_ray reflect_ray_struct;
        reflect_ray_struct.origin = vec3_add(hit_record.point, vec3_scale(hit_record.normal, 0.001));
        reflect_ray_struct.direction = reflect_dir;

        t_vec3 reflect_color = trace_ray(scene, reflect_ray_struct, depth + 1);

        // Use the modified reflectivity with Fresnel
        double reflect_contrib = final_reflectivity;
        final_color = vec3_add(final_color, vec3_scale(reflect_color, reflect_contrib));
        total_contribution += reflect_contrib;
    }

    // Add direct lighting contribution for remaining light
    double direct_contribution = 1.0 - total_contribution;
    if (direct_contribution > 0.001)
    {
        t_vec3 direct_color = vec3_create(0, 0, 0);

        // Add ambient lighting
        t_color_f ambient_linear = color_to_linear(scene->ambient.color);
        t_color_f surface_color = get_surface_color_linear(&hit_record);

        direct_color.x += surface_color.r * ambient_linear.r * scene->ambient.ratio;
        direct_color.y += surface_color.g * ambient_linear.g * scene->ambient.ratio;
        direct_color.z += surface_color.b * ambient_linear.b * scene->ambient.ratio;

        // Process each light source
        t_light *current_light = scene->lights;
        while (current_light)
        {
            if (!scene->graphic_settings.enable_hard_shadows ||
                !is_in_shadow(scene, hit_record.point,
                    vec3_normalize(vec3_subtract(current_light->position, hit_record.point)),
                    vec3_length(vec3_subtract(current_light->position, hit_record.point)), hit_record))
            {
                t_light_result light_result = compute_light(scene, hit_record, current_light);
                double light_intensity = light_result.diffuse * current_light->intensity;

                t_color_f light_color_linear = color_to_linear(current_light->color);
                t_color_f lit_color = get_pixel_color_linear(&hit_record, light_intensity,
                    light_color_linear, light_result.specular_intensity);

                direct_color.x += lit_color.r;
                direct_color.y += lit_color.g;
                direct_color.z += lit_color.b;
            }
            current_light = current_light->next;
        }

        final_color = vec3_add(final_color, vec3_scale(direct_color, direct_contribution));
    }

    return final_color;
}
