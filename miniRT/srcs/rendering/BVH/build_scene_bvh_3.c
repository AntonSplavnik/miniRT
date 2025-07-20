/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_scene_bvh_3.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 00:00:00 by antonsplavn      #+#    #+#             */
/*   Updated: 2025/06/23 00:00:00 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

t_bvh_node	*build_scene_bvh(t_scene *scene)
{
	t_object	**objects;
	t_aabb		*bounds;
	t_bvh_node	*root;

	if (scene->object_count == 0)
		return (NULL);
	objects = malloc(sizeof(t_object *) * scene->object_count);
	bounds = malloc(sizeof(t_aabb) * scene->object_count);
	if (!objects || !bounds)
		return (NULL);
	fill_objects_and_bounds(scene, objects, bounds, scene->object_count);
	root = build_scene_bvh_recursive((t_bvh_params){objects, bounds, \
		0, scene->object_count - 1, 0});
	free(objects);
	free(bounds);
	return (root);
}
