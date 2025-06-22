/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calculate_object_aabb.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 00:00:00 by antonsplavn      #+#    #+#             */
/*   Updated: 2025/06/23 00:00:00 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"
#include "../../includes/bvh.h"

t_aabb	calculate_sphere_aabb(t_sphere *sphere)
{
	t_aabb	bounds;
	double	radius;

	radius = sphere->radius;
	bounds.min = vec3_subtract(sphere->center, vec3_create(radius, radius,
				radius));
	bounds.max = vec3_add(sphere->center, vec3_create(radius, radius, radius));
	return (bounds);
}

t_aabb	calculate_plane_aabb(t_plane *plane)
{
	t_aabb	bounds;
	double	size;

	size = 1000.0;
	bounds.min = vec3_subtract(plane->point, vec3_create(size, size, size));
	bounds.max = vec3_add(plane->point, vec3_create(size, size, size));
	return (bounds);
}

t_aabb	calculate_cylinder_aabb(t_cylinder *cylinder)
{
	t_aabb	bounds;
	double	radius;
	double	half_height;
	t_vec3	axis_scaled;
	t_vec3	top;
	t_vec3	bottom;

	radius = cylinder->radius;
	half_height = cylinder->height / 2.0;
	axis_scaled = vec3_scale(cylinder->axis, half_height);
	top = vec3_add(cylinder->center, axis_scaled);
	bottom = vec3_subtract(cylinder->center, axis_scaled);
	bounds.min.x = fmin(top.x - radius, bottom.x - radius);
	bounds.min.y = fmin(top.y - radius, bottom.y - radius);
	bounds.min.z = fmin(top.z - radius, bottom.z - radius);
	bounds.max.x = fmax(top.x + radius, bottom.x + radius);
	bounds.max.y = fmax(top.y + radius, bottom.y + radius);
	bounds.max.z = fmax(top.z + radius, bottom.z + radius);
	return (bounds);
}
