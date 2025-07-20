/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bump_normal.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 11:09:53 by abillote          #+#    #+#             */
/*   Updated: 2025/07/20 22:40:47 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

static void	get_tangent_vectors_by_type(t_hit_record *hit_record,
										t_vec3 *tangent, t_vec3 *bitangent)
{
	if (hit_record->object->type == SPHERE)
		calculate_sphere_tangent_vectors(hit_record, tangent, bitangent);
	else if (hit_record->object->type == PLANE)
		calculate_plane_tangent_vectors(hit_record, tangent, bitangent);
	else if (hit_record->object->type == CYLINDER)
		calculate_cylinder_tangent_vectors(hit_record, tangent, bitangent);
	else if (hit_record->object->type == CUBE)
		calculate_cube_tangent_vectors(hit_record, tangent, bitangent);
	else
		calculate_tangent_vectors(hit_record->normal, tangent, bitangent);
}

static void	sample_bump_heights(t_material *material, t_hit_record *hit_record,
							t_bump_heights *heights)
{
	heights->du = 1.0f / material->bump_map->width;
	heights->dv = 1.0f / material->bump_map->height;
	heights->center = sample_bump_map(material->bump_map, \
		hit_record->uv.u, hit_record->uv.v);
	heights->u = sample_bump_map(material->bump_map, \
		hit_record->uv.u + heights->du, hit_record->uv.v);
	heights->v = sample_bump_map(material->bump_map, \
		hit_record->uv.u, hit_record->uv.v + heights->dv);
}

static void	calculate_gradients(t_bump_heights *heights,
							t_bump_gradients *gradients)
{
	float	bump_strength;

	gradients->gradient_u = (heights->u - heights->center) / heights->du;
	gradients->gradient_v = (heights->v - heights->center) / heights->dv;
	bump_strength = 0.1f;
	gradients->gradient_u *= bump_strength;
	gradients->gradient_v *= bump_strength;
}

static t_vec3	apply_bump_offset(t_hit_record *hit_record,
								t_bump_gradients *gradients,
								t_vec3 tangent, t_vec3 bitangent)
{
	t_vec3	bump_offset;
	t_vec3	perturbed_normal;

	bump_offset.x = gradients->gradient_u * tangent.x
		+ gradients->gradient_v * bitangent.x;
	bump_offset.y = gradients->gradient_u * tangent.y
		+ gradients->gradient_v * bitangent.y;
	bump_offset.z = gradients->gradient_u * tangent.z
		+ gradients->gradient_v * bitangent.z;
	perturbed_normal = vec3_subtract(hit_record->normal, bump_offset);
	return (vec3_normalize(perturbed_normal));
}

t_vec3	calculate_bump_normal(t_hit_record *hit_record)
{
	t_material			*material;
	t_vec3				tangent;
	t_vec3				bitangent;
	t_bump_heights		heights;
	t_bump_gradients	gradients;

	material = &hit_record->object->material;
	if (!material->has_bump_map || !material->bump_map)
		return (hit_record->normal);
	get_tangent_vectors_by_type(hit_record, &tangent, &bitangent);
	sample_bump_heights(material, hit_record, &heights);
	calculate_gradients(&heights, &gradients);
	return (apply_bump_offset(hit_record, &gradients, tangent, bitangent));
}
