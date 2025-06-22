/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_intersect_aabb.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 00:00:00 by antonsplavn      #+#    #+#             */
/*   Updated: 2025/06/23 00:00:00 by antonsplavn      ###   ########.fr       */
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

int	check_x_dimension(t_aabb bounds, t_vec3 ray_origin,
					t_vec3 ray_dir, double *near_far)
{
	double	epsilon;
	double	inv_dir;
	double	t1;
	double	t2;

	epsilon = 1e-6;
	if (fabs(ray_dir.x) < epsilon)
	{
		if (ray_origin.x < bounds.min.x || ray_origin.x > bounds.max.x)
			return (0);
	}
	else
	{
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
	}
	return (1);
}
