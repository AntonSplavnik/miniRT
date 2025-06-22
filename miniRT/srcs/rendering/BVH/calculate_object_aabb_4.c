/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calculate_object_aabb_4.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 00:00:00 by antonsplavn      #+#    #+#             */
/*   Updated: 2025/06/23 00:00:00 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"
#include "../../includes/bvh.h"

t_aabb	calculate_mesh_aabb(t_mesh *mesh)
{
	t_aabb		bounds;
	t_triangle	*tri;
	int			i;
	int			triangle_count;

	bounds.min = vec3_create(INFINITY, INFINITY, INFINITY);
	bounds.max = vec3_create(-INFINITY, -INFINITY, -INFINITY);
	i = 0;
	triangle_count = mesh->triangle_count;
	while (i < triangle_count)
	{
		tri = &mesh->triangles[i];
		update_mesh_min_bounds(tri, &bounds);
		update_mesh_max_bounds(tri, &bounds);
		i++;
	}
	adjust_mesh_aabb(mesh, &bounds);
	return (bounds);
}

t_aabb	calculate_object_aabb(t_object *object)
{
	t_aabb	bounds;

	bounds.min = vec3_create(0, 0, 0);
	bounds.max = vec3_create(0, 0, 0);
	if (object->type == SPHERE)
		bounds = calculate_sphere_aabb((t_sphere *)object->data);
	else if (object->type == PLANE)
		bounds = calculate_plane_aabb((t_plane *)object->data);
	else if (object->type == CYLINDER)
		bounds = calculate_cylinder_aabb((t_cylinder *)object->data);
	else if (object->type == CUBE)
		bounds = calculate_cube_aabb((t_cube *)object->data);
	else if (object->type == CONE)
		bounds = calculate_cone_aabb((t_cone *)object->data);
	else if (object->type == TRIANGLE)
		bounds = calculate_triangle_aabb((t_triangle *)object->data);
	else if (object->type == MESH)
		bounds = calculate_mesh_aabb((t_mesh *)object->data);
	return (bounds);
}
