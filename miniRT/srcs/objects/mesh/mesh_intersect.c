/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mesh_intersect.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 16:20:00 by abillote          #+#    #+#             */
/*   Updated: 2025/06/20 14:11:28 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/miniRT.h"

//New version to rotate properly each triangle in the mesh according to the mesh's rotation
int ray_mesh_intersect(t_ray ray, t_mesh mesh, double *t, int *triangle_idx)
{
	int	 i;
	double  closest_t;
	double  current_t;
	int	 hit_something;

	closest_t = INFINITY;
	hit_something = 0;
	*triangle_idx = -1;

	// For each triangle in the mesh
	for (i = 0; i < mesh.triangle_count; i++)
	{
		// Create a transformed triangle
		t_triangle transformed_triangle;

		// Apply scale
		t_vec3 scaled_v0 = {
			mesh.triangles[i].v0.x * mesh.scale.x,
			mesh.triangles[i].v0.y * mesh.scale.y,
			mesh.triangles[i].v0.z * mesh.scale.z
		};
		t_vec3 scaled_v1 = {
			mesh.triangles[i].v1.x * mesh.scale.x,
			mesh.triangles[i].v1.y * mesh.scale.y,
			mesh.triangles[i].v1.z * mesh.scale.z
		};
		t_vec3 scaled_v2 = {
			mesh.triangles[i].v2.x * mesh.scale.x,
			mesh.triangles[i].v2.y * mesh.scale.y,
			mesh.triangles[i].v2.z * mesh.scale.z
		};

		// Apply rotation
		t_vec3 rotated_v0 = rotate_point(scaled_v0, mesh.rotation);
		t_vec3 rotated_v1 = rotate_point(scaled_v1, mesh.rotation);
		t_vec3 rotated_v2 = rotate_point(scaled_v2, mesh.rotation);

		// Apply translation
		transformed_triangle.v0 = vec3_add(rotated_v0, mesh.position);
		transformed_triangle.v1 = vec3_add(rotated_v1, mesh.position);
		transformed_triangle.v2 = vec3_add(rotated_v2, mesh.position);

		// Recalculate normal for the transformed triangle
		t_vec3 edge1 = vec3_subtract(transformed_triangle.v1, transformed_triangle.v0);
		t_vec3 edge2 = vec3_subtract(transformed_triangle.v2, transformed_triangle.v0);
		transformed_triangle.normal = vec3_normalize(vec3_cross(edge1, edge2));

		// Test ray against this transformed triangle
		if (ray_triangle_intersect(ray, transformed_triangle, &current_t))
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
