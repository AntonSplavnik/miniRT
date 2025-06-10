/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_mesh.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 10:19:44 by abillote          #+#    #+#             */
/*   Updated: 2025/06/10 15:54:04 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

static FILE	*open_obj_file(const char *filename)
{
	FILE	*file;

	file = fopen(filename, "r");
	return (file);
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

static void	count_file_elements(FILE *file, int *vertex_count, int *face_count)
{
	char	line[256];

	*vertex_count = 0;
	*face_count = 0;
	while (fgets(line, sizeof(line), file))
	{
		if (line[0] == 'v' && line[1] == ' ')
			(*vertex_count)++;
		if (line[0] == 'f' && line[1] == ' ')
			(*face_count)++;
	}
}

void	calculate_triangle_normal(t_triangle *triangle)
{
	t_vec3	edge1;
	t_vec3	edge2;

	edge1 = vec3_subtract(triangle->v1, triangle->v0);
	edge2 = vec3_subtract(triangle->v2, triangle->v0);
	triangle->normal = vec3_normalize(vec3_cross(edge1, edge2));
}

static void	parse_file_content(FILE *file, t_vec3 *vertices,
	t_triangle *triangles, int *t_idx)
{
	char	line[256];
	int		v_idx;

	v_idx = 0;
	*t_idx = 0;
	while (fgets(line, sizeof(line), file))
	{
		if (line[0] == 'v' && line[1] == ' ')
			process_vertex_line(line, vertices, &v_idx);
		else if (line[0] == 'f' && line[1] == ' ')
			process_face_line(line, triangles, vertices, t_idx);
	}
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
	FILE		*file;
	int			vertex_count;
	int			face_count;
	t_vec3		*vertices;
	t_triangle	*triangles;
	int			triangle_count;
	t_mesh		*mesh;

	file = open_obj_file(filename);
	if (!file)
		return (NULL);
	count_file_elements(file, &vertex_count, &face_count);
	vertices = allocate_vertices(vertex_count);
	triangles = allocate_triangles(face_count);
	rewind(file);
	parse_file_content(file, vertices, triangles, &triangle_count);
	fclose(file);
	mesh = create_mesh_object(triangles, triangle_count);
	free(vertices);
	return (mesh);
}

void	parse_mesh_vectors(t_scene *scene, char **parts, t_vec3 *position, t_vec3 *rotation)
{
	if (!read_vector(parts[2], position))
	{
		free_split(parts);
		parse_error(scene, "Expected format for mesh position: x,y,z");
	}
	if (!read_vector(parts[3], rotation))
	{
		free_split(parts);
		parse_error(scene, "Expected format for mesh rotation: nx,ny,nz");
	}
}

static int	validate_mesh_scale_color(char **parts, double *scale, t_color *color)
{
	*scale = ft_atof(parts[4]);
	if (*scale <= 0)
		return (0);
	if (!read_color(parts[5], color))
		return (0);
	return (1);
}

t_object	*create_mesh_object_from_data(t_mesh *mesh, t_color color)
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

void	apply_mesh_transforms(t_mesh *mesh, t_vec3 position,
	t_vec3 rotation, double scale)
{
	mesh->position = position;
	mesh->rotation = rotation;
	mesh->scale = (t_vec3){scale, scale, scale};
}

int	parse_mesh(t_scene *scene, char *line)
{
	char		**parts;
	t_vec3		position;
	t_vec3		rotation;
	double		scale;
	t_color		color;
	t_mesh		*mesh;
	t_object	*mesh_obj;

	parts = ft_split_line(line, ' ');
	if (!parts)
		return (0);
	check_parts_count(scene, parts, 6, "mesh");
	parse_mesh_vectors(scene, parts, &position, &rotation);
	if (!validate_mesh_scale_color(parts, &scale, &color))
	{
		free_split(parts);
		parse_error(scene, "Invalid mesh scale/color");
		return (0);
	}
	mesh = load_obj_file(parts[1]);
	if (!check_mesh_error(mesh, parts, scene))
		return (0);
	apply_mesh_transforms(mesh, position, rotation, scale);
	mesh_obj = create_mesh_object_from_data(mesh, color);
	add_object(scene, mesh_obj);
	free_split(parts);
	return (1);
}
