/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_intersect_aabb_2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 03:24:10 by antonsplavn       #+#    #+#             */
/*   Updated: 2025/07/21 04:31:21 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"
#include "../../includes/bvh.h"

int	calculate_x_intersection(t_aabb bounds, t_vec3 ray_origin, t_vec3 ray_dir,
		double *near_far)
{
	double	inv_dir;
	double	t1;
	double	t2;

	inv_dir = 1.0 / ray_dir.x;
	t1 = (bounds.min.x - ray_origin.x) * inv_dir;
	t2 = (bounds.max.x - ray_origin.x) * inv_dir;
	if (t1 > t2)
		swap_values(&t1, &t2);
	if (t1 > near_far[0])
		near_far[0] = t1;
	if (t2 < near_far[1])
		near_far[1] = t2;
	if (near_far[0] > near_far[1] || near_far[1] < 0)
		return (0);
	return (1);
}

int	calculate_y_intersection(t_aabb bounds, t_vec3 ray_origin, t_vec3 ray_dir,
		double *near_far)
{
	double	inv_dir;
	double	t1;
	double	t2;

	inv_dir = 1.0 / ray_dir.y;
	t1 = (bounds.min.y - ray_origin.y) * inv_dir;
	t2 = (bounds.max.y - ray_origin.y) * inv_dir;
	if (t1 > t2)
		swap_values(&t1, &t2);
	if (t1 > near_far[0])
		near_far[0] = t1;
	if (t2 < near_far[1])
		near_far[1] = t2;
	if (near_far[0] > near_far[1] || near_far[1] < 0)
		return (0);
	return (1);
}

int	calculate_z_intersection(t_aabb bounds, t_vec3 ray_origin, t_vec3 ray_dir,
		double *near_far)
{
	double	inv_dir;
	double	t1;
	double	t2;

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
	return (1);
}
