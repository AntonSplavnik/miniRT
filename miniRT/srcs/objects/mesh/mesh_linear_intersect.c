/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mesh_linear_intersect.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 16:10:00 by antonsplavn       #+#    #+#             */
/*   Updated: 2025/07/18 16:20:27 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/miniRT.h"

/*
** Creates a triangle structure from transformed triangle data
*/
static t_triangle	create_triangle_from_transformed(\
													t_transformed_triangle *tri)
{
	t_triangle	result;

	result.v0 = tri->v0;
	result.v1 = tri->v1;
	result.v2 = tri->v2;
	result.normal = tri->normal;
	return (result);
}

/*
** Tests single triangle intersection and updates closest match
*/
static void	test_triangle_intersection(t_mesh_intersect_data *data, \
										t_triangle tri, int i)
{
	double	current_t;

	if (ray_triangle_intersect(data->ray, tri, &current_t))
	{
		if (current_t < data->closest_t)
		{
			data->closest_t = current_t;
			*(data->triangle_idx) = i;
			data->hit_something = 1;
		}
	}
}

/*
** Tests ray intersection with mesh triangles using linear search
*/
int	mesh_linear_intersect(t_ray ray, t_mesh mesh, double *t, int *triangle_idx)
{
	t_mesh_intersect_data	data;
	int						i;
	t_triangle				tri;

	data.ray = ray;
	data.closest_t = INFINITY;
	data.hit_something = 0;
	data.triangle_idx = triangle_idx;
	i = 0;
	while (i < mesh.triangle_count)
	{
		tri = create_triangle_from_transformed(&mesh.transformed_tris[i]);
		test_triangle_intersection(&data, tri, i);
		i++;
	}
	if (data.hit_something)
		*t = data.closest_t;
	return (data.hit_something);
}
