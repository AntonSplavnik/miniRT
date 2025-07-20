/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_scene_bvh_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 00:00:00 by antonsplavn      #+#    #+#             */
/*   Updated: 2025/06/23 00:00:00 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

void	count_scene_objects(t_scene *scene)
{
	t_object	*current;

	scene->object_count = 0;
	current = scene->objects;
	while (current)
	{
		scene->object_count++;
		current = current->next;
	}
}

void	fill_objects_and_bounds(t_scene *scene, t_object **objects,
						t_aabb *bounds, int object_count)
{
	t_object	*current;
	int			i;

	current = scene->objects;
	i = 0;
	while (i < object_count)
	{
		objects[i] = current;
		bounds[i] = calculate_object_aabb(current);
		current = current->next;
		i++;
	}
}
