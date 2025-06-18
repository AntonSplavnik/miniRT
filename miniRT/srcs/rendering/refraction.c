#include "../../includes/miniRT.h"

t_vec3 refract_ray(t_vec3 incident, t_vec3 normal, double eta_ratio, bool *total_internal_reflection)
{
    // Ensure incident ray is normalized
    incident = vec3_normalize(incident);
    
    // Calculate cosine of angle between incident ray and normal
    double cos_theta_i = -vec3_dot(incident, normal);
    
    // Calculate sin^2 of refracted angle using Snell's law
    double sin2_theta_t = eta_ratio * eta_ratio * (1.0 - cos_theta_i * cos_theta_i);
    
    // Check for total internal reflection
    if (sin2_theta_t > 1.0)
    {
        *total_internal_reflection = true;
        return vec3_create(0, 0, 0);
    }
    
    *total_internal_reflection = false;
    
    // Calculate cosine of refracted angle
    double cos_theta_t = sqrt(1.0 - sin2_theta_t);
    
    // Calculate refracted ray direction using Snell's law
    t_vec3 refracted = vec3_add(
        vec3_scale(incident, eta_ratio),
        vec3_scale(normal, eta_ratio * cos_theta_i - cos_theta_t)
    );
    
    return vec3_normalize(refracted);
}
