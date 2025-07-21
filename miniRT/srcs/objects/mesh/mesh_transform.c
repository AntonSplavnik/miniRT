/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mesh_transform.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 16:10:00 by antonsplavn       #+#    #+#             */
/*   Updated: 2025/07/18 16:10:31 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/miniRT.h"
#include "../../../includes/bvh.h"

/*
** Validates mesh data and allocates memory for transformed triangles
*/
static int	init_mesh_transformation(t_mesh *mesh)
{
	if (!mesh || !mesh->triangles || mesh->triangle_count <= 0)
		return (0);
	mesh->transformed_tris = malloc(mesh->triangle_count * \
		sizeof(t_transformed_triangle));
	if (!mesh->transformed_tris)
		return (0);
	return (1);
}

/*
** Transforms a single triangle vertices (scale, rotate, translate)
*/
static void	transform_triangle_vertices(t_mesh *mesh, int i)
{
	t_vec3	scaled_v0;
	t_vec3	scaled_v1;
	t_vec3	scaled_v2;

	scaled_v0 = vec3_multiply(mesh->triangles[i].v0, mesh->scale);
	scaled_v1 = vec3_multiply(mesh->triangles[i].v1, mesh->scale);
	scaled_v2 = vec3_multiply(mesh->triangles[i].v2, mesh->scale);
	mesh->transformed_tris[i].v0 = vec3_add(rotate_point(scaled_v0, \
		mesh->rotation), mesh->position);
	mesh->transformed_tris[i].v1 = vec3_add(rotate_point(scaled_v1, \
		mesh->rotation), mesh->position);
	mesh->transformed_tris[i].v2 = vec3_add(rotate_point(scaled_v2, \
		mesh->rotation), mesh->position);
}

/*
** Computes and stores the normal for a transformed triangle
*/
static void	compute_mesh_triangle_normal(t_mesh *mesh, int i)
{
	t_vec3	edge1;
	t_vec3	edge2;

	edge1 = vec3_subtract(mesh->transformed_tris[i].v1, \
		mesh->transformed_tris[i].v0);
	edge2 = vec3_subtract(mesh->transformed_tris[i].v2, \
		mesh->transformed_tris[i].v0);
	mesh->transformed_tris[i].normal = vec3_normalize(vec3_cross(edge1, \
		edge2));
}

/*
** Transforms vertex normals for smooth shading
*/
static void	transform_vertex_normals(t_mesh *mesh, int i)
{
	mesh->transformed_tris[i].has_vertex_normals = \
		mesh->triangles[i].has_vertex_normals;
	if (mesh->triangles[i].has_vertex_normals)
	{
		mesh->transformed_tris[i].n0 = vec3_normalize(rotate_point(\
			mesh->triangles[i].n0, mesh->rotation));
		mesh->transformed_tris[i].n1 = vec3_normalize(rotate_point(\
			mesh->triangles[i].n1, mesh->rotation));
		mesh->transformed_tris[i].n2 = vec3_normalize(rotate_point(\
			mesh->triangles[i].n2, mesh->rotation));
	}
}

/*
** Precomputes transformed triangles for mesh rendering
*/
void	precompute_transformed_triangles(t_mesh *mesh)
{
	int	i;

	if (!init_mesh_transformation(mesh))
		return ;
	i = 0;
	while (i < mesh->triangle_count)
	{
		transform_triangle_vertices(mesh, i);
		compute_mesh_triangle_normal(mesh, i);
		transform_vertex_normals(mesh, i);
		i++;
	}
	build_mesh_bvh(mesh);
}
