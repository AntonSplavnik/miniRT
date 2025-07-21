/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mesh_bvh_intersect.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 21:30:00 by abillote          #+#    #+#             */
/*   Updated: 2025/07/21 10:50:27 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"
#include "../../includes/bvh.h"

int	process_bvh_node_optimized(t_bvh_process_params *proc_params)
{
	int						node_index;
	t_aabb					node_bounds;
	int						left_index;
	int						right_index;

	node_index = proc_params->stack_info.stack[
		--(*proc_params->stack_info.stack_ptr)];
	node_bounds = proc_params->mesh->bvh.nodes[node_index];
	if (!ray_intersect_aabb_optimized(proc_params->ray, node_bounds,
			*proc_params->batch_params->closest_t))
		return (proc_params->batch_params->hit_something);
	left_index = proc_params->mesh->bvh.tri_indices[node_index * 2];
	right_index = proc_params->mesh->bvh.tri_indices[node_index * 2 + 1];
	if (left_index >= 0)
		return (handle_leaf_node(proc_params, left_index, right_index));
	push_internal_nodes(proc_params, left_index, right_index);
	return (proc_params->batch_params->hit_something);
}

int	mesh_bvh_intersect(t_ray ray, t_mesh *mesh, double *t, int *tri_idx)
{
	t_bvh_traverse_context	ctx;

	ctx.tri_idx = tri_idx;
	if (!initialize_bvh_traversal(mesh, ray, &ctx))
		return (0);
	if (process_bvh_traversal(&ctx))
	{
		*t = ctx.closest_t;
		return (1);
	}
	return (0);
}

int	process_bvh_node_bary(t_bvh_process_bary_params *proc_params)
{
	int							node_index;
	t_aabb						node_bounds;
	int							left_index;
	int							right_index;

	node_index = proc_params->stack_info.stack[
		--(*proc_params->stack_info.stack_ptr)];
	node_bounds = proc_params->mesh->bvh.nodes[node_index];
	if (!ray_intersect_aabb_optimized(proc_params->ray, node_bounds,
			*proc_params->bary_params->closest_t))
		return (proc_params->bary_params->hit_something);
	left_index = proc_params->mesh->bvh.tri_indices[node_index * 2];
	right_index = proc_params->mesh->bvh.tri_indices[node_index * 2 + 1];
	if (left_index >= 0)
		return (handle_leaf_node_bary(proc_params, left_index, right_index));
	push_internal_nodes_bary(proc_params, left_index, right_index);
	return (proc_params->bary_params->hit_something);
}

int	mesh_bvh_intersect_bary(t_ray ray, t_mesh *mesh, \
		t_bvh_bary_params *bary_params)
{
	t_bvh_bary_traverse_context	ctx;

	ctx.tri_idx = bary_params->tri_idx;
	if (!initialize_bvh_bary_traversal(mesh, ray, &ctx))
		return (0);
	if (process_bvh_bary_traversal(&ctx))
	{
		*bary_params->t = ctx.closest_t;
		*bary_params->bary = ctx.closest_bary;
		return (1);
	}
	return (0);
}
