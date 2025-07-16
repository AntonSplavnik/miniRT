/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compute_intersection.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 10:45:02 by abillote          #+#    #+#             */
/*   Updated: 2025/07/14 03:16:26 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

/*
** Check if a point is in shadow by testing for objects between
** the hit point and the light source
*/
int	is_in_shadow(t_scene *scene, t_vec3 hit_point, t_vec3 light_dir,
	double light_distance)
{
	t_ray			shadow_ray;
	t_object		*hit_object;
	t_hit_record	dummy_record;

	shadow_ray.origin = vec3_add(hit_point, vec3_scale(light_dir, 0.001));
	shadow_ray.direction = light_dir;
	if (find_closest_intersection(scene, shadow_ray, &dummy_record, \
		&hit_object) && dummy_record.t < light_distance)
		return (1);
	return (0);
}

/*
** Test intersection with basic object types (sphere, cylinder, plane)
*/
int	test_basic_intersections(t_object *current, t_ray ray, double *t_temp)
{
	t_sphere	*sphere;
	t_cylinder	*cylinder;
	t_plane		*plane;

	if (current->type == SPHERE)
	{
		sphere = (t_sphere *)(current->data);
		return (ray_sphere_intersect(ray, *sphere, t_temp));
	}
	if (current->type == CYLINDER)
	{
		cylinder = (t_cylinder *)(current->data);
		return (ray_cylinder_intersect(ray, *cylinder, t_temp));
	}
	if (current->type == PLANE)
	{
		plane = (t_plane *)(current->data);
		return (ray_plane_intersect(ray, *plane, t_temp));
	}
	return (0);
}

/*
** Test intersection with cube and triangle objects
*/
int	test_cube_triangle(t_object *current, t_ray ray, double *t_temp)
{
	t_cube		*cube;
	t_triangle	*triangle;

	if (current->type == CUBE)
	{
		cube = (t_cube *)(current->data);
		return (ray_cube_intersect(ray, *cube, t_temp));
	}
	if (current->type == TRIANGLE)
	{
		triangle = (t_triangle *)(current->data);
		return (ray_triangle_intersect(ray, *triangle, t_temp));
	}
	return (0);
}

/*
** Test intersection with mesh and cone objects
*/
int	test_mesh_cone(t_object *current, t_ray ray, double *t_temp,
	t_hit_record *hit_record)
{
	t_mesh	*mesh;
	t_cone	*cone;
	int		triangle_idx;

	if (current->type == MESH)
	{
		mesh = (t_mesh *)(current->data);
		if (ray_mesh_intersect(ray, *mesh, t_temp, &triangle_idx))
		{
			hit_record->triangle_idx = triangle_idx;
			return (1);
		}
	}
	if (current->type == CONE)
	{
		cone = (t_cone *)(current->data);
		return (ray_cone_intersect(ray, *cone, t_temp));
	}
	return (0);
}

/*
** Update closest intersection if current intersection is closer
*/
void	update_closest_hit(double t_temp, t_intersection_data *data,
	t_object *current)
{
	if (t_temp < data->t_closest)
	{
		data->t_closest = t_temp;
		data->hit_something = 1;
		data->hit_object = current;
	}
}
