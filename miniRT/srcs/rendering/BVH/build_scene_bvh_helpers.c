/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_scene_bvh_helpers.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 00:00:00 by antonsplavn      #+#    #+#             */
/*   Updated: 2025/06/23 00:00:00 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

t_bvh_node	*create_bvh_node(void)
{
	t_bvh_node	*node;

	node = malloc(sizeof(t_bvh_node));
	if (!node)
		return (NULL);
	node->left = NULL;
	node->right = NULL;
	node->is_leaf = 0;
	node->iteration = 0;
	node->object_ref = NULL;
	return (node);
}

t_bvh_node	*create_leaf_node(t_object **objects, t_aabb *bounds, int start)
{
	t_bvh_node	*node;

	node = create_bvh_node();
	if (!node)
		return (NULL);
	node->is_leaf = 1;
	node->object_ref = objects[start];
	node->bounds = bounds[start];
	return (node);
}

void	build_node_children(t_node_children_params params)
{
	int				mid;
	t_bvh_params	bvh_params;

	mid = params.start + (params.end - params.start) / 2;
	bvh_params.objects = params.objects;
	bvh_params.bounds = params.bounds;
	bvh_params.start = params.start;
	bvh_params.end = mid;
	bvh_params.depth = params.depth + 1;
	params.node->left = build_scene_bvh_recursive(bvh_params);
	bvh_params.start = mid + 1;
	bvh_params.end = params.end;
	params.node->right = build_scene_bvh_recursive(bvh_params);
}
