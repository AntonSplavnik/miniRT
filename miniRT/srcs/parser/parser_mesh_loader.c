/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_mesh_loader.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 16:30:19 by abillote          #+#    #+#             */
/*   Updated: 2025/07/03 17:15:22 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

int	count_and_allocate_file_elements(int fd, t_vec3 **vertices, \
										t_triangle **triangles)
{
	char	*line;
	int		vertex_count;
	int		face_count;
	int		line_count;

	vertex_count = 0;
	face_count = 0;
	line_count = 0;

	printf("Starting to count file elements...\n");
	line = get_next_line(fd);
	while (line)
	{
		line_count++;
		if (line_count % 100000 == 0)
			printf("Counted %d lines so far (vertices: %d, faces: %d)...\n",
					line_count, vertex_count, face_count);

		if (line[0] == 'v' && line[1] == ' ')
			vertex_count++;
		if (line[0] == 'f' && line[1] == ' ')
			face_count++;
		free(line);
		line = get_next_line(fd);
	}
	get_next_line(-1);
	printf("Counted vertices: %d, faces: %d\n", vertex_count, face_count);

	*vertices = malloc(sizeof(t_vec3) * vertex_count);
	*triangles = malloc(sizeof(t_triangle) * face_count * 2);

	if (!*vertices)
	{
		printf("Failed to allocate memory for vertices\n");
		return (0);
	}
	if (!*triangles)
	{
		printf("Failed to allocate memory for triangles\n");
		free(*vertices);
		return (0);
	}

	printf("Memory allocated successfully: vertices=%zu bytes, triangles=%zu bytes\n",
			sizeof(t_vec3) * vertex_count, sizeof(t_triangle) * face_count * 2);
	return (1);
}

static void	parse_file_content(int fd, t_vec3 *vertices, \
								t_triangle *triangles, int *t_idx)
{
	char	*line;
	int		v_idx;
	int		line_count;

	v_idx = 0;
	*t_idx = 0;
	line_count = 0;
	line = get_next_line(fd);
	while (line)
	{
		line_count++;
		if (line_count % 50000 == 0)
			printf("Processing line %d...\n", line_count);

		if (line[0] == 'v' && line[1] == ' ')
			process_vertex_line(line, vertices, &v_idx);
		else if (line[0] == 'f' && line[1] == ' ')
			process_face_line(line, triangles, vertices, t_idx);
		free(line);
		line = get_next_line(fd);
	}
	get_next_line(-1);
	printf("Finished processing %d lines, created %d triangles\n", line_count, *t_idx);
}

t_mesh	*load_obj_file(const char *filename)
{
	int			fd;
	t_vec3		*vertices;
	t_triangle	*triangles;
	int			triangle_count;
	t_mesh		*mesh;

	printf("Loading OBJ file: %s\n", filename);
	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		printf("Failed to open file: %s\n", filename);
		return (NULL);
	}

	printf("File opened successfully, starting to count elements...\n");
	if (!count_and_allocate_file_elements(fd, &vertices, &triangles))
	{
		printf("Failed to count and allocate file elements\n");
		close(fd);
		return (NULL);
	}

	printf("File elements counted and allocated\n");
	close(fd);

	printf("Reopening file for parsing...\n");
	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		printf("Failed to reopen file\n");
		free(vertices);
		free(triangles);
		return (NULL);
	}

	printf("Parsing file content...\n");
	parse_file_content(fd, vertices, triangles, &triangle_count);
	printf("File parsing completed, triangle count: %d\n", triangle_count);
	close(fd);

	printf("Creating mesh object...\n");
	mesh = create_mesh(triangles, triangle_count);
	printf("Mesh created successfully\n");
	free(vertices);
	return (mesh);
}
