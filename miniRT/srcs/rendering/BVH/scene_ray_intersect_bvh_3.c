/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_ray_intersect_bvh_3.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 00:00:00 by antonsplavn      #+#    #+#             */
/*   Updated: 2025/06/23 00:00:00 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"
#include "../../includes/bvh.h"

static void	process_node_aabb_test(t_bvh_node *node, t_ray ray,
			t_ray_aabb_params *ray_params, double *t_vals)
{
	ray_params->bounds = node->bounds;
	ray_params->ray_origin = ray.origin;
	ray_params->ray_dir = ray.direction;
	ray_params->t_min = &t_vals[0];
	ray_params->t_max = &t_vals[1];
}

void	process_bvh_node(t_process_node_params params)
{
	double				t_vals[2];
	t_bvh_node			*right;
	t_bvh_node			*left;
	int					*size_ptr;
	t_ray_aabb_params	ray_params;

	process_node_aabb_test(params.node, params.ray, &ray_params, t_vals);
	if (!ray_intersect_aabb_scalar(ray_params))
		return ;
	if (t_vals[0] > params.closest_t)
		return ;
	right = params.node->right;
	left = params.node->left;
	size_ptr = params.stack_size;
	if (left)
		params.stack[(*size_ptr)++] = left;
	if (right)
		params.stack[(*size_ptr)++] = right;
}

static void	init_traverse_params(t_bvh_traverse_params *params,
				t_leaf_intersection_params *leaf_params,
				t_process_node_params *node_params)
{
	t_ray	ray;
	double	closest_t_value;

	ray = params->ray_params.ray;
	closest_t_value = *(params->closest_t);
	leaf_params->ray = ray;
	leaf_params->closest_t = params->closest_t;
	leaf_params->hit_object = params->ray_params.hit_object;
	leaf_params->hit_record = params->ray_params.hit_record;
	node_params->ray = ray;
	node_params->closest_t = closest_t_value;
	node_params->stack = params->stack;
	node_params->stack_size = params->stack_size;
}

static int	process_bvh_stack(t_bvh_traverse_params *params,
				t_leaf_intersection_params *leaf_params,
				t_process_node_params *node_params)
{
	int			hit_something;
	t_bvh_node	*node;

	hit_something = 0;
	while (*(params->stack_size) > 0)
	{
		node = params->stack[--*(params->stack_size)];
		if (node->is_leaf && node->object_ref)
		{
			leaf_params->obj = node->object_ref;
			if (test_leaf_node_intersection(*leaf_params))
				hit_something = 1;
			node_params->closest_t = *(params->closest_t);
		}
		else
		{
			node_params->node = node;
			process_bvh_node(*node_params);
		}
	}
	return (hit_something);
}

int	traverse_bvh(t_bvh_traverse_params params)
{
	t_leaf_intersection_params	leaf_params;
	t_process_node_params		node_params;
	int							hit_something;

	init_traverse_params(&params, &leaf_params, &node_params);
	hit_something = process_bvh_stack(&params, &leaf_params, &node_params);
	return (hit_something);
}
