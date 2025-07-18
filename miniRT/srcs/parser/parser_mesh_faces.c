/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_mesh_faces.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 11:40:10 by abillote          #+#    #+#             */
/*   Updated: 2025/07/18 15:35:57 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

static void	init_face_data(t_face_data *face_data, int *indices, \
							int *normal_indices, t_process_data *process_data)
{
	normal_indices[0] = -1;
	normal_indices[1] = -1;
	normal_indices[2] = -1;
	normal_indices[3] = -1;
	face_data->indices = indices;
	face_data->t_idx = process_data->t_idx;
	face_data->normals = process_data->normals;
	face_data->normal_indices = normal_indices;
}

static void	process_face_data(t_process_data *process_data, \
							t_face_data *face_data, char **values)
{
	int	i;

	i = 0;
	while (values[i] && i < 4)
		i++;
	if (i == 3)
		parse_face_triangle(process_data->triangles, process_data->vertices, \
							face_data);
	else if (i == 4)
		parse_face_quad(process_data->triangles, process_data->vertices, \
						face_data);
}

void	process_face_line(char *line, t_process_data *process_data)
{
	char		**values;
	int			indices[4];
	int			normal_indices[4];
	t_face_data	face_data;

	init_face_data(&face_data, indices, normal_indices, process_data);
	values = ft_split_line(line + 2, ' ');
	if (!values || !values[0] || !values[1] || !values[2])
	{
		if (values)
			free_split(values);
		return ;
	}
	parse_face_indices(values, indices, normal_indices);
	process_face_data(process_data, &face_data, values);
	free_split(values);
}

void	process_vertex_line(char *line, t_vec3 *vertices, int *v_idx)
{
	char	**values;

	values = ft_split_line(line + 2, ' ');
	if (!values || !values[0] || !values[1] || !values[2])
	{
		if (values)
			free_split(values);
		return ;
	}
	vertices[*v_idx].x = ft_atof(values[0]);
	vertices[*v_idx].y = ft_atof(values[1]);
	vertices[*v_idx].z = ft_atof(values[2]);
	(*v_idx)++;
	free_split(values);
}

void	process_normal_line(char *line, t_vec3 *normals, int *n_idx)
{
	char	**values;

	values = ft_split_line(line + 3, ' ');
	if (!values || !values[0] || !values[1] || !values[2])
	{
		if (values)
			free_split(values);
		return ;
	}
	normals[*n_idx].x = ft_atof(values[0]);
	normals[*n_idx].y = ft_atof(values[1]);
	normals[*n_idx].z = ft_atof(values[2]);
	(*n_idx)++;
	free_split(values);
}
