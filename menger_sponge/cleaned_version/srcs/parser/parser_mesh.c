/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_mesh.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 10:19:44 by abillote          #+#    #+#             */
/*   Updated: 2025/06/04 15:34:38 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

t_mesh *load_obj_file(const char *filename)
{
	FILE *file = fopen(filename, "r");
	if (!file) return NULL;

	// First pass: count vertices and faces
	int vertex_count = 0;
	int face_count = 0;
	char line[256];

	while (fgets(line, sizeof(line), file)) {
		if (line[0] == 'v' && line[1] == ' ') vertex_count++;
		if (line[0] == 'f' && line[1] == ' ') face_count++;
	}

	// Allocate memory
	t_vec3 *vertices = malloc(sizeof(t_vec3) * vertex_count);
	t_triangle *triangles = malloc(sizeof(t_triangle) * face_count * 2); // Each quad face might become 2 triangles

	// Second pass: read vertices and faces
	rewind(file);
	int v_idx = 0;
	int t_idx = 0;

	while (fgets(line, sizeof(line), file)) {
		if (line[0] == 'v' && line[1] == ' ') {
			// Parse vertex
			sscanf(line, "v %lf %lf %lf", &vertices[v_idx].x, &vertices[v_idx].y, &vertices[v_idx].z);
			v_idx++;
		}
		else if (line[0] == 'f' && line[1] == ' ') {
			// Parse face (simplest case: just vertex indices)
			int indices[4];
			int count = sscanf(line, "f %d %d %d %d", &indices[0], &indices[1], &indices[2], &indices[3]);

			// Create first triangle (faces are 1-indexed in OBJ)
			triangles[t_idx].v0 = vertices[indices[0]-1];
			triangles[t_idx].v1 = vertices[indices[1]-1];
			triangles[t_idx].v2 = vertices[indices[2]-1];

			// Calculate normal
			t_vec3 edge1 = vec3_subtract(triangles[t_idx].v1, triangles[t_idx].v0);
			t_vec3 edge2 = vec3_subtract(triangles[t_idx].v2, triangles[t_idx].v0);
			triangles[t_idx].normal = vec3_normalize(vec3_cross(edge1, edge2));

			t_idx++;

			// If this is a quad, create second triangle
			if (count == 4) {
				triangles[t_idx].v0 = vertices[indices[0]-1];
				triangles[t_idx].v1 = vertices[indices[2]-1];
				triangles[t_idx].v2 = vertices[indices[3]-1];

				// Calculate normal
				edge1 = vec3_subtract(triangles[t_idx].v1, triangles[t_idx].v0);
				edge2 = vec3_subtract(triangles[t_idx].v2, triangles[t_idx].v0);
				triangles[t_idx].normal = vec3_normalize(vec3_cross(edge1, edge2));

				t_idx++;
			}
		}
	}

	fclose(file);

	// Create mesh
	t_mesh *mesh = malloc(sizeof(t_mesh));
	mesh->triangles = triangles;
	mesh->triangle_count = t_idx;
	mesh->position = (t_vec3){0, 0, 0};
	mesh->rotation = (t_vec3){0, 0, 0};
	mesh->scale = (t_vec3){1, 1, 1};

	free(vertices);
	return (mesh);
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
	char		*material_block;

	material_block = extract_material_block(line);
	if (material_block)
		trim_material_block(line);
	parts = ft_split_line(line, ' ');
	if (!parts)
		return (0);
	check_parts_count(scene, parts, 6, "mesh");
	filename = parts[1];
	scale = 0;
	if (!read_vector(parts[2], &position) ||
		!read_vector(parts[3], &rotation) ||
		!(scale = ft_atof(parts[4])) ||
		!read_color(parts[5], &color))
	{
		free_split(parts);
		parse_error(scene, "Invalid format for mesh file");
		return (0);
	}

	// Load OBJ file
	t_mesh *mesh = load_obj_file(filename);
	if (!mesh)
	{
		free_split(parts);
		parse_error(scene, "Failed to load OBJ file");
	}
	mesh->position = position;
	mesh->rotation = rotation;
	mesh->scale = (t_vec3){scale, scale, scale};

	// Create object
	mesh_obj = malloc(sizeof(t_object));
	mesh_obj->type = MESH;
	mesh_obj->data = mesh;
	mesh_obj->material = create_material(color);
	mesh_obj->next = NULL;

	if(material_block)
	{
		parse_material_properties(scene, material_block, &mesh_obj->material);
		free(material_block);
	}
	// Add to scene
	add_object(scene, mesh_obj);

	free_split(parts);
	return 1;
}
