/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   refraction.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 14:11:23 by abillote          #+#    #+#             */
/*   Updated: 2025/05/22 10:21:46 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "platform.h"

/**
 * Calculates a refracted ray direction according to Snell's law.
 *
 * @param incident The incident ray direction (should be normalized)
 * @param normal The surface normal at the intersection point (should be normalized)
 * @param ior_ratio The ratio of refractive indices (incident medium / transmission medium)
 * @return The direction of the refracted ray, or a zero vector if total internal reflection occurs
 */
t_vec3 refract_ray(t_vec3 incident, t_vec3 normal, double ior_ratio)
{
	double cos_i;
	double cos_t_squared;
	t_vec3 refracted;

	// Calculate cosine of angle between incident ray and normal
	cos_i = -vec3_dot(normal, incident);

	// Check if we're inside the object (incident ray and normal pointing in same direction)
	if (cos_i < 0)
	{
		// Flip the normal and cosine
		normal = vec3_negate(normal);
		cos_i = -vec3_dot(normal, incident);
	}

	// Calculate using Snell's law: n1 * sin(theta1) = n2 * sin(theta2)
	// Using trig identity: sin^2(theta) + cos^2(theta) = 1
	// We can derive cos(theta2) from these relations
	cos_t_squared = 1.0 - ior_ratio * ior_ratio * (1.0 - cos_i * cos_i);

	// Check for total internal reflection
	if (cos_t_squared < 0)
	{
		// Return zero vector to indicate total internal reflection
		return (t_vec3){0, 0, 0};
	}

	// Calculate the refracted direction using the formula:
	// T = n * I + (n * cos(i) - sqrt(cos(t)^2)) * N
	// Where n is the ratio of indices of refraction
	// I is the incident vector, N is the normal vector
	refracted.x = ior_ratio * incident.x + (ior_ratio * cos_i - sqrt(cos_t_squared)) * normal.x;
	refracted.y = ior_ratio * incident.y + (ior_ratio * cos_i - sqrt(cos_t_squared)) * normal.y;
	refracted.z = ior_ratio * incident.z + (ior_ratio * cos_i - sqrt(cos_t_squared)) * normal.z;

	// Normalize the refracted vector
	return vec3_normalize(refracted);
}

/**
 * Determine if a ray is entering or exiting a medium.
 *
 * @param ray_dir The ray direction
 * @param normal The surface normal
 * @return True if the ray is entering the medium, false if exiting
 */
int is_entering_medium(t_vec3 ray_dir, t_vec3 normal)
{
    // If dot product is negative, ray is entering the medium
    return (vec3_dot(ray_dir, normal) < 0);
}

/**
 * Calculate Fresnel coefficient to determine reflection vs. refraction ratio.
 * Uses Schlick's approximation of the Fresnel equations.
 *
 * @param incident The incident ray direction (normalized)
 * @param normal The surface normal (normalized)
 * @param ior_incident Index of refraction of the incident medium
 * @param ior_transmission Index of refraction of the transmission medium
 * @return The Fresnel coefficient (0.0 to 1.0) - higher means more reflection
 */
double calculate_fresnel(t_vec3 incident, t_vec3 normal,
                         double ior_incident, double ior_transmission)
{
    double cos_i;
    double r0;

    // Calculate cosine of angle between incident ray and normal
    cos_i = -vec3_dot(incident, normal);

    // If we're inside the object (incident ray and normal pointing in opposite directions)
    if (cos_i < 0)
    {
        // Flip normal and cosine
        cos_i = -cos_i;
    }

    // Calculate R0 (reflection coefficient for light incoming parallel to the normal)
    r0 = (ior_incident - ior_transmission) / (ior_incident + ior_transmission);
    r0 = r0 * r0;

    // Calculate Fresnel coefficient using Schlick's approximation
    return r0 + (1.0 - r0) * pow((1.0 - cos_i), 5);
}
