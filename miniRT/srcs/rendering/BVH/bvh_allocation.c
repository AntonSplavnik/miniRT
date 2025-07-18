/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh_allocation.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by abillote          #+#    #+#             */
/*   Updated: 2024/01/01 00:00:00 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

int	validate_bvh_allocation(t_mesh *mesh)
{
	if (!mesh->bvh.nodes || !mesh->bvh.tri_indices)
		return (0);
	return (1);
}

int	allocate_bvh_memory(t_mesh *mesh)
{
	mesh->bvh.max_nodes = 2 * mesh->triangle_count - 1;
	mesh->bvh.nodes = malloc(mesh->bvh.max_nodes * sizeof(t_aabb));
	mesh->bvh.tri_indices = malloc(mesh->bvh.max_nodes * 2 * sizeof(int));
	if (!validate_bvh_allocation(mesh))
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
