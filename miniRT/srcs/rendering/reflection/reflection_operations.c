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

t_vec3	calculate_reflection(t_ray_context context, double child_weight)
{
	t_vec3	reflect_dir;
	t_ray	reflect_ray_struct;

	reflect_dir = reflect_ray(context.ray.direction, context.hit_record.normal);
	reflect_ray_struct.origin = vec3_add(context.hit_record.point, \
		vec3_scale(context.hit_record.normal, 0.001));
	reflect_ray_struct.direction = reflect_dir;
	return (trace_ray(context.scene, reflect_ray_struct, \
		context.depth + 1, child_weight));
}

t_vec3	add_reflection_contribution(t_ray_context context, \
										t_material_contrib *contrib)
{
	t_vec3	reflect_color;
	double	reflect_contrib;

	reflect_contrib = contrib->fresnel * \
		context.hit_record.material.reflectivity;
	contrib->total_contrib += reflect_contrib;
	reflect_color = calculate_reflection(context, \
		context.weight * reflect_contrib);
	return (vec3_scale(reflect_color, reflect_contrib));
}
