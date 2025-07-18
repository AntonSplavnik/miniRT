/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 21:30:00 by abillote          #+#    #+#             */
/*   Updated: 2025/01/15 21:30:00 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"
#include "../../includes/bvh.h"

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

double	calculate_aabb_surface_area(t_aabb box)
{
	t_vec3	extent;

	extent = vec3_subtract(box.max, box.min);
	return (2.0 * (extent.x * extent.y + extent.y * extent.z
			+ extent.z * extent.x));
}

double	get_centroid_axis_value(t_triangle_centroid *centroid, int axis)
{
	if (axis == 0)
		return (centroid->centroid.x);
	if (axis == 1)
		return (centroid->centroid.y);
	return (centroid->centroid.z);
}

t_aabb	calculate_transformed_tri_aabb(t_mesh *mesh, int tri_idx)
{
	t_aabb	box;
	t_vec3	v0;
	t_vec3	v1;
	t_vec3	v2;

	v0 = mesh->transformed_tris[tri_idx].v0;
	v1 = mesh->transformed_tris[tri_idx].v1;
	v2 = mesh->transformed_tris[tri_idx].v2;
	box.min.x = fmin(fmin(v0.x, v1.x), v2.x);
	box.min.y = fmin(fmin(v0.y, v1.y), v2.y);
	box.min.z = fmin(fmin(v0.z, v1.z), v2.z);
	box.max.x = fmax(fmax(v0.x, v1.x), v2.x);
	box.max.y = fmax(fmax(v0.y, v1.y), v2.y);
	box.max.z = fmax(fmax(v0.z, v1.z), v2.z);
	return (box);
}
