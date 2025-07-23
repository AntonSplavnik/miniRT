/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trace_rays.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 12:50:49 by abillote          #+#    #+#             */
/*   Updated: 2025/07/23 13:01:09 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

t_vec3	add_emissive_contribution(t_hit_record hit_record, t_vec3 direct_color)
{
	if (hit_record.material.emissive_intensity > 0.0)
	{
		direct_color.x += hit_record.material.emissive_color.r * \
			hit_record.material.emissive_intensity;
		direct_color.y += hit_record.material.emissive_color.g * \
			hit_record.material.emissive_intensity;
		direct_color.z += hit_record.material.emissive_color.b * \
			hit_record.material.emissive_intensity;
	}
	return (direct_color);
}

t_vec3	add_ambient_contribution(t_scene *scene, t_hit_record hit_record, \
	t_vec3 direct_color)
{
	t_color_f	ambient_linear;
	t_color_f	surface_color;

	ambient_linear = color_to_linear(scene->ambient.color);
	surface_color = get_surface_color_linear(&hit_record);
	direct_color.x += surface_color.r * ambient_linear.r * scene->ambient.ratio;
	direct_color.y += surface_color.g * ambient_linear.g * scene->ambient.ratio;
	direct_color.z += surface_color.b * ambient_linear.b * scene->ambient.ratio;
	return (direct_color);
}

t_vec3	add_direct_light_contribution(t_scene *scene, t_hit_record hit_record, \
	t_vec3 direct_color)
{
	t_light	*current_light;

	current_light = scene->lights;
	direct_color = process_lights(scene, hit_record, current_light, direct_color);
	return (direct_color);
}

t_vec3	add_global_illumination_contribution(t_scene *scene, \
	t_hit_record hit_record, t_vec3 direct_color, int is_gi_ray, int depth)
{
	t_vec3	gi_contrib;

	if (scene->graphic_settings.enable_global_illumination && !is_gi_ray)
	{
		gi_contrib = compute_indirect_lighting(scene, hit_record, depth);
		if (isfinite(gi_contrib.x) && isfinite(gi_contrib.y) && \
			isfinite(gi_contrib.z))
		{
			gi_contrib = vec3_scale(gi_contrib, 0.8);
			direct_color = vec3_add(direct_color, gi_contrib);
		}
	}
	return (direct_color);
}

t_vec3	compute_lighting(t_scene *scene, t_hit_record hit_record,
	double total_contribution, t_vec3 final_color, int is_gi_ray, int depth)
{
	double		direct_contribution;
	t_vec3		direct_color;

	direct_contribution = 1.0 - total_contribution;
	if (direct_contribution <= 0.001)
		return (final_color);
	direct_color = vec3_create(0, 0, 0);
	direct_color = add_emissive_contribution(hit_record, direct_color);
	direct_color = add_ambient_contribution(scene, hit_record, direct_color);
	direct_color = add_direct_light_contribution(scene, hit_record, direct_color);
	direct_color = add_global_illumination_contribution(scene, hit_record, direct_color, is_gi_ray, depth);
	return (vec3_add(final_color, vec3_scale(direct_color, direct_contribution)));
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
	final_color = compute_lighting(context.scene, context.hit_record, \
		total_contribution, final_color, context.ray.is_gi_ray, context.depth);
	return (final_color);
}

t_vec3	trace_ray(t_scene *scene, t_ray ray, int depth)
{
	t_hit_record	hit_record;
	t_object		*hit_object;
	double			t;
	double			cos_theta;
	t_ray_context	context;

	if (depth > MAX_RAY_DEPTH)
		return (vec3_create(0, 0, 0));
	if (!find_closest_intersection(scene, ray, &t, &hit_object, &hit_record))
		return (get_background_color(scene));
	compute_ray_intersection(ray, hit_object, t, &hit_record);
	cos_theta = fabs(vec3_dot(ray.direction, hit_record.normal));
	context.scene = scene;
	context.ray = ray;
	context.hit_record = hit_record;
	context.depth = depth;
	return (process_material_interaction(context, cos_theta));
}
