/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mesh_bvh.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 04:49:38 by antonsplavn       #+#    #+#             */
/*   Updated: 2025/07/21 04:49:39 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

t_aabb	calculate_transformed_tri_aabb(t_mesh *mesh, int tri_idx)
{
	t_aabb	box;
	t_vec3	v0;
	t_vec3	v1;
	t_vec3	v2;

	v0 = mesh->transformed_tris[tri_idx].v0;
	v1 = mesh->transformed_tris[tri_idx].v1;
	v2 = mesh->transformed_tris[tri_idx].v2;
	box.min.x = fmin(fmin(v0.x, v1.x), v2.x);
	box.min.y = fmin(fmin(v0.y, v1.y), v2.y);
	box.min.z = fmin(fmin(v0.z, v1.z), v2.z);
	box.max.x = fmax(fmax(v0.x, v1.x), v2.x);
	box.max.y = fmax(fmax(v0.y, v1.y), v2.y);
	box.max.z = fmax(fmax(v0.z, v1.z), v2.z);
	return (box);
}

static void	precompute_triangle_centroids(t_mesh *mesh,
				t_triangle_centroid *centroids)
{
	int						i;
	t_transformed_triangle	*tri;

	i = 0;
	while (i < mesh->triangle_count)
	{
		tri = &mesh->transformed_tris[i];
		centroids[i].centroid.x = (tri->v0.x + tri->v1.x + tri->v2.x) / 3.0;
		centroids[i].centroid.y = (tri->v0.y + tri->v1.y + tri->v2.y) / 3.0;
		centroids[i].centroid.z = (tri->v0.z + tri->v1.z + tri->v2.z) / 3.0;
		centroids[i].original_index = i;
		i++;
	}
}

static int	allocate_bvh_arrays(t_mesh *mesh)
{
	mesh->bvh.max_nodes = 2 * mesh->triangle_count - 1;
	mesh->bvh.nodes = malloc(mesh->bvh.max_nodes * sizeof(t_aabb));
	mesh->bvh.tri_indices = malloc(mesh->bvh.max_nodes * 2 * sizeof(int));
	if (!mesh->bvh.nodes || !mesh->bvh.tri_indices)
	{
		if (mesh->bvh.nodes)
			free(mesh->bvh.nodes);
		if (mesh->bvh.tri_indices)
			free(mesh->bvh.tri_indices);
		mesh->bvh.nodes = NULL;
		mesh->bvh.tri_indices = NULL;
		return (0);
	}
	return (1);
}

static int	setup_triangle_indices(t_mesh *mesh, t_triangle_centroid *centroids)
{
	int	*tri_indices;

	tri_indices = malloc(mesh->triangle_count * sizeof(int));
	if (!tri_indices)
		return (0);
	copy_indices_from_centroids(mesh, centroids, tri_indices);
	if (mesh->bvh.node_children)
		free(mesh->bvh.node_children);
	mesh->bvh.node_children = tri_indices;
	return (1);
}

void	build_mesh_bvh(t_mesh *mesh)
{
	t_triangle_centroid	*centroids;

	if (!mesh || mesh->triangle_count == 0)
		return ;
	mesh->bvh.nodes = NULL;
	mesh->bvh.tri_indices = NULL;
	mesh->bvh.node_children = NULL;
	mesh->bvh.node_count = 0;
	mesh->bvh.max_nodes = 0;
	centroids = malloc(mesh->triangle_count * sizeof(t_triangle_centroid));
	if (!centroids)
		return ;
	precompute_triangle_centroids(mesh, centroids);
	if (!allocate_bvh_arrays(mesh))
	{
		free(centroids);
		return ;
	}
	build_mesh_bvh_fast(mesh, centroids);
	if (!setup_triangle_indices(mesh, centroids))
	{
		free(centroids);
		return ;
	}
	free(centroids);
}
