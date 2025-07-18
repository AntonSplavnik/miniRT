/* ************************************************************************** */
/*	                                                                        */
/*                                                        :::      ::::::::   */
/*   triangle_intersect.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 14:30:00 by abillote          #+#    #+#             */
/*   Updated: 2025/06/20 16:01:34 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/miniRT.h"

// Calculate the normal of a triangle (counter-clockwise winding)
t_vec3	triangle_normal(t_triangle triangle)
{
	t_vec3	edge1;
	t_vec3	edge2;
	t_vec3	normal;

	edge1 = vec3_subtract(triangle.v1, triangle.v0);
	edge2 = vec3_subtract(triangle.v2, triangle.v0);
	normal = vec3_cross(edge1, edge2);
	return (vec3_normalize(normal));
}

/**
 * Calculates preliminary values for Möller-Trumbore intersection algorithm
 *
 * @param ray       Ray to test for intersection
 * @param triangle  Triangle to test against
 * @param values    Output array to store calculated values [edge1, edge2, h, a]
 * @return          0 if ray is parallel to triangle, 1 otherwise
 */
static int	calculate_mt_preliminaries(t_ray ray, t_triangle triangle,
										t_vec3 values[4], double *a_value)
{
	values[0] = vec3_subtract(triangle.v1, triangle.v0);
	values[1] = vec3_subtract(triangle.v2, triangle.v0);
	values[2] = vec3_cross(ray.direction, values[1]);
	*a_value = vec3_dot(values[0], values[2]);
	if (fabs(*a_value) < 0.00001)
		return (0);
	return (1);
}

/**
 * Calculates barycentric U coordinate and tests its range
 *
 * @param ray	   Ray to test
 * @param triangle  Triangle to test against
 * @param values	Precalculated values [edge1, edge2, h, s]
 * @param factors   Factor values [f, u]
 * @return		  1 if u is in valid range, 0 otherwise
 */
static int	calculate_u_coordinate(t_ray ray, t_triangle triangle,
									t_vec3 values[4], double factors[2])
{
	t_vec3	s;
	double	dot_product;

	s = vec3_subtract(ray.origin, triangle.v0);
	values[3] = s;
	factors[0] = 1.0 / factors[0];
	dot_product = vec3_dot(s, values[2]);
	factors[1] = factors[0] * dot_product;
	if (factors[1] < 0.0 || factors[1] > 1.0)
		return (0);
	return (1);
}

/**
 * Calculates barycentric V coordinate and tests its range
 *
 * @param ray	   Ray to test
 * @param values	Precalculated values [edge1, edge2, h, s]
 * @param factors   Factor values [f, u, v]
 * @return		  1 if v is in valid range, 0 otherwise
 */
static int	calculate_v_coordinate(t_ray ray, t_vec3 values[4],
									double factors[3])
{
	t_vec3	q;
	double	dot_product;

	q = vec3_cross(values[3], values[0]);
	dot_product = vec3_dot(ray.direction, q);
	factors[2] = factors[0] * dot_product;
	if (factors[2] < 0.0 || factors[1] + factors[2] > 1.0)
		return (0);
	return (1);
}

/**
 * Ray-triangle intersection using Möller–Trumbore algorithm
 *
 * @param ray	   Ray to test for intersection
 * @param triangle  Triangle to test against
 * @param t		 Output parameter for intersection distance
 * @return		  1 if ray intersects triangle, 0 otherwise
 */
int	ray_triangle_intersect(t_ray ray, t_triangle triangle, double *t)
{
	t_vec3	values[4];
	double	factors[3];
	t_vec3	q;
	double	dot_product;

	if (!calculate_mt_preliminaries(ray, triangle, values, &factors[0]))
		return (0);
	if (!calculate_u_coordinate(ray, triangle, values, factors))
		return (0);
	if (!calculate_v_coordinate(ray, values, factors))
		return (0);
	q = vec3_cross(values[3], values[0]);
	dot_product = vec3_dot(values[1], q);
	*t = factors[0] * dot_product;
	if (*t > 0.001)
		return (1);
	return (0);
}
