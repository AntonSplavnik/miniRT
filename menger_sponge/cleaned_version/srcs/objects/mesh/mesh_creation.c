/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mesh_creation.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 16:00:00 by abillote          #+#    #+#             */
/*   Updated: 2025/05/10 16:15:00 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/**
 * Creates a new mesh object from an array of triangles
 * 
 * @param triangles Array of triangles
 * @param triangle_count Number of triangles
 * @param color Base color of the mesh
 * @return Pointer to the created object or NULL if allocation fails
 */
t_object	*create_mesh(t_triangle *triangles, int triangle_count, t_color color)
{
	t_object	*object;
	t_mesh		*mesh;

	object = malloc(sizeof(t_object));
	if (!object)
		return (NULL);
	mesh = malloc(sizeof(t_mesh));
	if (!mesh)
	{
		free(object);
		return (NULL);
	}
	mesh->triangles = triangles;
	mesh->triangle_count = triangle_count;
	mesh->position = vec3_create(0, 0, 0);  // Default position
	mesh->rotation = vec3_create(0, 0, 0);  // Default rotation
	mesh->scale = vec3_create(1, 1, 1);     // Default scale
	
	object->data = mesh;
	object->material = create_material(color);
	object->type = MESH;
	object->next = NULL;
	return (object);
}

/**
 * Creates a cube mesh composed of triangles
 * 
 * @param center Center position of the cube
 * @param size Size of the cube
 * @param color Color of the cube
 * @return Pointer to the created mesh object or NULL if allocation fails
 */
t_object	*create_cube_mesh(t_vec3 center, double size, t_color color)
{
	t_triangle	*triangles;
	t_vec3		vertices[8];
	int			i;
	double		half_size;
	
	half_size = size / 2.0;
	
	// Define the 8 vertices of the cube
	vertices[0] = vec3_create(center.x - half_size, center.y - half_size, center.z - half_size);
	vertices[1] = vec3_create(center.x + half_size, center.y - half_size, center.z - half_size);
	vertices[2] = vec3_create(center.x + half_size, center.y + half_size, center.z - half_size);
	vertices[3] = vec3_create(center.x - half_size, center.y + half_size, center.z - half_size);
	vertices[4] = vec3_create(center.x - half_size, center.y - half_size, center.z + half_size);
	vertices[5] = vec3_create(center.x + half_size, center.y - half_size, center.z + half_size);
	vertices[6] = vec3_create(center.x + half_size, center.y + half_size, center.z + half_size);
	vertices[7] = vec3_create(center.x - half_size, center.y + half_size, center.z + half_size);
	
	// Allocate memory for 12 triangles (6 faces, 2 triangles per face)
	triangles = malloc(sizeof(t_triangle) * 12);
	if (!triangles)
		return (NULL);
	
	// Front face (z = -half_size)
	triangles[0].v0 = vertices[0];
	triangles[0].v1 = vertices[1];
	triangles[0].v2 = vertices[2];
	triangles[1].v0 = vertices[0];
	triangles[1].v1 = vertices[2];
	triangles[1].v2 = vertices[3];
	
	// Back face (z = +half_size)
	triangles[2].v0 = vertices[5];
	triangles[2].v1 = vertices[4];
	triangles[2].v2 = vertices[7];
	triangles[3].v0 = vertices[5];
	triangles[3].v1 = vertices[7];
	triangles[3].v2 = vertices[6];
	
	// Left face (x = -half_size)
	triangles[4].v0 = vertices[4];
	triangles[4].v1 = vertices[0];
	triangles[4].v2 = vertices[3];
	triangles[5].v0 = vertices[4];
	triangles[5].v1 = vertices[3];
	triangles[5].v2 = vertices[7];
	
	// Right face (x = +half_size)
	triangles[6].v0 = vertices[1];
	triangles[6].v1 = vertices[5];
	triangles[6].v2 = vertices[6];
	triangles[7].v0 = vertices[1];
	triangles[7].v1 = vertices[6];
	triangles[7].v2 = vertices[2];
	
	// Bottom face (y = -half_size)
	triangles[8].v0 = vertices[4];
	triangles[8].v1 = vertices[5];
	triangles[8].v2 = vertices[1];
	triangles[9].v0 = vertices[4];
	triangles[9].v1 = vertices[1];
	triangles[9].v2 = vertices[0];
	
	// Top face (y = +half_size)
	triangles[10].v0 = vertices[3];
	triangles[10].v1 = vertices[2];
	triangles[10].v2 = vertices[6];
	triangles[11].v0 = vertices[3];
	triangles[11].v1 = vertices[6];
	triangles[11].v2 = vertices[7];
	
	// Calculate normals for all triangles
	for (i = 0; i < 12; i++)
	{
		t_vec3 edge1 = vec3_subtract(triangles[i].v1, triangles[i].v0);
		t_vec3 edge2 = vec3_subtract(triangles[i].v2, triangles[i].v0);
		triangles[i].normal = vec3_normalize(vec3_cross(edge1, edge2));
	}
	
	return (create_mesh(triangles, 12, color));
} 