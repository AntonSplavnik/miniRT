/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compute_ray_intersection_2.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 03:21:08 by antonsplavn       #+#    #+#             */
/*   Updated: 2025/07/14 03:22:38 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

/*
** Calculate normal for triangle objects with double-sided support
*/
void	compute_triangle_normal(t_hit_record *hit_record, t_ray ray)
{
	t_triangle	*triangle;

	triangle = (t_triangle *)(hit_record->object->data);
	hit_record->original_normal = triangle->normal;
	if (vec3_dot(ray.direction, hit_record->original_normal) > 0)
		hit_record->original_normal = vec3_negate(\
			hit_record->original_normal);
}

/*
** Handle mesh normal calculation with bounds checking
*/
void	compute_mesh_normal(t_hit_record *hit_record, t_ray ray)
{
	t_mesh	*mesh;
	int		triangle_idx;

	mesh = (t_mesh *)(hit_record->object->data);
	triangle_idx = (int)hit_record->triangle_idx;
	if (triangle_idx >= 0 && triangle_idx < mesh->triangle_count)
	{
		hit_record->original_normal = mesh->triangles[triangle_idx].normal;
		if (vec3_dot(ray.direction, hit_record->original_normal) > 0)
			hit_record->original_normal = vec3_negate(\
				hit_record->original_normal);
	}
	else
		hit_record->original_normal = vec3_create(0, 1, 0);
}
