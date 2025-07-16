/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mesh_intersect.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 16:20:00 by abillote          #+#    #+#             */
/*   Updated: 2025/07/13 22:44:06 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/miniRT.h"

/**
 * Creates a scaled vertex based on mesh scale factors
 *
 * @param vertex  Original vertex position
 * @param scale   Scale vector for x, y, z dimensions
 * @return		Scaled vertex
 */
static t_vec3	scale_vertex(t_vec3 vertex, t_vec3 scale)
{
	t_vec3	scaled;

	scaled.x = vertex.x * scale.x;
	scaled.y = vertex.y * scale.y;
	scaled.z = vertex.z * scale.z;
	return (scaled);
}

/**
 * Transforms a triangle by applying scale, rotation and translation
 *
 * @param t		Source triangle
 * @param mesh	Mesh containing transformation data
 * @return		Transformed triangle
 */
static t_triangle	transform_triangle(t_triangle t, t_mesh mesh, \
					t_vec3 *edge1, t_vec3 *edge2)
{
	t_triangle	transformed;
	t_vec3		scaled_v0;
	t_vec3		scaled_v1;
	t_vec3		scaled_v2;

	scaled_v0 = scale_vertex(t.v0, mesh.scale);
	scaled_v1 = scale_vertex(t.v1, mesh.scale);
	scaled_v2 = scale_vertex(t.v2, mesh.scale);
	transformed.v0 = vec3_add(rotate_point(scaled_v0, mesh.rotation),
			mesh.position);
	transformed.v1 = vec3_add(rotate_point(scaled_v1, mesh.rotation),
			mesh.position);
	transformed.v2 = vec3_add(rotate_point(scaled_v2, mesh.rotation),
			mesh.position);
	*edge1 = vec3_subtract(transformed.v1, transformed.v0);
	*edge2 = vec3_subtract(transformed.v2, transformed.v0);
	transformed.normal = vec3_normalize(vec3_cross(*edge1, *edge2));
	return (transformed);
}

/**
 * Test ray-triangle intersection and update closest hit if needed
 *
 * @param ray		  Ray to test against triangle
 * @param tri		  Triangle to test
 * @param best_t	   Current best intersection distance
 * @param triangle_idx Index of triangle if hit
 * @return			 1 if a hit was found, 0 otherwise
 */
static int	test_triangle_hit(t_ray ray, t_triangle tri,
						double *best_t, int *triangle_idx)
{
	double	current_t;
	int		hit;

	hit = 0;
	if (ray_triangle_intersect(ray, tri, &current_t))
	{
		if (current_t < *best_t)
		{
			*best_t = current_t;
			*triangle_idx = *triangle_idx + 1;
			hit = 1;
		}
	}
	return (hit);
}

/**
 * Test ray intersection with a single triangle in the mesh
 *
 * @param ray		  Ray to test
 * @param mesh		 Mesh containing triangles
 * @param idx		  Index of triangle to test
 * @param params	   Struct containing closest_t and triangle_idx
 * @return			 1 if hit was found, 0 otherwise
 */
static int	process_triangle(t_ray ray, t_mesh mesh,
						int idx, t_intersection_params *params)
{
	t_triangle	transformed_triangle;
	int			hit;
	t_vec3		edge1;
	t_vec3		edge2;

	transformed_triangle = transform_triangle(mesh.triangles[idx], mesh, \
					&edge1, &edge2);
	hit = test_triangle_hit(ray, transformed_triangle,
			&params->closest_t, &params->triangle_idx);
	return (hit);
}

/**
 * Tests ray against all triangles in a mesh
 *
 * @param ray		  Ray to test
 * @param mesh		 Mesh containing triangles
 * @param t			Output of intersection distance
 * @param triangle_idx Output index of hit triangle
 * @return			 1 if any triangle was hit, 0 otherwise
 */
int	ray_mesh_intersect(t_ray ray, t_mesh mesh, double *t, int *triangle_idx)
{
	int						i;
	int						hit_something;
	t_intersection_params	params;

	params.closest_t = INFINITY;
	params.triangle_idx = -1;
	hit_something = 0;
	i = 0;
	while (i < mesh.triangle_count)
	{
		if (process_triangle(ray, mesh, i, &params))
			hit_something = 1;
		i++;
	}
	if (hit_something)
	{
		*t = params.closest_t;
		*triangle_idx = params.triangle_idx;
		return (1);
	}
	return (0);
}
