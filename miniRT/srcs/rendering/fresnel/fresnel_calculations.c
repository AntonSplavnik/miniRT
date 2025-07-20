/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fresnel_calculations.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 16:50:00 by antonsplavn      #+#    #+#             */
/*   Updated: 2025/07/16 16:50:00 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/miniRT.h"

double	calculate_fresnel_reflectance(t_fresnel_params params)
{
	double	sin_t_sq;
	double	cos_t;
	double	rs;
	double	rp;

	sin_t_sq = (params.eta_i / params.eta_t) * (params.eta_i / params.eta_t) \
		* (1.0 - params.cos_i * params.cos_i);
	if (sin_t_sq >= 1.0)
		return (1.0);
	cos_t = sqrt(1.0 - sin_t_sq);
	rs = (params.eta_i * params.cos_i - params.eta_t * cos_t) \
		/ (params.eta_i * params.cos_i + params.eta_t * cos_t);
	rp = (params.eta_t * params.cos_i - params.eta_i * cos_t) \
		/ (params.eta_t * params.cos_i + params.eta_i * cos_t);
	return (0.5 * (rs * rs + rp * rp));
}

double	calculate_fresnel_schlick(double cos_theta, double ior)
{
	double	r0;

	r0 = pow((1.0 - ior) / (1.0 + ior), 2.0);
	return (r0 + (1.0 - r0) * pow(1.0 - cos_theta, 5.0));
}
