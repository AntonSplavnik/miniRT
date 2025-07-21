/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mesh_bvh_helpers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 04:53:28 by antonsplavn       #+#    #+#             */
/*   Updated: 2025/07/21 04:53:29 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

static void	compute_centroid_bounds(t_triangle_centroid *centroids,
							t_centroid_range range, t_vec3 *min_centroid,
							t_vec3 *max_centroid)
{
	int	i;

	*min_centroid = centroids[range.start].centroid;
	*max_centroid = centroids[range.start].centroid;
	i = range.start + 1;
	while (i <= range.end)
	{
		if (centroids[i].centroid.x < min_centroid->x)
			min_centroid->x = centroids[i].centroid.x;
		if (centroids[i].centroid.x > max_centroid->x)
			max_centroid->x = centroids[i].centroid.x;
		if (centroids[i].centroid.y < min_centroid->y)
			min_centroid->y = centroids[i].centroid.y;
		if (centroids[i].centroid.y > max_centroid->y)
			max_centroid->y = centroids[i].centroid.y;
		if (centroids[i].centroid.z < min_centroid->z)
			min_centroid->z = centroids[i].centroid.z;
		if (centroids[i].centroid.z > max_centroid->z)
			max_centroid->z = centroids[i].centroid.z;
		i++;
	}
}

static int	select_axis_from_extent(t_vec3 extent)
{
	if (extent.y > extent.x && extent.y > extent.z)
		return (1);
	if (extent.z > extent.x)
		return (2);
	return (0);
}

int	find_best_split_axis(t_triangle_centroid *centroids,
		t_centroid_range range)
{
	t_vec3	min_centroid;
	t_vec3	max_centroid;
	t_vec3	extent;

	compute_centroid_bounds(centroids, range, &min_centroid, &max_centroid);
	extent = vec3_subtract(max_centroid, min_centroid);
	return (select_axis_from_extent(extent));
}

double	calculate_aabb_surface_area(t_aabb box)
{
	t_vec3	extent;

	extent = vec3_subtract(box.max, box.min);
	return (2.0 * (extent.x * extent.y + extent.y * extent.z
			+ extent.z * extent.x));
}

void	copy_indices_from_centroids(t_mesh *mesh,
			t_triangle_centroid *centroids, int *tri_indices)
{
	int	i;

	if (!mesh || !centroids || !tri_indices)
		return ;
	i = 0;
	while (i < mesh->triangle_count)
	{
		tri_indices[i] = centroids[i].original_index;
		i++;
	}
}
