/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mesh_bvh_intersect_helpers.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 21:30:00 by abillote          #+#    #+#             */
/*   Updated: 2025/07/18 18:35:23 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"
#include "../../includes/bvh.h"

int	initialize_bvh_traversal(t_mesh *mesh, t_ray ray,
		t_bvh_traverse_context *ctx)
{
	if (!mesh->bvh.nodes || mesh->bvh.node_count == 0)
		return (0);
	ctx->stack_ptr = 0;
	ctx->closest_t = INFINITY;
	ctx->params = (t_batch_params){0, 0, &ctx->closest_t, ctx->tri_idx, 0};
	ctx->stack_info = (t_bvh_stack_params){ctx->stack, &ctx->stack_ptr};
	ctx->proc_params = (t_bvh_process_params){mesh, ray, ctx->stack_info,
		&ctx->params};
	ctx->stack[ctx->stack_ptr++] = 0;
	return (1);
}

int	process_bvh_traversal(t_bvh_traverse_context *ctx)
{
	while (ctx->stack_ptr > 0)
	{
		ctx->params.hit_something = process_bvh_node_optimized(\
				&ctx->proc_params);
	}
	return (ctx->params.hit_something);
}

int	initialize_bvh_bary_traversal(t_mesh *mesh, t_ray ray,
		t_bvh_bary_traverse_context *ctx)
{
	if (!mesh->bvh.nodes || mesh->bvh.node_count == 0)
		return (0);
	ctx->stack_ptr = 0;
	ctx->closest_t = INFINITY;
	ctx->params = (t_intersect_bary_full){&ctx->closest_t, ctx->tri_idx,
		&ctx->closest_bary, 0};
	ctx->stack_info = (t_bvh_stack_params){ctx->stack, &ctx->stack_ptr};
	ctx->proc_params = (t_bvh_process_bary_params){mesh, ray,
		ctx->stack_info, &ctx->params};
	ctx->stack[ctx->stack_ptr++] = 0;
	return (1);
}

int	process_bvh_bary_traversal(t_bvh_bary_traverse_context *ctx)
{
	while (ctx->stack_ptr > 0)
	{
		ctx->params.hit_something = process_bvh_node_bary(&ctx->proc_params);
	}
	return (ctx->params.hit_something);
}
