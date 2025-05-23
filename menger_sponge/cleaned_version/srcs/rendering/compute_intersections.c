/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_scene.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 10:45:02 by abillote          #+#    #+#             */
/*   Updated: 2025/05/05 10:28:58 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "platform.h"
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

int	find_closest_intersection(t_scene *scene, t_ray ray, double *t, t_object **hit_object)
{
	t_object	*current;
	double		t_closest;
	double		t_temp;
	int			hit_something;
	int			triangle_idx;

	current = scene->objects;
	t_closest = INFINITY;
	hit_something = 0;
	*hit_object = NULL;
	while (current)
	{
		if (current->type == SPHERE)
		{
			t_sphere *sphere = (t_sphere *)(current->data);
			if (ray_sphere_intersect(ray, *sphere, &t_temp) && t_temp < t_closest)
			{
				t_closest = t_temp;
				hit_something = 1;
				*hit_object = current;
			}
		}
		else if (current->type == CYLINDER)
		{
			t_cylinder *cylinder = (t_cylinder *)(current->data);
			if (ray_cylinder_intersect(ray, *cylinder, &t_temp) && t_temp < t_closest)
			{
				t_closest = t_temp;
				hit_something = 1;
				*hit_object = current;
			}
		}
		else if (current->type == PLANE)
		{
			t_plane	*plane = (t_plane *)(current->data);
			if (ray_plane_intersect(ray, *plane, &t_temp) && t_temp < t_closest)
			{
				t_closest = t_temp;
				hit_something = 1;
				*hit_object = current;
			}
		}
		else if (current->type == CUBE)
		{
			t_cube	*cube = (t_cube *)(current->data);
			if (ray_cube_intersect(ray, *cube, &t_temp) && t_temp < t_closest)
			{
				t_closest = t_temp;
				hit_something = 1;
				*hit_object = current;
			}
		}
		else if (current->type == TRIANGLE)
		{
			t_triangle *triangle = (t_triangle *)(current->data);
			if (ray_triangle_intersect(ray, *triangle, &t_temp) && t_temp < t_closest)
			{
				t_closest = t_temp;
				hit_something = 1;
				*hit_object = current;
			}
		}
		else if (current->type == MESH)
		{
			t_mesh *mesh = (t_mesh *)(current->data);
			if (ray_mesh_intersect(ray, *mesh, &t_temp, &triangle_idx) && t_temp < t_closest)
			{
				t_closest = t_temp;
				hit_something = 1;
				*hit_object = current;
				// Store the triangle index in the hit object for later normal computation
				// Note: This is a hack - we're assuming the user won't mess with the pointer
				// In a real implementation, this should be stored in a hit record structure
				current->material.reflectivity = triangle_idx;
			}
		}
		else if (current->type == CONE)
		{
			t_cone *cone= (t_cone *)(current->data);
			if (ray_cone_intersect(ray, *cone, &t_temp) && t_temp < t_closest)
			{
				t_closest = t_temp;
				hit_something = 1;
				*hit_object = current;
			}
		}
		current = current->next;
	}
	if (hit_something)
		*t = t_closest;
	return (hit_something);
}

void	compute_ray_intersection(t_ray ray, t_object *hit_object, double t, t_hit_record *hit_record)
{
	//calculate where the ray hit the object
	hit_record->point = vec3_add(ray.origin, vec3_scale(ray.direction, t));
	hit_record->t = t;
	hit_record->object = hit_object;
	hit_record->material = hit_object->material;
	hit_record->inside = 0;  // Initialize to false, can be set later if needed

	//calculate the normal at the hit point
	if (hit_object->type == SPHERE)
	{
		t_sphere *sphere = (t_sphere *)(hit_object->data);
		hit_record->normal = sphere_normal_at_point(hit_record->point, *sphere);
	}
	else if (hit_object->type == CYLINDER)
	{
		t_cylinder *cylinder = (t_cylinder *)(hit_object->data);
		hit_record->normal = cylinder_normal_at_point(hit_record->point, *cylinder);
	}
	else if (hit_object->type == PLANE)
	{
		t_plane *plane = (t_plane *)(hit_object->data);
		hit_record->normal = plane->normal;
		//double sided plane
		if (vec3_dot(ray.direction, hit_record->normal) > 0)
			hit_record->normal = vec3_negate(hit_record->normal);
	}
	else if (hit_object->type == CUBE)
	{
		t_cube *cube = (t_cube *)(hit_object->data);
		hit_record->normal = cube_normal_at_point(hit_record->point, *cube);
	}
	else if (hit_object->type == TRIANGLE)
	{
		t_triangle *triangle = (t_triangle *)(hit_object->data);
		hit_record->normal = triangle->normal;
		// Handle double-sided triangles by flipping normal if needed
		if (vec3_dot(ray.direction, hit_record->normal) > 0)
			hit_record->normal = vec3_negate(hit_record->normal);
	}
	else if (hit_object->type == CONE)
	{
		t_cone *cone = (t_cone *)(hit_object->data);
		hit_record->normal = cone_normal_at_point(hit_record->point, *cone);
	}
	else if (hit_object->type == MESH)
	{
		t_mesh *mesh = (t_mesh *)(hit_object->data);
		int triangle_idx = (int)hit_object->material.reflectivity;
		if (triangle_idx >= 0 && triangle_idx < mesh->triangle_count)
		{
			hit_record->normal = mesh->triangles[triangle_idx].normal;
			// Handle double-sided triangles by flipping normal if needed
			if (vec3_dot(ray.direction, hit_record->normal) > 0)
				hit_record->normal = vec3_negate(hit_record->normal);
		}
		else
		{
			// Fallback normal if index is out of bounds
			hit_record->normal = vec3_create(0, 1, 0);
		}
	}
}
