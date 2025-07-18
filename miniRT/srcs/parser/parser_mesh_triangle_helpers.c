/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_mesh_triangle_helpers.c                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 15:15:00 by abillote          #+#    #+#             */
/*   Updated: 2025/07/18 15:17:44 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

void	set_triangle_vertices_quad_1(t_triangle *triangle, t_vec3 *vertices, \
									int *indices)
{
	triangle->v0 = vertices[indices[0] - 1];
	triangle->v1 = vertices[indices[2] - 1];
	triangle->v2 = vertices[indices[3] - 1];
}

void	set_triangle_normals_quad_1(t_triangle *triangle, t_vec3 *normals, \
									int *normal_indices)
{
	if (normals && normal_indices[0] > 0 && normal_indices[2] > 0 \
		&& normal_indices[3] > 0)
	{
		triangle->n0 = normals[normal_indices[0] - 1];
		triangle->n1 = normals[normal_indices[2] - 1];
		triangle->n2 = normals[normal_indices[3] - 1];
		triangle->has_vertex_normals = 1;
	}
	else
	{
		triangle->has_vertex_normals = 0;
	}
}

void	set_triangle_vertices_quad_2(t_triangle *triangle, t_vec3 *vertices, \
									int *indices)
{
	triangle->v0 = vertices[indices[0] - 1];
	triangle->v1 = vertices[indices[1] - 1];
	triangle->v2 = vertices[indices[2] - 1];
}

void	set_triangle_normals_quad_2(t_triangle *triangle, t_vec3 *normals, \
									int *normal_indices)
{
	if (normals && normal_indices[0] > 0 && normal_indices[1] > 0 \
		&& normal_indices[2] > 0)
	{
		triangle->n0 = normals[normal_indices[0] - 1];
		triangle->n1 = normals[normal_indices[1] - 1];
		triangle->n2 = normals[normal_indices[2] - 1];
		triangle->has_vertex_normals = 1;
	}
	else
	{
		triangle->has_vertex_normals = 0;
	}
}

void	set_triangle_vertices(t_triangle *triangle, t_vec3 *vertices, \
							int *indices)
{
	triangle->v0 = vertices[indices[0] - 1];
	triangle->v1 = vertices[indices[1] - 1];
	triangle->v2 = vertices[indices[2] - 1];
}
