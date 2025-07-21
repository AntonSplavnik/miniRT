/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_intersaction.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 12:00:00 by abillote          #+#    #+#             */
/*   Updated: 2025/07/21 02:19:40 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

/*
** Find the closest intersection between a ray and all objects in the scene
*/
int	find_closest_intersection(t_scene *scene, t_ray ray,
	t_hit_record *hit_record, t_object **hit_object)
{
	t_object				*current;
	double					t_temp;
	t_intersection_data		data;

	if (scene->scene_bvh)
		return (scene_ray_intersect_bvh((t_scene_ray_params){scene, ray, \
			&hit_record->t, hit_object, hit_record}));
	current = scene->objects;
	data.t_closest = INFINITY;
	data.hit_something = 0;
	data.hit_object = NULL;
	while (current)
	{
		if (test_basic_shapes(current, ray, &t_temp)
			|| test_cube_cone(current, ray, &t_temp)
			|| test_triangle_mesh_bary(current, ray, &t_temp, hit_record))
			update_closest_hit(t_temp, &data, current);
		current = current->next;
	}
	if (data.hit_something)
	{
		hit_record->t = data.t_closest;
		*hit_object = data.hit_object;
	}
	return (data.hit_something);
}
