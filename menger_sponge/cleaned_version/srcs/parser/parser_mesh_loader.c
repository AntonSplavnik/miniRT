/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_mesh_loader.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 16:30:19 by abillote          #+#    #+#             */
/*   Updated: 2025/06/11 11:47:51 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

static int	open_obj_file(const char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	return (fd);
}

int	count_and_allocate_file_elements(int fd, t_vec3 **vertices, \
										t_triangle **triangles)
{
	char	*line;
	int		vertex_count;
	int		face_count;

	vertex_count = 0;
	face_count = 0;
	line = get_next_line(fd);
	while (line)
	{
		if (line[0] == 'v' && line[1] == ' ')
			vertex_count++;
		if (line[0] == 'f' && line[1] == ' ')
			face_count++;
		free(line);
		line = get_next_line(fd);
	}
	get_next_line(-1);
	*vertices = malloc(sizeof(t_vec3) * vertex_count);
	*triangles = malloc(sizeof(t_triangle) * face_count * 3);
	if (!vertices || !triangles)
		return (0);
	return (1);
}

static void	parse_file_content(int fd, t_vec3 *vertices, \
								t_triangle *triangles, int *t_idx)
{
	char	*line;
	int		v_idx;

	v_idx = 0;
	*t_idx = 0;
	line = get_next_line(fd);
	while (line)
	{
		if (line[0] == 'v' && line[1] == ' ')
			process_vertex_line(line, vertices, &v_idx);
		else if (line[0] == 'f' && line[1] == ' ')
			process_face_line(line, triangles, vertices, t_idx);
		free(line);
		line = get_next_line(fd);
	}
	get_next_line(-1);
}

t_mesh	*load_obj_file(const char *filename)
{
	int			fd;
	t_vec3		*vertices;
	t_triangle	*triangles;
	int			triangle_count;
	t_mesh		*mesh;

	fd = open_obj_file(filename);
	if (fd == -1 || !count_and_allocate_file_elements(fd, \
						&vertices, &triangles))
		return (NULL);
	close(fd);
	fd = open_obj_file(filename);
	if (fd == -1)
	{
		free(vertices);
		free(triangles);
		return (NULL);
	}
	parse_file_content(fd, vertices, triangles, &triangle_count);
	close(fd);
	mesh = create_mesh(triangles, triangle_count);
	free(vertices);
	return (mesh);
}
