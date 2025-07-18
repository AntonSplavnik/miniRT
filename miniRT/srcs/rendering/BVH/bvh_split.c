/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh_split.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 21:30:00 by abillote          #+#    #+#             */
/*   Updated: 2025/07/18 18:13:58 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"
#include "../../includes/bvh.h"

static int	assign_child_nodes(t_split_node_params \
		*split_params, int split_pos);

static int	push_child_nodes(t_split_node_params *split_params, \
		int split_pos);

int	split_node_fast(t_split_node_params *split_params)
{
	int					best_axis;
	double				threshold;
	int					split_pos;
	t_partition_params	part_params;

	best_axis = find_best_split_axis_fast(split_params->centroids,
			split_params->params->start, split_params->params->end);
	threshold = (get_centroid_axis_value(&split_params->centroids[
				split_params->params->start], best_axis)
			+ get_centroid_axis_value(&split_params->centroids[
				split_params->params->end], best_axis)) / 2.0;
	part_params = (t_partition_params){split_params->params->start,
		split_params->params->end, best_axis, threshold};
	split_pos = partition_by_threshold_fast(split_params->centroids,
			&part_params);
	if (split_pos == split_params->params->start
		|| split_pos > split_params->params->end)
		split_pos = split_params->params->start
			+ (split_params->params->end - split_params->params->start) / 2;
	return (assign_child_nodes(split_params, split_pos));
}

static int	assign_child_nodes(t_split_node_params *split_params, int split_pos)
{
	t_mesh_bvh	*bvh;
	int			node_idx;

	bvh = &split_params->mesh->bvh;
	node_idx = split_params->params->node_index;
	split_params->params->left_index = (*split_params->params->node_count)++;
	split_params->params->right_index = (*split_params->params->node_count)++;
	if (split_params->params->left_index >= bvh->max_nodes
		|| split_params->params->right_index >= bvh->max_nodes)
		return (split_params->stack_ptr);
	bvh->tri_indices[node_idx * 2] = -split_params->params->left_index;
	bvh->tri_indices[node_idx * 2 + 1] = -split_params->params->right_index;
	return (push_child_nodes(split_params, split_pos));
}

static int	push_child_nodes(t_split_node_params *split_params, int split_pos)
{
	if (split_params->stack_ptr < MAX_STACK_SIZE - 2)
	{
		split_params->stack[split_params->stack_ptr++] = (t_build_node){
			split_pos, split_params->params->end,
			split_params->params->right_index};
		split_params->stack[split_params->stack_ptr++] = (t_build_node){
			split_params->params->start, split_pos - 1,
			split_params->params->left_index};
	}
	return (split_params->stack_ptr);
}

int	process_build_node_fast(t_process_build_params *build_params)
{
	t_build_node	current;
	int				tri_count;
	t_split_params	params;
	int				bounds_result;

	current = build_params->stack[--build_params->stack_ptr];
	params.start = current.start;
	params.end = current.end;
	params.node_index = current.node_index;
	tri_count = params.end - params.start + 1;
	bounds_result = setup_node_bounds(build_params, &params);
	if (bounds_result != -1)
		return (bounds_result);
	return (handle_leaf_or_split(build_params, &params, tri_count));
}

void	build_mesh_bvh_fast(t_mesh *mesh, t_triangle_centroid *centroids)
{
	t_build_node			stack[MAX_STACK_SIZE];
	int						stack_ptr;
	int						node_count;
	t_process_build_params	build_params;

	stack_ptr = 0;
	node_count = 1;
	stack[stack_ptr++] = (t_build_node){0, mesh->triangle_count - 1, 0};
	while (stack_ptr > 0)
	{
		build_params = (t_process_build_params){mesh, centroids, stack,
			stack_ptr, &node_count};
		stack_ptr = process_build_node_fast(&build_params);
	}
	mesh->bvh.node_count = node_count;
}
