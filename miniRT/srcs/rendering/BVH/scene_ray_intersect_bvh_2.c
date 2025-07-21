/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_ray_intersect_bvh_2.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 00:00:00 by antonsplavn      #+#    #+#             */
/*   Updated: 2025/06/23 00:00:00 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"
#include "../../includes/bvh.h"

int	intersect_primitive(t_object *obj, t_ray ray, double *t_temp,
					int *triangle_idx)
{
	if (obj->type == SPHERE)
		return (intersect_sphere(ray, (t_sphere *)obj->data, t_temp));
	else if (obj->type == PLANE)
		return (intersect_plane(ray, (t_plane *)obj->data, t_temp));
	else if (obj->type == CYLINDER)
		return (intersect_cylinder(ray, (t_cylinder *)obj->data, t_temp));
	else if (obj->type == CUBE)
		return (intersect_cube(ray, (t_cube *)obj->data, t_temp));
	else if (obj->type == TRIANGLE)
		return (intersect_triangle(ray, (t_triangle *)obj->data, t_temp));
	else if (obj->type == MESH)
		return (intersect_mesh(ray, (t_mesh *)obj->data, t_temp,
				triangle_idx));
	else if (obj->type == CONE)
		return (intersect_cone(ray, (t_cone *)obj->data, t_temp));
	return (0);
}

int	intersect_primitive_bary(t_primitive_bary_params params, t_vec3 *bary)
{
	if (params.obj->type == SPHERE)
		return (intersect_sphere(params.ray, (t_sphere *)params.obj->data,
				params.t_temp));
	else if (params.obj->type == PLANE)
		return (intersect_plane(params.ray, (t_plane *)params.obj->data,
				params.t_temp));
	else if (params.obj->type == CYLINDER)
		return (intersect_cylinder(params.ray, (t_cylinder *)params.obj->data,
				params.t_temp));
	else if (params.obj->type == CUBE)
		return (intersect_cube(params.ray, (t_cube *)params.obj->data,
				params.t_temp));
	else if (params.obj->type == TRIANGLE)
		return (intersect_triangle_bary(params.ray,
				(t_triangle *)params.obj->data, params.t_temp, bary));
	else if (params.obj->type == MESH)
		return (intersect_mesh_bary(params.ray, (t_mesh *)params.obj->data,
				params.t_temp, (t_mesh_bary_params){params.t_temp,
				params.triangle_idx, bary}));
	else if (params.obj->type == CONE)
		return (intersect_cone(params.ray, (t_cone *)params.obj->data,
				params.t_temp));
	return (0);
}

static int	process_triangle_mesh_intersection(\
	t_leaf_intersection_params params, double *t_temp, double current_closest)
{
	int		triangle_idx;
	t_vec3	bary;

	triangle_idx = -1;
	if (!intersect_primitive_bary((t_primitive_bary_params){params.obj,
			params.ray, t_temp, &triangle_idx}, &bary))
		return (0);
	if (*t_temp >= current_closest)
		return (0);
	*(params.closest_t) = *t_temp;
	*(params.hit_object) = params.obj;
	params.hit_record->barycentric = bary;
	if (triangle_idx != -1)
		params.hit_record->triangle_idx = triangle_idx;
	return (1);
}

static int	process_primitive_intersection(t_leaf_intersection_params params,
									double *t_temp, double current_closest)
{
	int	triangle_idx;

	triangle_idx = -1;
	if (!intersect_primitive(params.obj, params.ray, t_temp, &triangle_idx))
		return (0);
	if (*t_temp >= current_closest)
		return (0);
	*(params.closest_t) = *t_temp;
	*(params.hit_object) = params.obj;
	if (triangle_idx != -1)
		params.hit_record->triangle_idx = triangle_idx;
	return (1);
}

int	test_leaf_node_intersection(t_leaf_intersection_params params)
{
	double	t_temp;
	double	current_closest;

	current_closest = *(params.closest_t);
	if (params.obj->type == TRIANGLE || params.obj->type == MESH)
		return (process_triangle_mesh_intersection(params, &t_temp,
				current_closest));
	else
		return (process_primitive_intersection(params, &t_temp,
				current_closest));
}
