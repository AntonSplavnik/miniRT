/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mesh_intersect.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 16:20:00 by abillote          #+#    #+#             */
/*   Updated: 2025/05/10 16:30:00 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/**
 * Finds the closest intersection between a ray and a mesh
 *
 * @param ray The ray to test
 * @param mesh The mesh to test
 * @param t Pointer to store the distance to intersection
 * @param triangle_idx Pointer to store the index of the hit triangle
 * @return 1 if there's an intersection, 0 otherwise
 */
int	ray_mesh_intersect(t_ray ray, t_mesh mesh, double *t, int *triangle_idx)
{
	int		i;
	double	closest_t;
	double	current_t;
	int		hit_something;

	closest_t = INFINITY;
	hit_something = 0;
	*triangle_idx = -1;

	// Check intersection with each triangle in the mesh
	for (i = 0; i < mesh.triangle_count; i++)
	{
		// Test ray against this triangle
		if (ray_triangle_intersect(ray, mesh.triangles[i], &current_t))
		{
			// Found a closer intersection?
			if (current_t < closest_t)
			{
				closest_t = current_t;
				*triangle_idx = i;
				hit_something = 1;
			}
		}
	}

	if (hit_something)
	{
		*t = closest_t;
		return (1);
	}
	return (0);
} 