/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_intersect_aabb_3.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 00:00:00 by antonsplavn      #+#    #+#             */
/*   Updated: 2025/06/23 00:00:00 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

int	check_z_dimension(t_aabb bounds, t_vec3 ray_origin,
					t_vec3 ray_dir, double *near_far)
{
	double	epsilon;
	double	inv_dir;
	double	t1;
	double	t2;

	epsilon = 1e-6;
	if (fabs(ray_dir.z) < epsilon)
	{
		if (ray_origin.z < bounds.min.z || ray_origin.z > bounds.max.z)
			return (0);
	}
	else
	{
		inv_dir = 1.0 / ray_dir.z;
		t1 = (bounds.min.z - ray_origin.z) * inv_dir;
		t2 = (bounds.max.z - ray_origin.z) * inv_dir;
		if (t1 > t2)
			swap_values(&t1, &t2);
		if (t1 > near_far[0])
			near_far[0] = t1;
		if (t2 < near_far[1])
			near_far[1] = t2;
		if (near_far[0] > near_far[1] || near_far[1] < 0)
			return (0);
	}
	return (1);
}

int	ray_intersect_aabb_scalar(t_aabb bounds, t_vec3 ray_origin,
							t_vec3 ray_dir, double *t_min, double *t_max)
{
	double	near_far[2];

	near_far[0] = -INFINITY;
	near_far[1] = INFINITY;
	if (!check_x_dimension(bounds, ray_origin, ray_dir, near_far))
		return (0);
	if (!check_y_dimension(bounds, ray_origin, ray_dir, near_far))
		return (0);
	if (!check_z_dimension(bounds, ray_origin, ray_dir, near_far))
		return (0);
	*t_min = near_far[0];
	*t_max = near_far[1];
	return (1);
}
