/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compute_normals.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 12:00:00 by abillote          #+#    #+#             */
/*   Updated: 2025/07/20 21:18:51 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

void	compute_complex_normals(t_hit_record *hit_record, t_ray ray)
{
	if (hit_record->object->type == MESH)
		compute_mesh_normal(hit_record, ray);
	else if (hit_record->object->type == TRIANGLE)
		compute_triangle_normal(hit_record, ray);
	else
		hit_record->original_normal = vec3_create(0, 1, 0);
}

/*
** Calculate normal based on object type
*/
void	compute_object_normal(t_hit_record *hit_record, t_ray ray)
{
	if (hit_record->object->type == SPHERE)
		compute_sphere_normal(hit_record);
	else if (hit_record->object->type == CYLINDER)
		compute_cylinder_normal(hit_record);
	else if (hit_record->object->type == PLANE)
		compute_plane_normal(hit_record, ray);
	else if (hit_record->object->type == CUBE)
		compute_cube_normal(hit_record);
	else if (hit_record->object->type == CONE)
		compute_cone_normal(hit_record);
	else
		compute_complex_normals(hit_record, ray);
	hit_record->normal = hit_record->original_normal;
}

/*
** Determine if ray is hitting from inside or outside object
*/
void	compute_inside_outside(t_hit_record *hit_record, t_ray ray)
{
	if (vec3_dot(ray.direction, hit_record->original_normal) > 0)
	{
		hit_record->inside = 1;
		hit_record->normal = vec3_negate(hit_record->original_normal);
	}
	else
	{
		hit_record->inside = 0;
		hit_record->normal = hit_record->original_normal;
	}
}

/*
** Compute complete ray intersection data including point, normal, and UV
*/
void	compute_ray_intersection(t_ray ray, t_object *hit_object,
	t_hit_record *hit_record)
{
	hit_record->point = vec3_add(ray.origin, \
		vec3_scale(ray.direction, hit_record->t));
	hit_record->object = hit_object;
	hit_record->material = hit_object->material;
	compute_object_normal(hit_record, ray);
	compute_inside_outside(hit_record, ray);
	if (hit_record->material.has_texture || hit_record->material.has_checker)
		hit_record->uv = calculate_uv_coordinates(hit_record->point, \
			hit_object);
	if (hit_record->material.has_bump_map)
		hit_record->normal = calculate_bump_normal(hit_record);
}
