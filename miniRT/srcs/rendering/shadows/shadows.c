/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shadows.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 03:16:40 by antonsplavn       #+#    #+#             */
/*   Updated: 2025/07/21 03:17:03 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

/*
** Process transparent object intersection and continue ray
*/
int	process_transparent_hit(t_shadow_data *data, t_object *hit_object, \
	double t)
{
	if (hit_object != data->last_object)
	{
		data->attenuation *= hit_object->material.transparency;
		data->last_object = hit_object;
	}
	if (data->attenuation < 0.001)
		return (0);
	data->shadow_ray.origin = vec3_add(data->shadow_ray.origin, \
		vec3_scale(data->shadow_ray.direction, t + 0.001));
	data->light_distance -= t;
	data->max_iterations--;
	return (1);
}

/*
** Calculate shadow attenuation considering transparency
*/
double	calculate_shadow_attenuation(t_scene *scene, t_shadow_params params)
{
	t_shadow_data	data;
	t_object		*hit_object;
	t_hit_record	hit_record;

	data.shadow_ray.origin = vec3_add(params.hit_point, \
		vec3_scale(params.surface_normal, 0.001));
	data.shadow_ray.direction = params.light_dir;
	data.attenuation = 1.0;
	data.light_distance = params.light_distance;
	data.max_iterations = 5;
	data.last_object = NULL;
	while (data.max_iterations > 0 && find_closest_intersection(scene, \
		data.shadow_ray, &hit_record, &hit_object) && \
		hit_record.t < data.light_distance)
	{
		if (hit_object->material.transparency > 0.001)
		{
			if (!process_transparent_hit(&data, hit_object, hit_record.t))
				return (0.0);
		}
		else
			return (0.0);
	}
	return (data.attenuation);
}
