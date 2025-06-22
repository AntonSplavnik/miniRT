/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   build_scene_bvh.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 00:00:00 by antonsplavn      #+#    #+#             */
/*   Updated: 2025/06/23 00:00:00 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

void	swap_objects(t_object **objects, t_aabb *bounds, int i, int j)
{
	t_object	*temp_obj;
	t_aabb		temp_bounds;

	temp_obj = objects[i];
	objects[i] = objects[j];
	objects[j] = temp_obj;
	temp_bounds = bounds[i];
	bounds[i] = bounds[j];
	bounds[j] = temp_bounds;
}

double	get_centroid_by_axis(t_aabb bounds, int axis)
{
	if (axis == 0)
		return ((bounds.min.x + bounds.max.x) * 0.5);
	else if (axis == 1)
		return ((bounds.min.y + bounds.max.y) * 0.5);
	else
		return ((bounds.min.z + bounds.max.z) * 0.5);
}

void	sort_objects_by_axis(t_sort_params params)
{
	int			i;
	int			j;
	double		centroid_i;
	double		centroid_j;

	i = params.start;
	while (i <= params.end)
	{
		j = i + 1;
		while (j <= params.end)
		{
			centroid_i = get_centroid_by_axis(params.bounds[i], params.axis);
			centroid_j = get_centroid_by_axis(params.bounds[j], params.axis);
			if (centroid_i > centroid_j)
				swap_objects(params.objects, params.bounds, i, j);
			j++;
		}
		i++;
	}
}
