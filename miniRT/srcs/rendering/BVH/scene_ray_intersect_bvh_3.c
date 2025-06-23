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

void	process_bvh_node(t_process_node_params params)
{
	double	node_tmin;
	double	node_tmax;

	if (!ray_intersect_aabb_scalar((t_ray_aabb_params){params.node->bounds,
			params.ray.origin, params.ray.direction, &node_tmin, &node_tmax}))
		return ;
	if (node_tmin > params.closest_t)
		return ;
	if (params.node->right)
		params.stack[(*(params.stack_size))++] = params.node->right;
	if (params.node->left)
		params.stack[(*(params.stack_size))++] = params.node->left;
}

int	traverse_bvh(t_bvh_traverse_params params)
{
	int			hit_something;
	t_bvh_node	*node;

	hit_something = 0;
	while (*(params.stack_size) > 0)
	{
		node = params.stack[--(*(params.stack_size))];
		if (node->is_leaf && node->object_ref)
			hit_something |= \
				test_leaf_node_intersection((t_leaf_intersection_params){
					node->object_ref, params.ray_params.ray, params.closest_t,
					params.ray_params.hit_object, \
					params.ray_params.hit_record});
		else
			process_bvh_node((t_process_node_params){
				node, params.ray_params.ray,
				*(params.closest_t), params.stack, params.stack_size});
	}
	return (hit_something);
}

int	scene_ray_intersect_bvh(t_scene_ray_params params)
{
	t_bvh_node	*stack[64];
	int			stack_size;
	double		closest_t;
	int			hit_something;

	if (!params.scene->scene_bvh)
		return (find_closest_intersection(params.scene, params.ray, params.t,
				params.hit_object, params.hit_record));
	stack_size = 0;
	stack[stack_size++] = params.scene->scene_bvh;
	closest_t = INFINITY;
	hit_something = traverse_bvh((t_bvh_traverse_params){
			params, stack, &stack_size, &closest_t});
	if (hit_something)
		*(params.t) = closest_t;
	return (hit_something);
}
