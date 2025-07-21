/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_intersect_aabb.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 03:24:22 by antonsplavn       #+#    #+#             */
/*   Updated: 2025/07/21 04:49:02 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

void	swap_values(double *a, double *b)
{
	double	temp;

	temp = *a;
	*a = *b;
	*b = temp;
}

static int	check_x_dimension(t_aabb bounds, t_vec3 ray_origin,
				t_vec3 ray_dir, double *t_vals)
{
	double	inv_dir;
	double	t1;
	double	t2;
	double	epsilon;

	epsilon = 1e-6;
	if (fabs(ray_dir.x) < epsilon)
	{
		if (ray_origin.x < bounds.min.x || ray_origin.x > bounds.max.x)
			return (0);
		return (1);
	}
	inv_dir = 1.0 / ray_dir.x;
	t1 = (bounds.min.x - ray_origin.x) * inv_dir;
	t2 = (bounds.max.x - ray_origin.x) * inv_dir;
	if (t1 > t2)
		swap_values(&t1, &t2);
	if (t1 > t_vals[0])
		t_vals[0] = t1;
	if (t2 < t_vals[1])
		t_vals[1] = t2;
	if (t_vals[0] > t_vals[1] || t_vals[1] < 0)
		return (0);
	return (1);
}

static int	check_y_dimension(t_aabb bounds, t_vec3 ray_origin,
				t_vec3 ray_dir, double *t_vals)
{
	double	inv_dir;
	double	t1;
	double	t2;
	double	epsilon;

	epsilon = 1e-6;
	if (fabs(ray_dir.y) < epsilon)
	{
		if (ray_origin.y < bounds.min.y || ray_origin.y > bounds.max.y)
			return (0);
		return (1);
	}
	inv_dir = 1.0 / ray_dir.y;
	t1 = (bounds.min.y - ray_origin.y) * inv_dir;
	t2 = (bounds.max.y - ray_origin.y) * inv_dir;
	if (t1 > t2)
		swap_values(&t1, &t2);
	if (t1 > t_vals[0])
		t_vals[0] = t1;
	if (t2 < t_vals[1])
		t_vals[1] = t2;
	if (t_vals[0] > t_vals[1] || t_vals[1] < 0)
		return (0);
	return (1);
}

static int	check_z_dimension(t_aabb bounds, t_vec3 ray_origin,
				t_vec3 ray_dir, double *t_vals)
{
	double	inv_dir;
	double	t1;
	double	t2;
	double	epsilon;

	epsilon = 1e-6;
	if (fabs(ray_dir.z) < epsilon)
	{
		if (ray_origin.z < bounds.min.z || ray_origin.z > bounds.max.z)
			return (0);
		return (1);
	}
	inv_dir = 1.0 / ray_dir.z;
	t1 = (bounds.min.z - ray_origin.z) * inv_dir;
	t2 = (bounds.max.z - ray_origin.z) * inv_dir;
	if (t1 > t2)
		swap_values(&t1, &t2);
	if (t1 > t_vals[0])
		t_vals[0] = t1;
	if (t2 < t_vals[1])
		t_vals[1] = t2;
	if (t_vals[0] > t_vals[1] || t_vals[1] < 0)
		return (0);
	return (1);
}

int	ray_intersect_aabb_scalar(t_ray_aabb_params params)
{
	double	t_vals[2];
	t_vec3	ray_origin;
	t_vec3	ray_dir;
	t_aabb	bounds;

	t_vals[0] = -INFINITY;
	t_vals[1] = INFINITY;
	ray_origin = params.ray_origin;
	ray_dir = params.ray_dir;
	bounds = params.bounds;
	if (!check_x_dimension(bounds, ray_origin, ray_dir, t_vals))
		return (0);
	if (!check_y_dimension(bounds, ray_origin, ray_dir, t_vals))
		return (0);
	if (!check_z_dimension(bounds, ray_origin, ray_dir, t_vals))
		return (0);
	*(params.t_min) = t_vals[0];
	*(params.t_max) = t_vals[1];
	return (1);
}
