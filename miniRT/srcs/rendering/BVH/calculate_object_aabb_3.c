/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calculate_object_aabb_3.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 00:00:00 by antonsplavn      #+#    #+#             */
/*   Updated: 2025/06/23 00:00:00 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"
#include "../../includes/bvh.h"

void	handle_negative_scale(t_aabb *bounds, t_vec3 scale)
{
	double	temp;

	if (scale.x < 0)
	{
		temp = bounds->min.x;
		bounds->min.x = bounds->max.x;
		bounds->max.x = temp;
	}
	if (scale.y < 0)
	{
		temp = bounds->min.y;
		bounds->min.y = bounds->max.y;
		bounds->max.y = temp;
	}
	if (scale.z < 0)
	{
		temp = bounds->min.z;
		bounds->min.z = bounds->max.z;
		bounds->max.z = temp;
	}
}

void	update_mesh_min_bounds(t_triangle *tri, t_aabb *bounds)
{
	bounds->min.x = fmin(bounds->min.x, fmin(fmin(tri->v0.x, tri->v1.x),
				tri->v2.x));
	bounds->min.y = fmin(bounds->min.y, fmin(fmin(tri->v0.y, tri->v1.y),
				tri->v2.y));
	bounds->min.z = fmin(bounds->min.z, fmin(fmin(tri->v0.z, tri->v1.z),
				tri->v2.z));
}

void	update_mesh_max_bounds(t_triangle *tri, t_aabb *bounds)
{
	bounds->max.x = fmax(bounds->max.x, fmax(fmax(tri->v0.x, tri->v1.x),
				tri->v2.x));
	bounds->max.y = fmax(bounds->max.y, fmax(fmax(tri->v0.y, tri->v1.y),
				tri->v2.y));
	bounds->max.z = fmax(bounds->max.z, fmax(fmax(tri->v0.z, tri->v1.z),
				tri->v2.z));
}

void	adjust_mesh_aabb(t_mesh *mesh, t_aabb *bounds)
{
	t_vec3	position;
	t_vec3	scale;

	position = mesh->position;
	scale = mesh->scale;
	bounds->min.x = position.x + bounds->min.x * scale.x;
	bounds->min.y = position.y + bounds->min.y * scale.y;
	bounds->min.z = position.z + bounds->min.z * scale.z;
	bounds->max.x = position.x + bounds->max.x * scale.x;
	bounds->max.y = position.y + bounds->max.y * scale.y;
	bounds->max.z = position.z + bounds->max.z * scale.z;
	handle_negative_scale(bounds, scale);
}
