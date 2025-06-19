#include "../../includes/miniRT.h"

// Debug version of refract_ray with additional safety checks
t_vec3 refract_ray(t_vec3 incident, t_vec3 normal, double eta_ratio, bool *total_internal_reflection)
{
    // Ensure vectors are normalized
    incident = vec3_normalize(incident);
    normal = vec3_normalize(normal);
    
    // Calculate cosine of angle between incident ray and normal
    // Use the incident ray direction as-is, normal should already be oriented correctly
    double cos_theta_i = -vec3_dot(incident, normal);
    
    // If cos_theta_i is negative, we need to flip the normal
    if (cos_theta_i < 0.0) {
        cos_theta_i = -cos_theta_i;
        normal = vec3_negate(normal);
    }
    
    // Clamp to handle numerical precision errors
    cos_theta_i = fmax(0.0, fmin(1.0, cos_theta_i));
    
    // Calculate discriminant for Snell's law
    double sin_theta_i_sq = 1.0 - cos_theta_i * cos_theta_i;
    double sin_theta_t_sq = eta_ratio * eta_ratio * sin_theta_i_sq;
    
    // Check for total internal reflection
    if (sin_theta_t_sq >= 1.0)
    {
        *total_internal_reflection = true;
        return vec3_create(0, 0, 0);
    }
    
    *total_internal_reflection = false;
    
    double cos_theta_t = sqrt(1.0 - sin_theta_t_sq);
    
    // Calculate refracted ray direction using vector form of Snell's law
    t_vec3 refracted = vec3_add(
        vec3_scale(incident, eta_ratio),
        vec3_scale(normal, eta_ratio * cos_theta_i - cos_theta_t)
    );
    
    return vec3_normalize(refracted);
}