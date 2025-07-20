/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   refraction_operations.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 16:50:00 by antonsplavn      #+#    #+#             */
/*   Updated: 2025/07/16 16:50:00 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/miniRT.h"

t_vec3	handle_total_internal_reflection(t_ray_context context,
	t_material_contrib *contrib)
{
	t_vec3	reflect_color;
	double	reflect_contrib;

	reflect_color = calculate_reflection(context.scene, context.ray, \
		context.hit_record, context.depth);
	reflect_contrib = contrib->transmission * \
		context.hit_record.material.transparency;
	contrib->total_contrib += reflect_contrib;
	return (vec3_scale(reflect_color, reflect_contrib));
}

t_vec3	process_refraction(t_ray_context context,
	t_material_contrib *contrib, t_vec3 refract_dir)
{
	t_ray	refract_ray_struct;
	t_vec3	refract_color;
	double	refract_contrib;

	refract_ray_struct.origin = vec3_add(context.hit_record.point, \
		vec3_scale(refract_dir, 0.001));
	refract_ray_struct.direction = refract_dir;
	refract_color = trace_ray(context.scene, refract_ray_struct, \
		context.depth + 1);
	refract_contrib = contrib->transmission * \
		context.hit_record.material.transparency;
	contrib->total_contrib += refract_contrib;
	return (vec3_scale(refract_color, refract_contrib));
}

t_vec3	add_refraction_contribution(t_ray_context context,
	t_material_contrib *contrib, t_refraction_params refract_params)
{
	if (!refract_params.total_internal_reflection)
		return (process_refraction(context, contrib, \
			refract_params.refract_dir));
	else
		return (handle_total_internal_reflection(context, contrib));
}
