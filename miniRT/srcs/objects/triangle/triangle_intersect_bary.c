/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   triangle_intersect_bary.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 15:36:04 by antonsplavn       #+#    #+#             */
/*   Updated: 2025/07/18 15:53:02 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/miniRT.h"

/*
** Computes triangle edges and cross product for ray-triangle intersection
*/
static int	compute_triangle_edges(t_ray ray, t_triangle triangle, \
									t_triangle_data *data)
{
	data->edge1 = vec3_subtract(triangle.v1, triangle.v0);
	data->edge2 = vec3_subtract(triangle.v2, triangle.v0);
	data->h = vec3_cross(ray.direction, data->edge2);
	data->a = vec3_dot(data->edge1, data->h);
	if (fabs(data->a) < 0.00001)
		return (0);
	return (1);
}

/*
** Computes barycentric coordinates u and v for triangle intersection
*/
static int	compute_barycentric_coords(t_ray ray, t_triangle triangle, \
										t_triangle_data *data)
{
	t_vec3	s;

	data->f = 1.0 / data->a;
	s = vec3_subtract(ray.origin, triangle.v0);
	data->u = data->f * vec3_dot(s, data->h);
	if (data->u < 0.0 || data->u > 1.0)
		return (0);
	data->q = vec3_cross(s, data->edge1);
	data->v = data->f * vec3_dot(ray.direction, data->q);
	if (data->v < 0.0 || data->u + data->v > 1.0)
		return (0);
	return (1);
}

/*
** Finalizes intersection test and sets barycentric coordinates
*/
static int	finalize_intersection(t_triangle_data *data, double *t, \
									t_vec3 *bary)
{
	*t = data->f * vec3_dot(data->edge2, data->q);
	if (*t > 0.001)
	{
		bary->x = 1.0 - data->u - data->v;
		bary->y = data->u;
		bary->z = data->v;
		return (1);
	}
	return (0);
}

/*
** Ray-triangle intersection using barycentric coordinates
*/
int	ray_triangle_intersect_bary(t_ray ray, t_triangle triangle, double *t, \
									t_vec3 *bary)
{
	t_triangle_data	data;

	if (!compute_triangle_edges(ray, triangle, &data))
		return (0);
	if (!compute_barycentric_coords(ray, triangle, &data))
		return (0);
	return (finalize_intersection(&data, t, bary));
}
