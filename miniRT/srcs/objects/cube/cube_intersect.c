/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cube_intersect.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 13:00:00 by abillote          #+#    #+#             */
/*   Updated: 2025/06/20 10:15:47 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/miniRT.h"

int	calculate_slab_x_intersection(t_slab slabs, double *t_min, double *t_max)
{
	double	temp;

	if (fabs(slabs.slab_x_dir) < 0.0001)
	{
		if (slabs.slab_x_origin < slabs.slab_x_min - 0.00001 \
			|| slabs.slab_x_origin > slabs.slab_x_max + 0.00001)
			return (0);
		*t_min = -INFINITY;
		*t_max = INFINITY;
	}
	else
	{
		*t_min = (slabs.slab_x_min - slabs.slab_x_origin) \
				/ slabs.slab_x_dir;
		*t_max = (slabs.slab_x_max - slabs.slab_x_origin) \
				/ slabs.slab_x_dir;
		if (*t_min > *t_max)
		{
			temp = *t_min;
			*t_min = *t_max;
			*t_max = temp;
		}
		return (1);
	}
	return (1);
}

int	calculate_slab_y_intersection(t_slab slabs, double *t_min, double *t_max)
{
	double	temp;

	if (fabs(slabs.slab_y_dir) < 0.0001)
	{
		if (slabs.slab_y_origin < slabs.slab_y_min - 0.00001 \
				|| slabs.slab_y_origin > slabs.slab_y_max + 0.00001)
			return (0);
		*t_min = -INFINITY;
		*t_max = INFINITY;
	}
	else
	{
		*t_min = (slabs.slab_y_min - slabs.slab_y_origin) \
				/ slabs.slab_y_dir;
		*t_max = (slabs.slab_y_max - slabs.slab_y_origin) \
				/ slabs.slab_y_dir;
		if (*t_min > *t_max)
		{
			temp = *t_min;
			*t_min = *t_max;
			*t_max = temp;
		}
		return (1);
	}
	return (1);
}

int	calculate_slab_z_intersection(t_slab slabs, double *t_min, double *t_max)
{
	double	temp;

	if (fabs(slabs.slab_z_dir) < 0.0001)
	{
		if (slabs.slab_z_origin < slabs.slab_z_min - 0.00001 \
				|| slabs.slab_z_origin > slabs.slab_z_max + 0.00001)
			return (0);
		*t_min = -INFINITY;
		*t_max = INFINITY;
	}
	else
	{
		*t_min = (slabs.slab_z_min - slabs.slab_z_origin) \
				/ slabs.slab_z_dir;
		*t_max = (slabs.slab_z_max - slabs.slab_z_origin) \
				/ slabs.slab_z_dir;
		if (*t_min > *t_max)
		{
			temp = *t_min;
			*t_min = *t_max;
			*t_max = temp;
		}
		return (1);
	}
	return (1);
}

int	calculate_slab_intersection_point(t_slab slabs, \
			double *t_near, double *t_far)
{
	double	t_min;
	double	t_max;

	t_min = 0;
	t_max = 0;
	if (!calculate_slab_x_intersection(slabs, &t_min, &t_max))
		return (0);
	*t_near = fmax(*t_near, t_min);
	*t_far = fmin(*t_far, t_max);
	if (*t_near > *t_far || *t_far < 0.001)
		return (0);
	if (!calculate_slab_y_intersection(slabs, &t_min, &t_max))
		return (0);
	*t_near = fmax(*t_near, t_min);
	*t_far = fmin(*t_far, t_max);
	if (*t_near > *t_far || *t_far < 0.001)
		return (0);
	if (!calculate_slab_z_intersection(slabs, &t_min, &t_max))
		return (0);
	*t_near = fmax(*t_near, t_min);
	*t_far = fmin(*t_far, t_max);
	if (*t_near > *t_far || *t_far < 0.001)
		return (0);
	return (1);
}

// Ray-cube intersection test using AABB approach
int	ray_cube_intersect(t_ray ray, t_cube cube, double *t)
{
	t_vec3	min_bound;
	t_vec3	max_bound;
	t_slab	slabs;
	double	t_near;
	double	t_far;

	min_bound = vec3_subtract(cube.center, vec3_scale(vec3_create(1, 1, 1), \
				cube.side_length / 2.0));
	max_bound = vec3_add(cube.center, vec3_scale(vec3_create(1, 1, 1), \
				cube.side_length / 2.0));
	t_near = -INFINITY;
	t_far = INFINITY;
	slabs = calculate_slab_data(ray, min_bound, max_bound);
	if (!calculate_slab_intersection_point(slabs, &t_near, &t_far))
		return (0);
	else
	{
		if (t_near > 0.001)
			*t = t_near;
		else
			*t = t_far;
		return (1);
	}
}
