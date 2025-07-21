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
** Validate and interpolate vertex normal for triangle
*/
void	interpolate_triangle_normal(t_hit_record *hit_record,
	t_triangle *triangle)
{
	double	bary_sum;
	t_vec3	interpolated;
	double	norm_len;

	bary_sum = hit_record->barycentric.x + hit_record->barycentric.y \
		+ hit_record->barycentric.z;
	if (fabs(bary_sum - 1.0) < 0.001 && hit_record->barycentric.x >= 0.0 \
		&& hit_record->barycentric.x <= 1.0 && hit_record->barycentric.y \
		>= 0.0 && hit_record->barycentric.y <= 1.0 && \
		hit_record->barycentric.z >= 0.0 && hit_record->barycentric.z <= 1.0)
	{
		interpolated = interpolate_vertex_normal(hit_record->barycentric, \
			triangle->n0, triangle->n1, triangle->n2);
		norm_len = vec3_length(interpolated);
		if (norm_len > 0.1 && norm_len < 10.0)
			hit_record->original_normal = interpolated;
	}
}

/*
** Calculate normal for triangle objects with double-sided support
*/
void	compute_triangle_normal(t_hit_record *hit_record, t_ray ray)
{
	t_triangle	*triangle;

	triangle = (t_triangle *)(hit_record->object->data);
	hit_record->original_normal = triangle->normal;
	if (triangle->has_vertex_normals)
		interpolate_triangle_normal(hit_record, triangle);
	if (!triangle->has_vertex_normals && vec3_dot(ray.direction, \
		hit_record->original_normal) > 0)
		hit_record->original_normal = vec3_negate(\
			hit_record->original_normal);
}

/*
** Interpolate mesh vertex normals with validation
*/
void	interpolate_mesh_normal(t_hit_record *hit_record,
	t_transformed_triangle *tri)
{
	double	bary_sum;
	t_vec3	interpolated;
	double	norm_len;

	bary_sum = hit_record->barycentric.x + hit_record->barycentric.y \
		+ hit_record->barycentric.z;
	if (fabs(bary_sum - 1.0) < 0.001 && hit_record->barycentric.x >= 0.0 \
		&& hit_record->barycentric.x <= 1.0 && hit_record->barycentric.y \
		>= 0.0 && hit_record->barycentric.y <= 1.0 && \
		hit_record->barycentric.z >= 0.0 && hit_record->barycentric.z <= 1.0)
	{
		interpolated = interpolate_vertex_normal(hit_record->barycentric, \
			tri->n0, tri->n1, tri->n2);
		norm_len = vec3_length(interpolated);
		if (norm_len > 0.1 && norm_len < 10.0)
			hit_record->original_normal = interpolated;
	}
}

/*
** Handle mesh normal calculation with bounds checking
*/
void	compute_mesh_normal(t_hit_record *hit_record, t_ray ray)
{
	t_mesh					*mesh;
	int						triangle_idx;
	t_transformed_triangle	*tri;

	mesh = (t_mesh *)(hit_record->object->data);
	triangle_idx = (int)hit_record->triangle_idx;
	if (triangle_idx >= 0 && triangle_idx < mesh->triangle_count)
	{
		tri = &mesh->transformed_tris[triangle_idx];
		hit_record->original_normal = tri->normal;
		if (tri->has_vertex_normals)
			interpolate_mesh_normal(hit_record, tri);
		if (!tri->has_vertex_normals && vec3_dot(ray.direction, \
			hit_record->original_normal) > 0)
			hit_record->original_normal = vec3_negate(\
				hit_record->original_normal);
	}
	else
		hit_record->original_normal = vec3_create(0, 1, 0);
}
