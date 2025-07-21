/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh_intersect_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 21:30:00 by abillote          #+#    #+#             */
/*   Updated: 2025/07/18 17:51:31 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"
#include "../../includes/bvh.h"

int	ray_intersect_aabb_optimized(t_ray ray, t_aabb bounds, double max_t)
{
	double	t1;
	double	t2;
	double	t_near;
	double	t_far;

	t1 = (bounds.min.x - ray.origin.x) / ray.direction.x;
	t2 = (bounds.max.x - ray.origin.x) / ray.direction.x;
	t_near = fmin(t1, t2);
	t_far = fmax(t1, t2);
	t1 = (bounds.min.y - ray.origin.y) / ray.direction.y;
	t2 = (bounds.max.y - ray.origin.y) / ray.direction.y;
	t_near = fmax(t_near, fmin(t1, t2));
	t_far = fmin(t_far, fmax(t1, t2));
	t1 = (bounds.min.z - ray.origin.z) / ray.direction.z;
	t2 = (bounds.max.z - ray.origin.z) / ray.direction.z;
	t_near = fmax(t_near, fmin(t1, t2));
	t_far = fmin(t_far, fmax(t1, t2));
	return (t_near <= t_far && t_far > 0.001 && t_near < max_t);
}

static int	compute_triangle_edges(t_ray ray, t_transformed_triangle tri,
		t_intersect_data *data)
{
	t_vec3	edge1;
	t_vec3	edge2;
	t_vec3	h;

	edge1 = vec3_subtract(tri.v1, tri.v0);
	edge2 = vec3_subtract(tri.v2, tri.v0);
	h = vec3_cross(ray.direction, edge2);
	data->a = vec3_dot(edge1, h);
	if (data->a > -0.001 && data->a < 0.001)
		return (0);
	data->f = 1.0 / data->a;
	return (1);
}

static int	compute_barycentric_coords(t_ray ray, t_transformed_triangle tri,
		t_intersect_data *data)
{
	t_vec3	s;
	t_vec3	q;

	s = vec3_subtract(ray.origin, tri.v0);
	data->u = data->f * vec3_dot(s, vec3_cross(ray.direction,
				vec3_subtract(tri.v2, tri.v0)));
	if (data->u < 0.0 || data->u > 1.0)
		return (0);
	q = vec3_cross(s, vec3_subtract(tri.v1, tri.v0));
	data->v = data->f * vec3_dot(ray.direction, q);
	if (data->v < 0.0 || data->u + data->v > 1.0)
		return (0);
	data->temp_t = data->f * vec3_dot(vec3_subtract(tri.v2, tri.v0), q);
	return (1);
}

int	ray_triangle_intersect_fast(t_ray ray, t_transformed_triangle tri,
		double *t)
{
	t_intersect_data	data;

	if (!compute_triangle_edges(ray, tri, &data))
		return (0);
	if (!compute_barycentric_coords(ray, tri, &data))
		return (0);
	if (data.temp_t > 0.001)
	{
		*t = data.temp_t;
		return (1);
	}
	return (0);
}
