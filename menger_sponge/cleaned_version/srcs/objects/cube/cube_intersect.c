/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cube_intersect.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 13:00:00 by abillote          #+#    #+#             */
/*   Updated: 2025/05/10 13:10:00 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/miniRT.h"

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

// Ray-cube intersection test using AABB approach
int	ray_cube_intersect(t_ray ray, t_cube cube, double *t)
{
	t_vec3	half_size;
	t_vec3	min_bound;
	t_vec3	max_bound;
	double	t_near;
	double	t_far;
	double	t_min[3];
	double	t_max[3];
	int		i;

	// Calculate half size of the cube
	half_size = vec3_scale(vec3_create(1, 1, 1), cube.side_length / 2.0);

	// Calculate the AABB bounds
	min_bound = vec3_subtract(cube.center, half_size);
	max_bound = vec3_add(cube.center, half_size);

	// Initialize t_near and t_far
	t_near = -INFINITY;
	t_far = INFINITY;

	// Check intersection with all 3 axis-aligned slabs
	i = 0;
	while (i < 3)
	{
		double component_dir = i == 0 ? ray.direction.x : (i == 1 ? ray.direction.y : ray.direction.z);
		double component_orig = i == 0 ? ray.origin.x : (i == 1 ? ray.origin.y : ray.origin.z);
		double component_min = i == 0 ? min_bound.x : (i == 1 ? min_bound.y : min_bound.z);
		double component_max = i == 0 ? max_bound.x : (i == 1 ? max_bound.y : max_bound.z);

		// Handle case where ray is parallel to the slab
		if (fabs(component_dir) < 0.0001)
		{
			// If origin is outside slab, no intersection
			if (component_orig < component_min || component_orig > component_max)
				return (0);
		}
		else
		{
			// Calculate intersection times with the slab
			t_min[i] = (component_min - component_orig) / component_dir;
			t_max[i] = (component_max - component_orig) / component_dir;

			// Ensure t_min <= t_max
			if (t_min[i] > t_max[i])
			{
				double temp = t_min[i];
				t_min[i] = t_max[i];
				t_max[i] = temp;
			}

			// Update t_near and t_far
			t_near = max(t_near, t_min[i]);
			t_far = min(t_far, t_max[i]);

			// Check if there's no intersection
			if (t_near > t_far || t_far < 0.001)
				return (0);
		}
		i++;
	}

	// If we reach this point, there is an intersection
	*t = t_near > 0.001 ? t_near : t_far;
	return (1);
} 