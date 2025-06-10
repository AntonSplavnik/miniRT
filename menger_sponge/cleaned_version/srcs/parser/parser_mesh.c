/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_mesh.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 10:19:44 by abillote          #+#    #+#             */
/*   Updated: 2025/06/10 12:27:59 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

static int	open_obj_file(const char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	return (fd);
}

static t_vec3	*allocate_vertices(int vertex_count)
{
	t_vec3	*vertices;

	vertices = malloc(sizeof(t_vec3) * vertex_count);
	return (vertices);
}

static t_triangle	*allocate_triangles(int face_count)
{
	t_triangle	*triangles;

	triangles = malloc(sizeof(t_triangle) * face_count * 2);
	return (triangles);
}

static void	calculate_triangle_normal(t_triangle *triangle)
{
	t_vec3	edge1;
	t_vec3	edge2;

	edge1 = vec3_subtract(triangle->v1, triangle->v0);
	edge2 = vec3_subtract(triangle->v2, triangle->v0);
	triangle->normal = vec3_normalize(vec3_cross(edge1, edge2));
}

static void	create_first_triangle(t_triangle *triangles, t_vec3 *vertices,
	int *indices, int *t_idx)
{
	triangles[*t_idx].v0 = vertices[indices[0] - 1];
	triangles[*t_idx].v1 = vertices[indices[1] - 1];
	triangles[*t_idx].v2 = vertices[indices[2] - 1];
	calculate_triangle_normal(&triangles[*t_idx]);
	(*t_idx)++;
}

static void	create_second_triangle(t_triangle *triangles, t_vec3 *vertices,
	int *indices, int *t_idx)
{
	triangles[*t_idx].v0 = vertices[indices[0] - 1];
	triangles[*t_idx].v1 = vertices[indices[2] - 1];
	triangles[*t_idx].v2 = vertices[indices[3] - 1];
	calculate_triangle_normal(&triangles[*t_idx]);
	(*t_idx)++;
}

static int	parse_face_indices(char *line, int *indices)
{
	char	**parts;
	char	**face_parts;
	int		count;
	int		i;

	parts = ft_split_line(line, ' ');
	if (!parts)
		return (0);
	count = count_parts(parts) - 1;
	if (count < 3 || count > 4)
	{
		free_split(parts);
		return (0);
	}
	i = 0;
	while (i < count)
	{
		face_parts = ft_split_line(parts[i + 1], '/');
		if (face_parts && face_parts[0])
			indices[i] = ft_atoi(face_parts[0]);
		else
			indices[i] = 0;
		if (face_parts)
			free_split(face_parts);
		i++;
	}
	free_split(parts);
	return (count);
}

static void	parse_face(char *line, t_triangle *triangles, t_vec3 *vertices,
	int *t_idx, int vertex_count)
{
	int	indices[4];
	int	count;
	int	i;

	count = parse_face_indices(line, indices);
	if (count < 3)
		return ;
	i = 0;
	while (i < count)
	{
		if (indices[i] <= 0 || indices[i] > vertex_count)
			return ;
		i++;
	}
	create_first_triangle(triangles, vertices, indices, t_idx);
	if (count == 4)
		create_second_triangle(triangles, vertices, indices, t_idx);
}

static void	process_obj_line(char *line, t_vec3 *vertices,
	t_triangle *triangles, int *v_idx, int *t_idx, int vertex_count)
{
	if (line[0] == 'v' && line[1] == ' ')
	{
		parse_vertex_line(line, &vertices[*v_idx]);
		(*v_idx)++;
	}
	else if (line[0] == 'f' && line[1] == ' ')
	{
		parse_face(line, triangles, vertices, t_idx, vertex_count);
	}
}

static int	parse_file_content(int fd, t_vec3 *vertices,
	t_triangle *triangles, int *t_idx, int vertex_count)
{
	char	*line;
	int		v_idx;

	v_idx = 0;
	*t_idx = 0;
	line = get_next_line(fd);
	while (line)
	{
		process_obj_line(line, vertices, triangles, &v_idx, t_idx, vertex_count);
		free(line);
		line = get_next_line(fd);
	}
	get_next_line(-1);
	return (1);
}

static t_mesh	*create_mesh_object(t_triangle *triangles, int triangle_count)
{
	t_mesh	*mesh;

	mesh = malloc(sizeof(t_mesh));
	if (!mesh)
		return (NULL);
	mesh->triangles = triangles;
	mesh->triangle_count = triangle_count;
	mesh->position = (t_vec3){0, 0, 0};
	mesh->rotation = (t_vec3){0, 0, 0};
	mesh->scale = (t_vec3){1, 1, 1};
	return (mesh);
}

t_mesh	*load_obj_file(const char *filename)
{
	int			fd;
	int			vertex_count;
	int			face_count;
	t_vec3		*vertices;
	t_triangle	*triangles;
	int			triangle_count;
	t_mesh		*mesh;

	fd = open_obj_file(filename);
	if (fd < 0)
		return (NULL);
	count_obj_elements(fd, &vertex_count, &face_count);
	close(fd);
	vertices = allocate_vertices(vertex_count);
	triangles = allocate_triangles(face_count);
	if (!vertices || !triangles)
		return (NULL);
	fd = open_obj_file(filename);
	if (fd < 0)
		return (NULL);
	parse_file_content(fd, vertices, triangles, &triangle_count, vertex_count);
	close(fd);
	mesh = create_mesh_object(triangles, triangle_count);
	free(vertices);
	return (mesh);
}

static int	validate_mesh_params(char **parts, t_vec3 *position,
	t_vec3 *rotation, double *scale, t_color *color)
{
	if (!read_vector(parts[2], position))
		return (0);
	if (!read_vector(parts[3], rotation))
		return (0);
	*scale = ft_atof(parts[4]);
	if (*scale <= 0)
		return (0);
	if (!read_color(parts[5], color))
		return (0);
	return (1);
}

static t_object	*create_mesh_object_from_data(t_mesh *mesh, t_color color)
{
	t_object	*mesh_obj;

	mesh_obj = malloc(sizeof(t_object));
	if (!mesh_obj)
		return (NULL);
	mesh_obj->type = MESH;
	mesh_obj->data = mesh;
	mesh_obj->material = create_material(color);
	mesh_obj->next = NULL;
	return (mesh_obj);
}

static void	apply_mesh_transforms(t_mesh *mesh, t_vec3 position,
	t_vec3 rotation, double scale)
{
	mesh->position = position;
	mesh->rotation = rotation;
	mesh->scale = (t_vec3){scale, scale, scale};
}

int	parse_mesh(t_scene *scene, char *line)
{
	char		**parts;
	char		*filename;
	t_vec3		position;
	t_vec3		rotation;
	double		scale;
	t_color		color;
	t_object	*mesh_obj;
	t_mesh		*mesh;

	parts = ft_split_line(line, ' ');
	if (!parts)
		return (0);
	check_parts_count(scene, parts, 6, "mesh");
	filename = parts[1];
	if (!validate_mesh_params(parts, &position, &rotation, &scale, &color))
	{
		free_split(parts);
		parse_error(scene, "Invalid format for mesh file");
		return (0);
	}
	mesh = load_obj_file(filename);
	if (!mesh)
	{
		free_split(parts);
		parse_error(scene, "Failed to load OBJ file");
	}
	apply_mesh_transforms(mesh, position, rotation, scale);
	mesh_obj = create_mesh_object_from_data(mesh, color);
	add_object(scene, mesh_obj);
	printf("Mesh loaded from file: %s\n", filename);
	free_split(parts);
	return (1);
}
