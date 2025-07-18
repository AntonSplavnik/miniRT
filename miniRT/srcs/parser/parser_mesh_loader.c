/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_mesh_loader.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 16:30:19 by abillote          #+#    #+#             */
/*   Updated: 2025/07/18 15:35:57 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

int	count_and_allocate_file_elements(int fd, t_vec3 **vertices, \
									t_triangle **triangles, t_vec3 **normals)
{
	t_counts	counts;

	count_elements(fd, &counts.vertex_count, &counts.face_count, \
				&counts.normal_count);
	return (allocate_memory(vertices, triangles, normals, &counts));
}

static int	allocate_and_open_file(const char *filename, t_vec3 **vertices, \
								t_triangle **triangles, t_vec3 **normals)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
		return (-1);
	if (!count_and_allocate_file_elements(fd, vertices, triangles, normals))
	{
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

static t_mesh	*process_obj_file(const char *filename, t_vec3 *vertices, \
								t_triangle *triangles, t_vec3 *normals)
{
	int			fd;
	int			triangle_count;
	t_mesh_data	mesh_data;

	fd = open_file_for_parsing(filename, vertices, triangles, normals);
	if (fd == -1)
		return (NULL);
	mesh_data.vertices = vertices;
	mesh_data.triangles = triangles;
	mesh_data.normals = normals;
	mesh_data.t_idx = &triangle_count;
	parse_file_content(fd, &mesh_data);
	close(fd);
	free(vertices);
	free(normals);
	return (create_mesh(triangles, triangle_count));
}

t_mesh	*load_obj_file(const char *filename)
{
	t_vec3		*vertices;
	t_triangle	*triangles;
	t_vec3		*normals;
	t_mesh		*result;

	if (allocate_and_open_file(filename, &vertices, &triangles, &normals) == -1)
		return (NULL);
	result = process_obj_file(filename, vertices, triangles, normals);
	return (result);
}
