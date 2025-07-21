/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_ray_intersect_bvh.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 00:00:00 by antonsplavn      #+#    #+#             */
/*   Updated: 2025/06/23 00:00:00 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"
#include "../../includes/bvh.h"

int	scene_ray_intersect_bvh(t_scene_ray_params params)
{
	t_bvh_node				*stack[64];
	int						stack_size;
	double					closest_t;
	int						hit_something;
	t_bvh_traverse_params	traverse_params;

	if (!params.scene->scene_bvh)
		return (find_closest_intersection(params.scene, params.ray,
				params.hit_record, params.hit_object));
	stack_size = 0;
	stack[stack_size++] = params.scene->scene_bvh;
	closest_t = INFINITY;
	traverse_params.ray_params = params;
	traverse_params.stack = stack;
	traverse_params.stack_size = &stack_size;
	traverse_params.closest_t = &closest_t;
	hit_something = traverse_bvh(traverse_params);
	if (hit_something)
		*(params.t) = closest_t;
	return (hit_something);
}
