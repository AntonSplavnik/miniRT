/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trace_ray_helpers.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 18:30:00 by antonsplavn      #+#    #+#             */
/*   Updated: 2025/07/18 18:30:00 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

double	get_light_shadow_attenuation(t_scene *scene, t_hit_record hit_record,
	t_light *light)
{
	t_shadow_params	params;

	if (!scene->graphic_settings.enable_hard_shadows)
		return (1.0);
	params.hit_point = hit_record.point;
	params.light_dir = vec3_normalize(vec3_subtract(light->position, \
		hit_record.point));
	params.light_distance = vec3_length(vec3_subtract(light->position, \
		hit_record.point));
	params.surface_normal = hit_record.normal;
	return (calculate_shadow_attenuation(scene, params));
}

t_vec3	apply_light_contribution(t_light_params params)
{
	t_light_result	light_result;
	double			light_intensity;
	t_color_f		light_color_linear;
	t_color_f		lit_color;

	light_result = compute_light(params.scene, params.hit_record, \
		params.light);
	light_intensity = light_result.diffuse * params.light->intensity * \
		params.shadow_attenuation;
	light_color_linear = color_to_linear(params.light->color);
	lit_color = get_pixel_color_linear(&params.hit_record, light_intensity, \
		light_color_linear, light_result.specular_intensity);
	params.direct_color.x += lit_color.r;
	params.direct_color.y += lit_color.g;
	params.direct_color.z += lit_color.b;
	return (params.direct_color);
}
