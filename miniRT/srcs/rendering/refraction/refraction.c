/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   refraction.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/20 16:30:00 by abillote          #+#    #+#             */
/*   Updated: 2025/07/18 16:53:22 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

static double	calculate_cos_theta_i(t_vec3 incident, t_vec3 *normal)
{
	double	cos_theta_i;

	cos_theta_i = -vec3_dot(incident, *normal);
	if (cos_theta_i < 0.0)
	{
		cos_theta_i = -cos_theta_i;
		*normal = vec3_negate(*normal);
	}
	return (fmax(0.0, fmin(1.0, cos_theta_i)));
}

static bool	check_total_internal_reflection(double eta_ratio,
	double cos_theta_i)
{
	double	sin_theta_i_sq;
	double	sin_theta_t_sq;

	sin_theta_i_sq = 1.0 - cos_theta_i * cos_theta_i;
	sin_theta_t_sq = eta_ratio * eta_ratio * sin_theta_i_sq;
	return (sin_theta_t_sq >= 1.0);
}

static t_vec3	calculate_refracted_direction(t_vec3 incident, t_vec3 normal,
	double eta_ratio, double cos_theta_i)
{
	double	sin_theta_i_sq;
	double	sin_theta_t_sq;
	double	cos_theta_t;
	t_vec3	refracted;

	sin_theta_i_sq = 1.0 - cos_theta_i * cos_theta_i;
	sin_theta_t_sq = eta_ratio * eta_ratio * sin_theta_i_sq;
	cos_theta_t = sqrt(1.0 - sin_theta_t_sq);
	refracted = vec3_add(vec3_scale(incident, eta_ratio),
			vec3_scale(normal, eta_ratio * cos_theta_i - cos_theta_t));
	return (vec3_normalize(refracted));
}

t_vec3	refract_ray(t_vec3 incident, t_vec3 normal, double eta_ratio,
	bool *total_internal_reflection)
{
	double	cos_theta_i;

	incident = vec3_normalize(incident);
	normal = vec3_normalize(normal);
	cos_theta_i = calculate_cos_theta_i(incident, &normal);
	if (check_total_internal_reflection(eta_ratio, cos_theta_i))
	{
		*total_internal_reflection = true;
		return (vec3_create(0, 0, 0));
	}
	*total_internal_reflection = false;
	return (calculate_refracted_direction(incident, normal, eta_ratio,
			cos_theta_i));
}
