/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mesh_bary_intersect.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 16:20:00 by antonsplavn       #+#    #+#             */
/*   Updated: 2025/07/18 16:20:10 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/miniRT.h"

/*
** Creates a triangle with vertex normals from transformed triangle data
*/
static t_triangle	create_triangle_with_normals(t_transformed_triangle *tri)
{
	t_triangle	result;

	result.v0 = tri->v0;
	result.v1 = tri->v1;
	result.v2 = tri->v2;
	result.normal = tri->normal;
	result.n0 = tri->n0;
	result.n1 = tri->n1;
	result.n2 = tri->n2;
	result.has_vertex_normals = tri->has_vertex_normals;
	return (result);
}

/*
** Tests single triangle intersection with barycentric coordinates
*/
static void	test_triangle_bary_intersection(t_mesh_bary_data *data, \
												t_triangle tri, int i)
{
	double	current_t;
	t_vec3	current_bary;

	if (ray_triangle_intersect_bary(data->ray, tri, &current_t, &current_bary))
	{
		if (current_t < data->closest_t)
		{
			data->closest_t = current_t;
			data->closest_bary = current_bary;
			*(data->triangle_idx) = i;
			data->hit_something = 1;
		}
	}
}

/*
** Tests ray intersection with mesh triangles using barycentric coordinates
*/
int	mesh_linear_intersect_bary(t_ray ray, t_mesh mesh, \
								t_mesh_bary_params params)
{
	t_mesh_bary_data	data;
	int					i;
	t_triangle			tri;

	data.ray = ray;
	data.closest_t = INFINITY;
	data.hit_something = 0;
	data.triangle_idx = params.triangle_idx;
	i = 0;
	while (i < mesh.triangle_count)
	{
		tri = create_triangle_with_normals(&mesh.transformed_tris[i]);
		test_triangle_bary_intersection(&data, tri, i);
		i++;
	}
	if (data.hit_something)
	{
		*(params.t) = data.closest_t;
		*(params.bary) = data.closest_bary;
	}
	return (data.hit_something);
}
