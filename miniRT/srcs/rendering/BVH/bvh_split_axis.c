/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh_split_axis.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by abillote          #+#    #+#             */
/*   Updated: 2024/01/01 00:00:00 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

static void	update_min_max_centroids(t_vec3 *min_centroid, t_vec3 *max_centroid,
		t_vec3 current_centroid)
{
	if (current_centroid.x < min_centroid->x)
		min_centroid->x = current_centroid.x;
	if (current_centroid.y < min_centroid->y)
		min_centroid->y = current_centroid.y;
	if (current_centroid.z < min_centroid->z)
		min_centroid->z = current_centroid.z;
	if (current_centroid.x > max_centroid->x)
		max_centroid->x = current_centroid.x;
	if (current_centroid.y > max_centroid->y)
		max_centroid->y = current_centroid.y;
	if (current_centroid.z > max_centroid->z)
		max_centroid->z = current_centroid.z;
}

static int	determine_best_axis(t_vec3 extent)
{
	if (extent.x >= extent.y && extent.x >= extent.z)
		return (0);
	if (extent.y >= extent.z)
		return (1);
	return (2);
}

int	find_best_split_axis_fast(t_triangle_centroid *centroids,
		int start, int end)
{
	t_vec3	min_centroid;
	t_vec3	max_centroid;
	t_vec3	extent;
	int		i;

	min_centroid = centroids[start].centroid;
	max_centroid = centroids[start].centroid;
	i = start + 1;
	while (i <= end)
	{
		update_min_max_centroids(&min_centroid, &max_centroid,
			centroids[i].centroid);
		i++;
	}
	extent.x = max_centroid.x - min_centroid.x;
	extent.y = max_centroid.y - min_centroid.y;
	extent.z = max_centroid.z - min_centroid.z;
	return (determine_best_axis(extent));
}
