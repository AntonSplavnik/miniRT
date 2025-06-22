/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calculate_object_aabb_2.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 00:00:00 by antonsplavn      #+#    #+#             */
/*   Updated: 2025/06/23 00:00:00 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"
#include "../../includes/bvh.h"

t_aabb	calculate_cube_aabb(t_cube *cube)
{
	t_aabb	bounds;
	double	half_side;

	half_side = cube->side_length / 2.0;
	bounds.min = vec3_subtract(cube->center, vec3_create(half_side, half_side,
				half_side));
	bounds.max = vec3_add(cube->center, vec3_create(half_side, half_side,
				half_side));
	return (bounds);
}

t_aabb	calculate_cone_aabb(t_cone *cone)
{
	t_aabb	bounds;
	double	radius;
	double	height;
	t_vec3	apex;
	t_vec3	axis_scaled;
	t_vec3	base_center;

	radius = cone->radius;
	height = cone->height;
	apex = cone->apex;
	axis_scaled = vec3_scale(cone->axis, height);
	base_center = vec3_subtract(apex, axis_scaled);
	bounds.min.x = fmin(apex.x, base_center.x - radius);
	bounds.min.y = fmin(apex.y, base_center.y - radius);
	bounds.min.z = fmin(apex.z, base_center.z - radius);
	bounds.max.x = fmax(apex.x, base_center.x + radius);
	bounds.max.y = fmax(apex.y, base_center.y + radius);
	bounds.max.z = fmax(apex.z, base_center.z + radius);
	return (bounds);
}

t_aabb	calculate_triangle_aabb(t_triangle *triangle)
{
	t_aabb	bounds;
	double	epsilon;

	bounds.min.x = fmin(fmin(triangle->v0.x, triangle->v1.x), triangle->v2.x);
	bounds.min.y = fmin(fmin(triangle->v0.y, triangle->v1.y), triangle->v2.y);
	bounds.min.z = fmin(fmin(triangle->v0.z, triangle->v1.z), triangle->v2.z);
	bounds.max.x = fmax(fmax(triangle->v0.x, triangle->v1.x), triangle->v2.x);
	bounds.max.y = fmax(fmax(triangle->v0.y, triangle->v1.y), triangle->v2.y);
	bounds.max.z = fmax(fmax(triangle->v0.z, triangle->v1.z), triangle->v2.z);
	epsilon = 0.0001;
	if (bounds.max.x - bounds.min.x < epsilon)
	{
		bounds.min.x -= epsilon;
		bounds.max.x += epsilon;
	}
	if (bounds.max.y - bounds.min.y < epsilon)
	{
		bounds.min.y -= epsilon;
		bounds.max.y += epsilon;
	}
	if (bounds.max.z - bounds.min.z < epsilon)
	{
		bounds.min.z -= epsilon;
		bounds.max.z += epsilon;
	}
	return (bounds);
}
