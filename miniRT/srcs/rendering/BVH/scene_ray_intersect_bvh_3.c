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

void	process_bvh_node(t_bvh_node *node, t_ray ray, double closest_t,
					t_bvh_node **stack, int *stack_size)
{
	double	node_tmin;
	double	node_tmax;

	if (!ray_intersect_aabb_scalar(node->bounds, ray.origin, ray.direction,
			&node_tmin, &node_tmax))
		return ;
	if (node_tmin > closest_t)
		return ;
	if (node->right)
		stack[(*stack_size)++] = node->right;
	if (node->left)
		stack[(*stack_size)++] = node->left;
}

int	scene_ray_intersect_bvh(t_scene *scene, t_ray ray, double *t,
					t_object **hit_object, t_hit_record *hit_record)
{
	t_bvh_node	*stack[64];
	int			stack_size;
	double		closest_t;
	int			hit_something;
	t_bvh_node	*node;

	if (!scene->scene_bvh)
		return (find_closest_intersection(scene, ray, t, hit_object,
				hit_record));
	stack_size = 0;
	stack[stack_size++] = scene->scene_bvh;
	closest_t = INFINITY;
	hit_something = 0;
	while (stack_size > 0)
	{
		node = stack[--stack_size];
		if (node->is_leaf && node->object_ref)
		{
			hit_something |= test_leaf_node_intersection(node->object_ref, ray,
					&closest_t, hit_object, hit_record);
		}
		else
		{
			process_bvh_node(node, ray, closest_t, stack, &stack_size);
		}
	}
	if (hit_something)
		*t = closest_t;
	return (hit_something);
}
