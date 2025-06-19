/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trace_rays.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 14:40:00 by abillote          #+#    #+#             */
/*   Updated: 2025/06/03 11:24:04 by abillote         ###   ########.fr       */
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
// Fresnel equations to calculate reflection/transmission coefficients
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



// Main trace_ray function with refraction support
int trace_ray(t_scene *scene, t_ray ray, int depth)
{
    // Prevent infinite recursion
    if (depth > scene->max_depth)
        return scene->background_color;
    
    t_hit_record hit_record;
    t_object *hit_object;
    double t;
    
    // Check for intersection
    if (!find_closest_intersection(scene, ray, &t, &hit_object, &hit_record))
        return scene->background_color;
    
    // Compute intersection details
    compute_ray_intersection(ray, hit_object, t, &hit_record);
    
    // Initialize color accumulation
    int final_color = 0;
    double total_contribution = 0.0;
    
    // Calculate direct lighting (diffuse + specular)
    if (!scene->graphic_settings.enable_reflections && 
        (!scene->graphic_settings.enable_refraction || !hit_record.material.has_refraction))
    {
        // Simple lighting calculation for non-reflective/non-refractive materials
        t_light *current_light = scene->lights;
        while (current_light)
        {
            if (!is_in_shadow(scene, hit_record.point, 
                vec3_normalize(vec3_subtract(current_light->position, hit_record.point)),
                vec3_length(vec3_subtract(current_light->position, hit_record.point)), hit_record))
            {
                t_light_result light_result = compute_light(scene, hit_record, current_light);
                double light_intensity = light_result.diffuse * current_light->intensity;
                
                int light_color = get_pixel_color(&hit_record, light_intensity, 
                    current_light->color, light_result.specular_intensity);
                final_color = add_colors(final_color, light_color);
            }
            current_light = current_light->next;
        }
        return final_color;
    }
    
    // Handle materials with refraction
    if (scene->graphic_settings.enable_refraction && 
        hit_record.material.has_refraction && 
        hit_record.material.transparency > 0.0)
    {
        // Determine refractive indices
        double eta_i = 1.0; // air
        double eta_t = hit_record.material.refractive_index;
        
        // If we're inside the object, swap the indices
        if (hit_record.inside)
        {
            eta_i = hit_record.material.refractive_index;
            eta_t = 1.0;
        }
        
        double eta_ratio = eta_i / eta_t;
        
        // Calculate Fresnel reflectance
        double cos_theta_i = fabs(vec3_dot(ray.direction, hit_record.normal));
        double fresnel = fresnel_reflectance(cos_theta_i, eta_i, eta_t);
        
        // Calculate reflection contribution
        if (scene->graphic_settings.enable_reflections && fresnel > 0.001)
        {
            t_vec3 reflect_dir = reflect_ray(ray.direction, hit_record.normal);
            t_ray reflect_ray;
            reflect_ray.origin = vec3_add(hit_record.point, vec3_scale(hit_record.normal, 0.001));
            reflect_ray.direction = reflect_dir;
            
            int reflect_color = trace_ray(scene, reflect_ray, depth + 1);
            final_color = blend_colors(final_color, reflect_color, 
                total_contribution, fresnel * hit_record.material.reflectivity);
            total_contribution += fresnel * hit_record.material.reflectivity;
        }
        
        // Calculate refraction contribution
        double transmission = 1.0 - fresnel;
        if (transmission > 0.001 && hit_record.material.transparency > 0.001)
        {
            bool total_internal_reflection = false;
            t_vec3 refract_dir = refract_ray(ray.direction, hit_record.normal, eta_ratio, &total_internal_reflection);
            
            if (!total_internal_reflection)
            {
                t_ray refract_ray;
                // Offset ray origin slightly in the direction of refraction to avoid self-intersection
                refract_ray.origin = vec3_add(hit_record.point, vec3_scale(refract_dir, 0.001));
                refract_ray.direction = refract_dir;
                
                int refract_color = trace_ray(scene, refract_ray, depth + 1);
                final_color = blend_colors(final_color, refract_color, 
                    total_contribution, transmission * hit_record.material.transparency);
                total_contribution += transmission * hit_record.material.transparency;
            }
            else
            {
                // Total internal reflection - use perfect reflection (100% of transmission energy)
                t_vec3 reflect_dir = reflect_ray(ray.direction, hit_record.normal);
                t_ray reflect_ray;
                reflect_ray.origin = vec3_add(hit_record.point, vec3_scale(hit_record.normal, 0.001));
                reflect_ray.direction = reflect_dir;
                
                int reflect_color = trace_ray(scene, reflect_ray, depth + 1);
                final_color = blend_colors(final_color, reflect_color, 
                    total_contribution, transmission * hit_record.material.transparency);
                total_contribution += transmission * hit_record.material.transparency;
            }
        }
    }
    // Handle purely reflective materials
    else if (scene->graphic_settings.enable_reflections && 
             hit_record.material.reflectivity > 0.001)
    {
        t_vec3 reflect_dir = reflect_ray(ray.direction, hit_record.normal);
        t_ray reflect_ray;
        reflect_ray.origin = vec3_add(hit_record.point, vec3_scale(hit_record.normal, 0.001));
        reflect_ray.direction = reflect_dir;
        
        int reflect_color = trace_ray(scene, reflect_ray, depth + 1);
        final_color = blend_colors(final_color, reflect_color, 
            total_contribution, hit_record.material.reflectivity);
        total_contribution += hit_record.material.reflectivity;
    }
    
    // Add direct lighting contribution for remaining light
    double direct_contribution = 1.0 - total_contribution;
    if (direct_contribution > 0.001)
    {
        int direct_color = 0;
        t_light *current_light = scene->lights;
        while (current_light)
        {
            if (!is_in_shadow(scene, hit_record.point, 
                vec3_normalize(vec3_subtract(current_light->position, hit_record.point)),
                vec3_length(vec3_subtract(current_light->position, hit_record.point)), hit_record))
            {
                t_light_result light_result = compute_light(scene, hit_record, current_light);
                double light_intensity = light_result.diffuse * current_light->intensity;
                
                int light_color = get_pixel_color(&hit_record, light_intensity, 
                    current_light->color, light_result.specular_intensity);
                direct_color = add_colors(direct_color, light_color);
            }
            current_light = current_light->next;
        }
        
        final_color = blend_colors(final_color, direct_color, total_contribution, direct_contribution);
    }
    
    return final_color;
}