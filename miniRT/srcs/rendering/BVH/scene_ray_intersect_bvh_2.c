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


int	intersect_mesh(t_ray ray, t_mesh *mesh, double *t_temp, int *triangle_idx)
{
	return (ray_mesh_intersect(ray, *mesh, t_temp, triangle_idx));
}

int	intersect_mesh_bary(t_ray ray, t_mesh *mesh, double *t_temp, int *triangle_idx, t_vec3 *bary)
{
	t_mesh_intersect_params params = {ray, *mesh, t_temp, triangle_idx, bary};
	return (ray_mesh_intersect_bary(params));
}

int	intersect_cone(t_ray ray, t_cone *cone, double *t_temp)
{
	return (ray_cone_intersect(ray, *cone, t_temp));
}

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

int	intersect_primitive_bary(t_object *obj, t_ray ray, double *t_temp,
					int *triangle_idx, t_vec3 *bary)
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
		return (intersect_triangle_bary(ray, (t_triangle *)obj->data, t_temp, bary));
	else if (obj->type == MESH)
		return (intersect_mesh_bary(ray, (t_mesh *)obj->data, t_temp,
				triangle_idx, bary));
	else if (obj->type == CONE)
		return (intersect_cone(ray, (t_cone *)obj->data, t_temp));
	return (0);
}

int	test_leaf_node_intersection(t_leaf_intersection_params params)
{
	double	t_temp;
	int		triangle_idx;
	double	current_closest;
	t_vec3	bary;

	current_closest = *(params.closest_t);
	triangle_idx = -1;
	if (params.obj->type == TRIANGLE || params.obj->type == MESH)
	{
		if (!intersect_primitive_bary(params.obj, params.ray, &t_temp, &triangle_idx, &bary))
			return (0);
		if (t_temp >= current_closest)
			return (0);
		*(params.closest_t) = t_temp;
		*(params.hit_object) = params.obj;
		params.hit_record->barycentric = bary;
		if (triangle_idx != -1)
			params.hit_record->triangle_idx = triangle_idx;
	}
	else
	{
		if (!intersect_primitive(params.obj, params.ray, &t_temp, &triangle_idx))
			return (0);
		if (t_temp >= current_closest)
			return (0);
		*(params.closest_t) = t_temp;
		*(params.hit_object) = params.obj;
		if (triangle_idx != -1)
			params.hit_record->triangle_idx = triangle_idx;
	}
	return (1);
}
