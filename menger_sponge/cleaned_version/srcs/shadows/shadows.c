/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shadows.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 10:05:53 by abillote          #+#    #+#             */
/*   Updated: 2025/05/22 12:25:37 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

int any_intersection(t_scene *scene, t_ray ray, double max_dist)
{
	t_object	*current;
	double	  t_temp;

	current = scene->objects;
	while (current)
	{
		if (current->type == SPHERE)
		{
			t_sphere *sphere = (t_sphere *)(current->data);
			if (ray_sphere_intersect(ray, *sphere, &t_temp) && t_temp < max_dist)
				return (1);
		}
		else if (current->type == CYLINDER)
		{
			t_cylinder *cylinder = (t_cylinder *)(current->data);
			if (ray_cylinder_intersect(ray, *cylinder, &t_temp) && t_temp < max_dist)
				return (1);
		}
		else if (current->type == PLANE)
		{
			t_plane *plane = (t_plane *)(current->data);
			if (ray_plane_intersect(ray, *plane, &t_temp) && t_temp < max_dist)
				return (1);
		}
		else if (current->type == TRIANGLE)
		{
			t_triangle *triangle = (t_triangle *)(current->data);
			if (ray_triangle_intersect(ray, *triangle, &t_temp) && t_temp < max_dist)
				return (1);
		}
		else if (current->type == MESH)
		{
			t_mesh *mesh = (t_mesh *)(current->data);
			int triangle_idx;
			if (ray_mesh_intersect(ray, *mesh, &t_temp, &triangle_idx) && t_temp < max_dist)
				return (1);
		}
		else if (current->type == CONE)
		{
			t_cone *cone = (t_cone *)(current->data);
			if (ray_cone_intersect(ray, *cone, &t_temp) && t_temp < max_dist)
				return (1);
		}
		else if (current->type == CUBE)
		{
			t_cube *cube = (t_cube *)(current->data);
			if (ray_cube_intersect(ray, *cube, &t_temp) && t_temp < max_dist)
				return (1);
		}
		current = current->next;
	}
	return (0);
}

int	is_in_shadow(t_scene *scene, t_vec3 hit_point, t_vec3 light_dir, double light_distance)
{
	t_ray		shadow_ray;

	// If there are no objects, nothing can cast shadows
	if (!scene->objects)
		return (0);

	//Create a shadow ray from the hit point towards the light
	shadow_ray.origin = vec3_add(hit_point, vec3_scale(light_dir, 0.001)); //offset to avoid self intersection
	shadow_ray.direction = light_dir;

	if (any_intersection(scene, shadow_ray, light_distance))
		return (1);
	return (0);
}

// int	is_in_shadow(t_scene *scene, t_vec3 hit_point, t_vec3 light_dir, double light_distance)
// {
// 	t_ray		shadow_ray;
// 	t_object	*hit_object;
// 	double		t;

// 	// If there are no objects, nothing can cast shadows
// 	if (!scene->objects)
// 		return (0);

// 	//Create a shadow ray from the hit point towards the light
// 	shadow_ray.origin = vec3_add(hit_point, vec3_scale(light_dir, 0.001)); //offset to avoid self intersection
// 	shadow_ray.direction = light_dir;

// 	if (find_closest_intersection(scene, shadow_ray, &t, &hit_object) && t < light_distance)
// 		return (1);
// 	return (0);
// }
