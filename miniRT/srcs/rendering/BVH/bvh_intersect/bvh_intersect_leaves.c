/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh_intersect_leaves.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 21:30:00 by abillote          #+#    #+#             */
/*   Updated: 2025/07/18 18:12:59 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"
#include "../../includes/bvh.h"

int	test_single_triangle(t_mesh *mesh, t_ray ray, int idx,
		t_intersect_result *result)
{
	int						original_idx;
	double					current_t;
	t_transformed_triangle	*tri;

	original_idx = get_triangle_original_index(mesh, idx);
	if (!is_valid_triangle_index(original_idx, mesh->triangle_count))
		return (result->hit_something);
	tri = &mesh->transformed_tris[original_idx];
	if (ray_triangle_intersect_fast(ray, *tri, &current_t))
	{
		if (current_t < *result->closest_t)
		{
			*result->closest_t = current_t;
			*result->tri_idx = original_idx;
			result->hit_something = 1;
		}
	}
	return (result->hit_something);
}

static t_triangle	setup_triangle_from_mesh(t_mesh *mesh, int original_idx)
{
	t_triangle	temp_tri;

	temp_tri = (t_triangle){
		.v0 = mesh->transformed_tris[original_idx].v0,
		.v1 = mesh->transformed_tris[original_idx].v1,
		.v2 = mesh->transformed_tris[original_idx].v2,
		.normal = mesh->transformed_tris[original_idx].normal,
		.n0 = mesh->transformed_tris[original_idx].n0,
		.n1 = mesh->transformed_tris[original_idx].n1,
		.n2 = mesh->transformed_tris[original_idx].n2,
		.has_vertex_normals = mesh->\
			transformed_tris[original_idx].has_vertex_normals
	};
	return (temp_tri);
}

int	test_single_triangle_bary(t_mesh *mesh, t_ray ray, int idx,
		t_intersect_bary_result *result)
{
	int			original_idx;
	double		current_t;
	t_vec3		current_bary;
	t_triangle	temp_tri;

	original_idx = get_triangle_original_index(mesh, idx);
	if (!is_valid_triangle_index(original_idx, mesh->triangle_count))
		return (result->hit_something);
	temp_tri = setup_triangle_from_mesh(mesh, original_idx);
	if (ray_triangle_intersect_bary(ray, temp_tri, &current_t, &current_bary))
	{
		if (current_t < *result->closest_t)
		{
			*result->closest_t = current_t;
			*result->tri_idx = original_idx;
			*result->closest_bary = current_bary;
			result->hit_something = 1;
		}
	}
	return (result->hit_something);
}
