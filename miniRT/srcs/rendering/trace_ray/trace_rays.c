/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trace_rays.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 12:50:49 by abillote          #+#    #+#             */
/*   Updated: 2025/07/18 18:21:56 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

t_vec3	add_direct_lighting(t_scene *scene, t_hit_record hit_record,
	double total_contribution, t_vec3 final_color)
{
	double		direct_contribution;
	t_vec3		direct_color;
	t_color_f	ambient_linear;
	t_color_f	surface_color;
	t_light		*current_light;

	direct_contribution = 1.0 - total_contribution;
	if (direct_contribution <= 0.001)
		return (final_color);
	direct_color = vec3_create(0, 0, 0);
	ambient_linear = color_to_linear(scene->ambient.color);
	surface_color = get_surface_color_linear(&hit_record);
	direct_color.x += surface_color.r * ambient_linear.r * scene->ambient.ratio;
	direct_color.y += surface_color.g * ambient_linear.g * scene->ambient.ratio;
	direct_color.z += surface_color.b * ambient_linear.b * scene->ambient.ratio;
	current_light = scene->lights;
	direct_color = process_lights(scene, hit_record, current_light, \
		direct_color);
	return (vec3_add(final_color, vec3_scale(direct_color, \
		direct_contribution)));
}

t_vec3	process_lights(t_scene *scene, t_hit_record hit_record,
	t_light *current_light, t_vec3 direct_color)
{
	double			shadow_attenuation;
	t_light_params	params;

	while (current_light)
	{
		shadow_attenuation = get_light_shadow_attenuation(scene, hit_record, \
			current_light);
		if (shadow_attenuation > 0.001)
		{
			params.scene = scene;
			params.hit_record = hit_record;
			params.light = current_light;
			params.shadow_attenuation = shadow_attenuation;
			params.direct_color = direct_color;
			direct_color = apply_light_contribution(params);
		}
		current_light = current_light->next;
	}
	return (direct_color);
}

t_vec3	process_material_interaction(t_ray_context context, double cos_theta)
{
	t_vec3	final_color;
	double	total_contribution;

	final_color = vec3_create(0, 0, 0);
	total_contribution = 0.0;
	if (context.scene->graphic_settings.enable_refraction && \
		context.hit_record.material.has_refraction && \
		context.hit_record.material.transparency > 0.0)
	{
		final_color = handle_refractive_material(context, cos_theta);
		total_contribution = 1.0;
	}
	else if (context.scene->graphic_settings.enable_reflections && \
		context.hit_record.material.reflectivity > 0.001)
	{
		final_color = handle_reflective_material(context, cos_theta, \
			&total_contribution);
	}
	final_color = add_direct_lighting(context.scene, context.hit_record, \
		total_contribution, final_color);
	return (final_color);
}

t_vec3	trace_ray(t_scene *scene, t_ray ray, int depth)
{
	t_hit_record	hit_record;
	t_object		*hit_object;
	double			cos_theta;
	t_ray_context	context;

	if (depth > MAX_RAY_DEPTH)
		return (vec3_create(0, 0, 0));
	if (!find_closest_intersection(scene, ray, &hit_record, &hit_object))
		return (get_background_color(scene));
	compute_ray_intersection(ray, hit_object, &hit_record);
	cos_theta = fabs(vec3_dot(ray.direction, hit_record.normal));
	context.scene = scene;
	context.ray = ray;
	context.hit_record = hit_record;
	context.depth = depth;
	return (process_material_interaction(context, cos_theta));
}
