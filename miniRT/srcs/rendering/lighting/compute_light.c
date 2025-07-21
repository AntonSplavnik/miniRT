/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compute_light.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 11:18:55 by abillote          #+#    #+#             */
/*   Updated: 2025/07/20 21:16:46 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

t_light_result	compute_light(t_scene *scene, t_hit_record hit_record, \
								t_light *light)
{
	t_light_result	result;
	t_vec3			to_light;
	t_vec3			view_dir;
	t_vec3			reflect_dir;
	double			specular;

	to_light = vec3_subtract(light->position, hit_record.point);
	result.light_distance = vec3_length(to_light);
	result.light_dir = vec3_normalize(to_light);
	result.diffuse = fmax(0.0, vec3_dot(hit_record.normal, result.light_dir));
	result.specular_intensity = 0.0;
	view_dir = vec3_normalize(vec3_subtract(scene->camera.position, \
		hit_record.point));
	reflect_dir = vec3_subtract(vec3_scale(hit_record.normal, 2.0 * \
		vec3_dot(result.light_dir, hit_record.normal)), result.light_dir);
	reflect_dir = vec3_normalize(reflect_dir);
	if (scene->graphic_settings.enable_specular)
	{
		specular = pow (fmax(0.0, vec3_dot(view_dir, reflect_dir)), \
							hit_record.object->material.shininess);
		result.specular_intensity = hit_record.object->material.specular * \
		specular;
	}
	return (result);
}

/* 	Calculate specular component with improved Blinn-Phong model
	result.specular_intensity = 0.0;
	if(scene->graphic_settings.enable_specular)
	{
		// Calculate half-vector between view direction and light direction
		t_vec3 half_vec = vec3_add(view_dir, result.light_dir);
		half_vec = vec3_normalize(half_vec);

		// Calculate specular using the half-vector (Blinn-Phong)
		double spec_angle = fmax(0.0, vec3_dot(hit_record.normal, half_vec));
		double specular = pow(spec_angle, hit_record.object->material.shininess);

		// Apply light falloff and material specular coefficient
		result.specular_intensity = hit_record.object->material.specular * \
		specular;

		// Increase specular intensity for more visible highlights
		result.specular_intensity *= 1.0;

		// // Apply minimal distance attenuation to specular highlights
		double distance_factor = 1.0 / (1.0 + 0.001 * result.light_distance * \
		result.light_distance);
		result.specular_intensity *= distance_factor;
	} */
