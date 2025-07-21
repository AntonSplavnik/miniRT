/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mesh_bvh_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 05:03:46 by antonsplavn       #+#    #+#             */
/*   Updated: 2025/07/21 05:03:47 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

void	ft_swap_centroid(t_triangle_centroid *a, t_triangle_centroid *b)
{
	t_triangle_centroid	tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

t_aabb	aabb_union(t_aabb a, t_aabb b)
{
	t_aabb	res;

	res.min.x = fmin(a.min.x, b.min.x);
	res.min.y = fmin(a.min.y, b.min.y);
	res.min.z = fmin(a.min.z, b.min.z);
	res.max.x = fmax(a.max.x, b.max.x);
	res.max.y = fmax(a.max.y, b.max.y);
	res.max.z = fmax(a.max.z, b.max.z);
	return (res);
}

double	get_centroid_axis_value(t_triangle_centroid *centroid, int axis)
{
	if (axis == 0)
		return (centroid->centroid.x);
	if (axis == 1)
		return (centroid->centroid.y);
	return (centroid->centroid.z);
}

t_aabb	compute_bounds_from_centroids(t_mesh *mesh,
			t_triangle_centroid *centroids, t_centroid_range range)
{
	t_aabb	bounds;
	t_aabb	tri_bounds;
	int		i;

	bounds = calculate_transformed_tri_aabb(mesh,
			centroids[range.start].original_index);
	i = range.start + 1;
	while (i <= range.end)
	{
		tri_bounds = calculate_transformed_tri_aabb(mesh,
				centroids[i].original_index);
		bounds = aabb_union(bounds, tri_bounds);
		i++;
	}
	return (bounds);
}
