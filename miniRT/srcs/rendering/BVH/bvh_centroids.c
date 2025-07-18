/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh_centroids.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 21:30:00 by abillote          #+#    #+#             */
/*   Updated: 2025/07/18 17:26:38 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"
#include "../../includes/bvh.h"

void	precompute_triangle_centroids(t_mesh *mesh,
			t_triangle_centroid *centroids)
{
	int						i;
	t_transformed_triangle	*tri;

	i = 0;
	while (i < mesh->triangle_count)
	{
		tri = &mesh->transformed_tris[i];
		centroids[i].centroid.x = (tri->v0.x + tri->v1.x + tri->v2.x) / 3.0;
		centroids[i].centroid.y = (tri->v0.y + tri->v1.y + tri->v2.y) / 3.0;
		centroids[i].centroid.z = (tri->v0.z + tri->v1.z + tri->v2.z) / 3.0;
		centroids[i].original_index = i;
		i++;
	}
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

t_aabb	compute_bounds_from_centroids(t_mesh *mesh,
			t_triangle_centroid *centroids, int start, int end)
{
	t_aabb	bounds;
	t_aabb	tri_bounds;
	int		i;

	bounds = calculate_transformed_tri_aabb(mesh,
			centroids[start].original_index);
	i = start + 1;
	while (i <= end)
	{
		tri_bounds = calculate_transformed_tri_aabb(mesh,
				centroids[i].original_index);
		bounds = aabb_union(bounds, tri_bounds);
		i++;
	}
	return (bounds);
}
