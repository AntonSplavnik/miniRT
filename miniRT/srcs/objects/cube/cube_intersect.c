/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cube_intersect.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 13:00:00 by abillote          #+#    #+#             */
/*   Updated: 2025/06/19 13:08:31 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/miniRT.h"

typedef struct s_slab
{
	double	slab_x_dir;
	double	slab_x_origin;
	double	slab_x_min;
	double	slab_x_max;
	double	slab_y_dir;
	double	slab_y_origin;
	double	slab_y_min;
	double	slab_y_max;
	double	slab_z_dir;
	double	slab_z_origin;
	double	slab_z_min;
	double	slab_z_max;
}	t_slab;

// Helper function to find the minimum of two values
static double	min(double a, double b)
{
	if (a < b)
		return (a);
	return (b);
}

// Helper function to find the maximum of two values
static double	max(double a, double b)
{
	if (a > b)
		return (a);
	return (b);
}

t_slab	calculate_slab_data(t_ray ray, t_vec3 min_bound, t_vec3 max_bound)
{
	t_slab	slab;

	slab.slab_x_dir = ray.direction.x;
	slab.slab_x_origin = ray.origin.x;
	slab.slab_x_max = max_bound.x;
	slab.slab_x_min = min_bound.x;
	slab.slab_y_dir = ray.direction.y;
	slab.slab_y_origin = ray.origin.y;
	slab.slab_y_max = max_bound.y;
	slab.slab_y_min = min_bound.y;
	slab.slab_z_dir = ray.direction.z;
	slab.slab_z_origin = ray.origin.z;
	slab.slab_z_max = max_bound.z;
	slab.slab_z_min = min_bound.z;
	return (slab);
}

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
	*t_near = max(*t_near, t_min);
	*t_far = min(*t_far, t_max);
	if (*t_near > *t_far || *t_far < 0.001)
		return (0);
	if (!calculate_slab_y_intersection(slabs, &t_min, &t_max))
		return (0);
	*t_near = max(*t_near, t_min);
	*t_far = min(*t_far, t_max);
	if (*t_near > *t_far || *t_far < 0.001)
		return (0);
	if (!calculate_slab_z_intersection(slabs, &t_min, &t_max))
		return (0);
	*t_near = max(*t_near, t_min);
	*t_far = min(*t_far, t_max);
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

// Calculate normal at the intersection point
t_vec3	cube_normal_at_point(t_vec3 point, t_cube cube)
{
	t_vec3	center_to_point;
	t_vec3	abs_center_to_point;
	double	max_component;
	t_vec3	normal;

	// Calculate vector from center to intersection point
	center_to_point = vec3_subtract(point, cube.center);

	// Find which face was hit (max component approach)
	abs_center_to_point.x = fabs(center_to_point.x);
	abs_center_to_point.y = fabs(center_to_point.y);
	abs_center_to_point.z = fabs(center_to_point.z);

	// Determine which component is largest to find which face was hit
	max_component = max(abs_center_to_point.x, max(abs_center_to_point.y, abs_center_to_point.z));

	// Set normal based on the face that was hit
	normal = vec3_create(0, 0, 0);
	if (max_component == abs_center_to_point.x)
		normal.x = center_to_point.x > 0 ? 1 : -1;
	else if (max_component == abs_center_to_point.y)
		normal.y = center_to_point.y > 0 ? 1 : -1;
	else
		normal.z = center_to_point.z > 0 ? 1 : -1;

	// Apply rotation (if the cube has rotation support in the future)
	return (vec3_normalize(normal));
}
