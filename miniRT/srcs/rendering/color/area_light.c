/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   area_light.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 16:00:00 by antonsplavn       #+#    #+#             */
/*   Updated: 2025/07/23 17:21:44 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/miniRT.h"

double	simple_random(void)
{
	return ((double)rand() / RAND_MAX);
}

t_vec3	sample_area_light_position(t_light *light)
{
	double	u;
	double	v;
	t_vec3	sample_point;
	t_vec3	u_offset;
	t_vec3	v_offset;

	u = (simple_random() - 0.5) * light->width;
	v = (simple_random() - 0.5) * light->height;
	u_offset = vec3_scale(light->u_axis, u);
	v_offset = vec3_scale(light->v_axis, v);
	sample_point = vec3_add(light->position, u_offset);
	sample_point = vec3_add(sample_point, v_offset);
	return (sample_point);
}

t_light_result	compute_light_at_position(t_scene *scene, t_hit_record hit_record, \
									t_vec3 light_pos)
{
	t_light_result	result;
	t_vec3			to_light;
	t_vec3			view_dir;
	t_vec3			reflect_dir;
	double			specular;

	to_light = vec3_subtract(light_pos, hit_record.point);
	result.light_distance = vec3_length(to_light);
	result.light_dir = vec3_normalize(to_light);
	result.diffuse = fmax(0.0, vec3_dot(hit_record.normal, result.light_dir));
	view_dir = vec3_normalize(vec3_subtract(scene->camera.position, \
		hit_record.point));
	reflect_dir = vec3_subtract(vec3_scale(hit_record.normal, 2.0 * \
		vec3_dot(result.light_dir, hit_record.normal)), result.light_dir);
	reflect_dir = vec3_normalize(reflect_dir);
	if (scene->graphic_settings.enable_specular)
	{
		specular = pow(fmax(0.0, vec3_dot(view_dir, reflect_dir)), \
			hit_record.object->material.shininess);
		result.specular_intensity = hit_record.object->material.specular * \
			specular;
	}
	return (result);
}

double	calculate_area_light_shadow(t_scene *scene, t_hit_record hit_record, \
								t_light *light)
{
	int		i;
	int		samples;
	double	shadow_factor;
	t_vec3	light_sample_pos;
	t_vec3	light_dir;
	double	light_distance;

	samples = 40;
	shadow_factor = 0.0;
	i = 0;
	while (i < samples)
	{
		light_sample_pos = sample_area_light_position(light);
		light_dir = vec3_normalize(vec3_subtract(light_sample_pos, \
			hit_record.point));
		light_distance = vec3_length(vec3_subtract(light_sample_pos, \
			hit_record.point));
		if (!is_in_shadow(scene, hit_record.point, light_dir, \
			light_distance, hit_record))
			shadow_factor += 1.0;
		i++;
	}
	return (shadow_factor / (double)samples);
}

t_light_result	compute_area_light(t_scene *scene, t_hit_record hit_record, \
								t_light *light)
{
	t_light_result	result;
	t_light_result	sample_result;
	t_vec3			light_sample_pos;
	int				samples;
	int				i;


	samples = 40;
	result.diffuse = 0.0;
	result.specular_intensity = 0.0;
	result.light_distance = 0.0;
	result.light_dir = vec3_create(0, 0, 0);
	i = 0;
	while (i < samples)
	{
		light_sample_pos = sample_area_light_position(light);
		sample_result = compute_light_at_position(scene, hit_record, \
			light_sample_pos);
		result.diffuse += sample_result.diffuse;
		result.specular_intensity += sample_result.specular_intensity;
		result.light_distance += sample_result.light_distance;
		result.light_dir = vec3_add(result.light_dir, sample_result.light_dir);
		i++;
	}
	result.diffuse /= (double)samples;
	result.specular_intensity /= (double)samples;
	result.light_distance /= (double)samples;
	result.light_dir = vec3_normalize(result.light_dir);
	return (result);
}
