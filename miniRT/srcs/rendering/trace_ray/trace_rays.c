/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trace_rays.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/20 12:50:49 by abillote          #+#    #+#             */
/*   Updated: 2025/07/18 13:30:26 by antonsplavn      ###   ########.fr       */
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
	t_light_result	light_result;
	double			light_intensity;
	t_color_f		light_color_linear;
	t_color_f		lit_color;

	while (current_light)
	{
		if (!scene->graphic_settings.enable_hard_shadows || \
			!is_in_shadow(scene, hit_record.point, \
			vec3_normalize(vec3_subtract(current_light->position, \
			hit_record.point)), vec3_length(vec3_subtract(\
			current_light->position, hit_record.point)), hit_record))
		{
			light_result = compute_light(scene, hit_record, current_light);
			light_intensity = light_result.diffuse * current_light->intensity;
			light_color_linear = color_to_linear(current_light->color);
			lit_color = get_pixel_color_linear(&hit_record, light_intensity, \
				light_color_linear, light_result.specular_intensity);
			direct_color.x += lit_color.r;
			direct_color.y += lit_color.g;
			direct_color.z += lit_color.b;
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
		final_color = handle_reflective_material(context, cos_theta,
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
