/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh_helpers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by abillote          #+#    #+#             */
/*   Updated: 2025/07/21 10:30:06 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

int	get_triangle_original_index(t_mesh *mesh, int idx)
{
	if (mesh->bvh.node_children && idx < mesh->triangle_count)
		return (mesh->bvh.node_children[idx]);
	return (idx);
}

int	is_valid_triangle_index(int original_idx, int triangle_count)
{
	return (original_idx >= 0 && original_idx < triangle_count);
}

// int	setup_node_bounds(t_process_build_params *build_params,
// 		t_split_params *params)
// {
// 	t_aabb	node_bounds;

// 	if (params->node_index >= build_params->mesh->bvh.max_nodes)
// 		return (build_params->stack_ptr);
// 	node_bounds = compute_bounds_from_centroids(build_params->mesh,
// 			build_params->centroids, params->start, params->end);
// 	build_params->mesh->bvh.nodes[params->node_index] = node_bounds;
// 	return (-1);
// }

// int	handle_leaf_or_split(t_process_build_params *build_params,
// 		t_split_params *params, int tri_count)
// {
// 	t_split_node_params	split_params;

// 	if (tri_count <= LEAF_TRI_THRESHOLD)
// 	{
// 		process_leaf_node(build_params->mesh, params);
// 		return (build_params->stack_ptr);
// 	}
// 	params->node_count = build_params->node_count;
// 	split_params = (t_split_node_params){build_params->mesh,
// 		build_params->centroids, build_params->stack,
// 		build_params->stack_ptr, params};
// 	return (split_node_fast(&split_params));
// }

// void	process_leaf_node(t_mesh *mesh, t_split_params *params)
// {
// 	mesh->bvh.tri_indices[params->node_index * 2] = params->start;
// 	mesh->bvh.tri_indices[params->node_index * 2 + 1] = params->end
// 		- params->start + 1;
// }
