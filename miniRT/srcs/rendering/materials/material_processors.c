/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   material_processors.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 16:50:00 by antonsplavn      #+#    #+#             */
/*   Updated: 2025/07/16 16:50:00 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/miniRT.h"

t_vec3	process_reflection_refraction(t_ray_context context,
	t_material_contrib *contrib, t_fresnel_params fresnel_params)
{
	t_vec3				final_color;
	double				eta_ratio;
	t_refraction_params	refract_params;

	final_color = vec3_create(0, 0, 0);
	if (context.scene->graphic_settings.enable_reflections && \
		contrib->fresnel > 0.001)
		final_color = vec3_add(final_color, \
			add_reflection_contribution(context, contrib));
	contrib->transmission = 1.0 - contrib->fresnel;
	if (contrib->transmission > 0.001 && \
		context.hit_record.material.transparency > 0.001)
	{
		eta_ratio = fresnel_params.eta_i / fresnel_params.eta_t;
		refract_params.total_internal_reflection = false;
		refract_params.refract_dir = refract_ray(context.ray.direction, \
			context.hit_record.normal, eta_ratio, \
			&refract_params.total_internal_reflection);
		final_color = vec3_add(final_color, \
			add_refraction_contribution(context, contrib, refract_params));
	}
	return (final_color);
}

t_vec3	handle_refractive_material(t_ray_context context, double cos_theta)
{
	t_fresnel_params	fresnel_params;
	t_material_contrib	contrib;

	fresnel_params.eta_i = 1.0;
	fresnel_params.eta_t = context.hit_record.material.refractive_index;
	if (context.hit_record.inside)
	{
		fresnel_params.eta_i = context.hit_record.material.refractive_index;
		fresnel_params.eta_t = 1.0;
	}
	fresnel_params.cos_i = cos_theta;
	contrib.total_contrib = 0.0;
	contrib.fresnel = calculate_fresnel_reflectance(fresnel_params);
	contrib.transmission = 1.0 - contrib.fresnel;
	return (process_reflection_refraction(context, &contrib, fresnel_params));
}

t_vec3	handle_reflective_material(t_ray_context context, double cos_theta, \
	double *total_contrib)
{
	double	material_ior;
	double	fresnel;
	double	min_reflectivity;
	double	final_reflectivity;

	material_ior = 1.5;
	if (context.hit_record.material.reflectivity > 0.7)
		material_ior = 0.2;
	fresnel = calculate_fresnel_schlick(cos_theta, material_ior);
	min_reflectivity = context.hit_record.material.reflectivity * 0.5;
	final_reflectivity = min_reflectivity + \
		(context.hit_record.material.reflectivity - min_reflectivity) * fresnel;
	*total_contrib = final_reflectivity;
	return (vec3_scale(calculate_reflection(context.scene, context.ray, \
		context.hit_record, context.depth), final_reflectivity));
}
