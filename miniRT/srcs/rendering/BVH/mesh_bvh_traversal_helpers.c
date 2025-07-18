/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mesh_bvh_traversal_helpers.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 21:30:00 by abillote          #+#    #+#             */
/*   Updated: 2025/01/15 21:30:00 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"
#include "../../includes/bvh.h"

int	handle_leaf_node(t_bvh_process_params *proc_params,
		int left_index, int right_index)
{
	t_leaf_batch_context	ctx;

	ctx = (t_leaf_batch_context){left_index, right_index,
		proc_params->batch_params};
	return (process_leaf_batch(proc_params->mesh, proc_params->ray, &ctx));
}

void	push_internal_nodes(t_bvh_process_params *proc_params,
		int left_index, int right_index)
{
	if (*proc_params->stack_info.stack_ptr < 64 - 2)
	{
		proc_params->stack_info.stack[
			(*proc_params->stack_info.stack_ptr)++] = -right_index;
		proc_params->stack_info.stack[
			(*proc_params->stack_info.stack_ptr)++] = -left_index;
	}
}

int	handle_leaf_node_bary(t_bvh_process_bary_params *proc_params,
		int left_index, int right_index)
{
	t_leaf_batch_bary_params	leaf_params;

	leaf_params = (t_leaf_batch_bary_params){left_index, right_index,
		proc_params->bary_params};
	return (test_leaf_triangles_batch_bary(proc_params->mesh,
			proc_params->ray, &leaf_params));
}

void	push_internal_nodes_bary(t_bvh_process_bary_params *proc_params,
		int left_index, int right_index)
{
	if (*proc_params->stack_info.stack_ptr < 64 - 2)
	{
		proc_params->stack_info.stack[
			(*proc_params->stack_info.stack_ptr)++] = -right_index;
		proc_params->stack_info.stack[
			(*proc_params->stack_info.stack_ptr)++] = -left_index;
	}
}

int	process_leaf_batch(t_mesh *mesh, t_ray ray,
		t_leaf_batch_context *ctx)
{
	t_leaf_batch_params	leaf_params;

	leaf_params = (t_leaf_batch_params){ctx->left_index, ctx->right_index,
		ctx->params->closest_t, ctx->params->tri_idx,
		ctx->params->hit_something};
	return (test_leaf_triangles_batch(mesh, ray, &leaf_params));
}
