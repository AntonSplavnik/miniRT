/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reflection_operations.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 16:50:00 by antonsplavn      #+#    #+#             */
/*   Updated: 2025/07/16 16:50:00 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/miniRT.h"

t_vec3	calculate_reflection(t_scene *scene, t_ray ray, \
								t_hit_record hit_record, int depth)
{
	t_vec3	reflect_dir;
	t_ray	reflect_ray_struct;

	reflect_dir = reflect_ray(ray.direction, hit_record.normal);
	reflect_ray_struct.origin = vec3_add(hit_record.point, \
		vec3_scale(hit_record.normal, 0.001));
	reflect_ray_struct.direction = reflect_dir;
	return (trace_ray(scene, reflect_ray_struct, depth + 1));
}

t_vec3	add_reflection_contribution(t_ray_context context, \
										t_material_contrib *contrib)
{
	t_vec3	reflect_color;
	double	reflect_contrib;

	reflect_color = calculate_reflection(context.scene, context.ray, \
		context.hit_record, context.depth);
	reflect_contrib = contrib->fresnel * \
		context.hit_record.material.reflectivity;
	contrib->total_contrib += reflect_contrib;
	return (vec3_scale(reflect_color, reflect_contrib));
}
