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

int	test_leaf_node_intersection(t_leaf_intersection_params params)
{
	double	t_temp;
	int		triangle_idx;
	double	current_closest;

	current_closest = *(params.closest_t);
	triangle_idx = -1;
	if (!intersect_primitive(params.obj, params.ray, &t_temp, &triangle_idx))
		return (0);
	if (t_temp >= current_closest)
		return (0);
	*(params.closest_t) = t_temp;
	*(params.hit_object) = params.obj;
	if (triangle_idx != -1)
		params.hit_record->triangle_idx = triangle_idx;
	return (1);
}
