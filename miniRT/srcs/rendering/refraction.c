#include "../../includes/miniRT.h"

// Improved refract_ray function with better numerical stability
t_vec3 refract_ray(t_vec3 incident, t_vec3 normal, double eta_ratio, bool *total_internal_reflection)
{
    // Ensure vectors are normalized
    incident = vec3_normalize(incident);
    normal = vec3_normalize(normal);
    
    // Calculate cosine of angle between incident ray and normal
    double cos_theta_i = vec3_dot(vec3_negate(incident), normal);
    
    // Ensure we're working with the correct normal direction
    if (cos_theta_i < 0.0) {
        cos_theta_i = -cos_theta_i;
        normal = vec3_negate(normal);
    }
    
    // Calculate discriminant for Snell's law
    double discriminant = 1.0 - eta_ratio * eta_ratio * (1.0 - cos_theta_i * cos_theta_i);
    
    // Check for total internal reflection
    if (discriminant < 0.0)
    {
        *total_internal_reflection = true;
        return vec3_create(0, 0, 0);
    }
    
    *total_internal_reflection = false;
    
    // Calculate refracted ray direction using Snell's law
    double cos_theta_t = sqrt(discriminant);
    
    // Combine the parallel and perpendicular components to get the refracted direction
    t_vec3 refracted = vec3_add(
        vec3_scale(incident, eta_ratio),
        vec3_scale(normal, eta_ratio * cos_theta_i - cos_theta_t)
    );
    
    return vec3_normalize(refracted);
}