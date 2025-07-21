/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mesh_bvh_split.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 04:50:31 by antonsplavn       #+#    #+#             */
/*   Updated: 2025/07/21 05:30:00 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

static int	calculate_split_position(t_mesh_split_params params,
				t_centroid_range range)
{
	int		best_axis;
	double	threshold;
	int		split_pos;

	best_axis = find_best_split_axis(params.centroids, range);
	threshold = (get_centroid_axis_value(&params.centroids[range.start], \
			best_axis) + get_centroid_axis_value(&params.centroids[range.end], \
			best_axis)) / 2.0;
	split_pos = partition_by_threshold((t_partition_params){params.centroids,
			best_axis, threshold}, range);
	if (split_pos == range.start || split_pos > range.end)
		split_pos = range.start + (range.end - range.start) / 2;
	return (split_pos);
}

static int	setup_child_nodes(t_mesh_split_params params, int node_index,
				int *left_index, int *right_index)
{
	*left_index = (*params.node_count)++;
	*right_index = (*params.node_count)++;
	if (*left_index >= params.mesh->bvh.max_nodes
		|| *right_index >= params.mesh->bvh.max_nodes)
		return (0);
	params.mesh->bvh.tri_indices[node_index * 2] = -*left_index;
	params.mesh->bvh.tri_indices[node_index * 2 + 1] = -*right_index;
	return (1);
}

static int	split_node_fast(t_mesh_split_params params, t_centroid_range range,
				int node_index, int stack_ptr)
{
	int	split_pos;
	int	left_index;
	int	right_index;

	split_pos = calculate_split_position(params, range);
	if (!setup_child_nodes(params, node_index, &left_index, &right_index))
		return (stack_ptr);
	if (stack_ptr < MAX_STACK_SIZE - 2)
	{
		params.stack[stack_ptr++] = (t_build_node){split_pos, range.end,
			right_index};
		params.stack[stack_ptr++] = (t_build_node){range.start, split_pos - 1,
			left_index};
	}
	return (stack_ptr);
}

static int	process_build_node_fast(t_mesh_split_params params, int stack_ptr)
{
	t_build_node		current;
	t_aabb				node_bounds;
	t_centroid_range	range;
	int					tri_count;

	current = params.stack[--stack_ptr];
	range.start = current.start;
	range.end = current.end;
	tri_count = range.end - range.start + 1;
	if (current.node_index >= params.mesh->bvh.max_nodes)
		return (stack_ptr);
	node_bounds = compute_bounds_from_centroids(params.mesh, params.centroids,
			range);
	params.mesh->bvh.nodes[current.node_index] = node_bounds;
	if (tri_count <= LEAF_TRI_THRESHOLD)
	{
		params.mesh->bvh.tri_indices[current.node_index * 2] = range.start;
		params.mesh->bvh.tri_indices[current.node_index * 2 + 1] = tri_count;
		return (stack_ptr);
	}
	return (split_node_fast(params, range, current.node_index, stack_ptr));
}

void	build_mesh_bvh_fast(t_mesh *mesh, t_triangle_centroid *centroids)
{
	t_build_node		stack[MAX_STACK_SIZE];
	t_mesh_split_params	params;
	int					stack_ptr;
	int					node_count;

	stack_ptr = 0;
	node_count = 1;
	params.mesh = mesh;
	params.centroids = centroids;
	params.stack = stack;
	params.node_count = &node_count;
	stack[stack_ptr++] = (t_build_node){0, mesh->triangle_count - 1, 0};
	while (stack_ptr > 0)
		stack_ptr = process_build_node_fast(params, stack_ptr);
	mesh->bvh.node_count = node_count;
}
