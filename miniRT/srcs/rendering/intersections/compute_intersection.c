/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compute_intersection.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 10:45:02 by abillote          #+#    #+#             */
/*   Updated: 2025/07/21 03:16:55 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

/*
** Test intersection with basic object types (sphere, cylinder, plane)
*/
int	test_basic_shapes(t_object *current, t_ray ray, double *t_temp)
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
** Test intersection with cube and cone objects
*/
int	test_cube_cone(t_object *current, t_ray ray, double *t_temp)
{
	t_cube		*cube;
	t_cone		*cone;

	if (current->type == CUBE)
	{
		cube = (t_cube *)(current->data);
		return (ray_cube_intersect(ray, *cube, t_temp));
	}
	if (current->type == CONE)
	{
		cone = (t_cone *)(current->data);
		return (ray_cone_intersect(ray, *cone, t_temp));
	}
	return (0);
}

/*
** Test intersection with triangle and mesh objects using barycentric coords
*/
int	test_triangle_mesh_bary(t_object *current, t_ray ray, double *t_temp,
	t_hit_record *hit_record)
{
	t_mesh					*mesh;
	t_triangle				*triangle;
	int						triangle_idx;
	t_mesh_intersect_params	params;

	if (current->type == TRIANGLE)
	{
		triangle = (t_triangle *)(current->data);
		return (ray_triangle_intersect_bary(ray, *triangle, t_temp, \
			&hit_record->barycentric));
	}
	if (current->type == MESH)
	{
		mesh = (t_mesh *)(current->data);
		params = (t_mesh_intersect_params){ray, *mesh, t_temp, \
			&triangle_idx, &hit_record->barycentric};
		if (ray_mesh_intersect_bary(params))
		{
			hit_record->triangle_idx = triangle_idx;
			return (1);
		}
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
