/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_mesh_face_processing.c                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 15:20:00 by abillote          #+#    #+#             */
/*   Updated: 2025/07/18 15:35:57 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

void	parse_face_quad(t_triangle *triangles, t_vec3 *vertices, \
					t_face_data *face_data)
{
	set_triangle_vertices_quad_1(&triangles[*face_data->t_idx], vertices, \
								face_data->indices);
	set_triangle_normals_quad_1(&triangles[*face_data->t_idx], \
								face_data->normals, face_data->normal_indices);
	calculate_triangle_normal(&triangles[*face_data->t_idx]);
	(*face_data->t_idx)++;
	set_triangle_vertices_quad_2(&triangles[*face_data->t_idx], vertices, \
								face_data->indices);
	set_triangle_normals_quad_2(&triangles[*face_data->t_idx], \
								face_data->normals, face_data->normal_indices);
	calculate_triangle_normal(&triangles[*face_data->t_idx]);
	(*face_data->t_idx)++;
}

void	set_triangle_normals(t_triangle *triangle, t_vec3 *normals, \
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

void	parse_face_triangle(t_triangle *triangles, t_vec3 *vertices, \
						t_face_data *face_data)
{
	set_triangle_vertices(&triangles[*face_data->t_idx], vertices, \
						face_data->indices);
	set_triangle_normals(&triangles[*face_data->t_idx], face_data->normals, \
						face_data->normal_indices);
	calculate_triangle_normal(&triangles[*face_data->t_idx]);
	(*face_data->t_idx)++;
}

void	parse_vertex_normal_indices(char *values[], int *normal_indices, int i)
{
	char	*vertex_part;
	char	*normal_part;
	char	*first_slash;
	char	*second_slash;

	vertex_part = values[i];
	normal_part = NULL;
	first_slash = ft_strchr(vertex_part, '/');
	if (first_slash)
	{
		second_slash = ft_strchr(first_slash + 1, '/');
		if (second_slash)
		{
			normal_part = second_slash + 1;
			normal_indices[i] = ft_atoi(normal_part);
		}
	}
}

void	parse_face_indices(char **values, int *indices, int *normal_indices)
{
	int	i;

	i = 0;
	while (values[i] && i < 4)
	{
		parse_vertex_normal_indices(values, normal_indices, i);
		indices[i] = ft_atoi(values[i]);
		i++;
	}
}
