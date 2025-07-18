/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_mesh_loader_helpers.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 15:45:00 by abillote          #+#    #+#             */
/*   Updated: 2025/07/18 15:46:41 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

void	count_elements(int fd, int *vertex_count, int *face_count, \
					int *normal_count)
{
	char	*line;

	*vertex_count = 0;
	*face_count = 0;
	*normal_count = 0;
	line = get_next_line(fd);
	while (line)
	{
		if (line[0] == 'v' && line[1] == ' ')
			(*vertex_count)++;
		else if (line[0] == 'v' && line[1] == 'n' && line[2] == ' ')
			(*normal_count)++;
		if (line[0] == 'f' && line[1] == ' ')
			(*face_count)++;
		free(line);
		line = get_next_line(fd);
	}
	get_next_line(-1);
}

int	allocate_memory(t_vec3 **vertices, t_triangle **triangles, \
					t_vec3 **normals, t_counts *counts)
{
	*vertices = malloc(sizeof(t_vec3) * counts->vertex_count);
	*triangles = malloc(sizeof(t_triangle) * counts->face_count * 2);
	*normals = malloc(sizeof(t_vec3) * counts->normal_count);
	if (!*vertices)
		return (0);
	if (!*triangles)
	{
		free(*vertices);
		return (0);
	}
	if (!*normals)
	{
		free(*vertices);
		free(*triangles);
		return (0);
	}
	return (1);
}

void	parse_file_content(int fd, t_mesh_data *mesh_data)
{
	char			*line;
	int				v_idx;
	int				n_idx;
	t_process_data	process_data;

	v_idx = 0;
	n_idx = 0;
	*mesh_data->t_idx = 0;
	process_data.triangles = mesh_data->triangles;
	process_data.vertices = mesh_data->vertices;
	process_data.t_idx = mesh_data->t_idx;
	process_data.normals = mesh_data->normals;
	line = get_next_line(fd);
	while (line)
	{
		if (line[0] == 'v' && line[1] == ' ')
			process_vertex_line(line, mesh_data->vertices, &v_idx);
		else if (line[0] == 'v' && line[1] == 'n' && line[2] == ' ')
			process_normal_line(line, mesh_data->normals, &n_idx);
		else if (line[0] == 'f' && line[1] == ' ')
			process_face_line(line, &process_data);
		free(line);
		line = get_next_line(fd);
	}
	get_next_line(-1);
}

void	cleanup_memory(t_vec3 *vertices, t_triangle *triangles, \
					t_vec3 *normals)
{
	free(vertices);
	free(triangles);
	free(normals);
}

int	open_file_for_parsing(const char *filename, t_vec3 *vertices, \
						t_triangle *triangles, t_vec3 *normals)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		cleanup_memory(vertices, triangles, normals);
		return (-1);
	}
	return (fd);
}
