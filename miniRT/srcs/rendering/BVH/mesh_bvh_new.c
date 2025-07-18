/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mesh_bvh.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 21:30:00 by abillote          #+#    #+#             */
/*   Updated: 2025/01/15 21:30:00 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"
#include "../../includes/bvh.h"

static void	initialize_bvh_structure(t_mesh *mesh)
{
	mesh->bvh.nodes = NULL;
	mesh->bvh.tri_indices = NULL;
	mesh->bvh.node_children = NULL;
	mesh->bvh.node_count = 0;
	mesh->bvh.max_nodes = 0;
}

static void	cleanup_on_failure(t_triangle_centroid *centroids, t_mesh *mesh)
{
	free(centroids);
	if (mesh->bvh.nodes)
		free(mesh->bvh.nodes);
	if (mesh->bvh.tri_indices)
		free(mesh->bvh.tri_indices);
	mesh->bvh.nodes = NULL;
	mesh->bvh.tri_indices = NULL;
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

static int	build_bvh_core(t_mesh *mesh)
{
	t_triangle_centroid	*centroids;

	centroids = malloc(mesh->triangle_count * sizeof(t_triangle_centroid));
	if (!centroids)
		return (0);
	precompute_triangle_centroids(mesh, centroids);
	if (!allocate_bvh_memory(mesh))
	{
		cleanup_on_failure(centroids, mesh);
		return (0);
	}
	build_mesh_bvh_fast(mesh, centroids);
	if (!setup_triangle_indices(mesh, centroids))
	{
		cleanup_on_failure(centroids, mesh);
		return (0);
	}
	free(centroids);
	return (1);
}

void	build_mesh_bvh(t_mesh *mesh)
{
	if (!mesh || mesh->triangle_count == 0)
		return ;
	initialize_bvh_structure(mesh);
	build_bvh_core(mesh);
}
