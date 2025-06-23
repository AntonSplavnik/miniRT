/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_intersect_aabb_2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 00:00:00 by antonsplavn      #+#    #+#             */
/*   Updated: 2025/06/23 00:00:00 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"
#include "../../includes/bvh.h"

int	check_x_dimension(t_aabb bounds, t_vec3 ray_origin,
					t_vec3 ray_dir, double *near_far)
{
	double	epsilon;

	epsilon = 1e-6;
	if (fabs(ray_dir.x) < epsilon)
	{
		if (ray_origin.x < bounds.min.x || ray_origin.x > bounds.max.x)
			return (0);
	}
	else
	{
		if (!calculate_x_intersection(bounds, ray_origin, ray_dir, near_far))
			return (0);
	}
	return (1);
}

int	check_y_dimension(t_aabb bounds, t_vec3 ray_origin,
					t_vec3 ray_dir, double *near_far)
{
	double	epsilon;

	epsilon = 1e-6;
	if (fabs(ray_dir.y) < epsilon)
	{
		if (ray_origin.y < bounds.min.y || ray_origin.y > bounds.max.y)
			return (0);
	}
	else
	{
		if (!calculate_y_intersection(bounds, ray_origin, ray_dir, near_far))
			return (0);
	}
	return (1);
}

int	check_z_dimension(t_aabb bounds, t_vec3 ray_origin,
					t_vec3 ray_dir, double *near_far)
{
	double	epsilon;

	epsilon = 1e-6;
	if (fabs(ray_dir.z) < epsilon)
	{
		if (ray_origin.z < bounds.min.z || ray_origin.z > bounds.max.z)
			return (0);
	}
	else
	{
		if (!calculate_z_intersection(bounds, ray_origin, ray_dir, near_far))
			return (0);
	}
	return (1);
}
