/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_scene_bvh_2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 00:00:00 by antonsplavn      #+#    #+#             */
/*   Updated: 2025/06/23 00:00:00 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

void	compute_total_bounds(t_aabb *total_bounds, t_aabb *bounds,
							int start, int end)
{
	int		i;

	*total_bounds = bounds[start];
	i = start + 1;
	while (i <= end)
	{
		total_bounds->min.x = fmin(total_bounds->min.x, bounds[i].min.x);
		total_bounds->min.y = fmin(total_bounds->min.y, bounds[i].min.y);
		total_bounds->min.z = fmin(total_bounds->min.z, bounds[i].min.z);
		total_bounds->max.x = fmax(total_bounds->max.x, bounds[i].max.x);
		total_bounds->max.y = fmax(total_bounds->max.y, bounds[i].max.y);
		total_bounds->max.z = fmax(total_bounds->max.z, bounds[i].max.z);
		i++;
	}
}

int	find_split_axis(t_aabb total_bounds)
{
	double	x_extent;
	double	y_extent;
	double	z_extent;
	int		axis;

	x_extent = total_bounds.max.x - total_bounds.min.x;
	y_extent = total_bounds.max.y - total_bounds.min.y;
	z_extent = total_bounds.max.z - total_bounds.min.z;
	axis = 0;
	if (y_extent > x_extent && y_extent > z_extent)
		axis = 1;
	else if (z_extent > x_extent && z_extent > y_extent)
		axis = 2;
	return (axis);
}

t_bvh_node	*build_scene_bvh_recursive(t_bvh_params params)
{
	t_bvh_node	*node;
	t_aabb		total_bounds;
	int			axis;

	if (params.start > params.end)
		return (NULL);
	if (params.start == params.end)
		return (create_leaf_node(params.objects, params.bounds, params.start));
	node = create_bvh_node();
	if (!node)
		return (NULL);
	compute_total_bounds(&total_bounds, \
			params.bounds, params.start, params.end);
	node->bounds = total_bounds;
	axis = find_split_axis(total_bounds);
	sort_objects_by_axis((t_sort_params){params.objects, params.bounds, \
			params.start, params.end, axis});
	build_node_children((t_node_children_params){node, params.objects, \
		params.bounds, params.start, params.end, params.depth});
	return (node);
}
