/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compute_ray_intersection.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 12:00:00 by abillote          #+#    #+#             */
/*   Updated: 2025/07/14 03:22:28 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

/*
** Calculate normal for sphere objects
*/
void	compute_sphere_normal(t_hit_record *hit_record)
{
	t_sphere	*sphere;

	sphere = (t_sphere *)(hit_record->object->data);
	hit_record->original_normal = sphere_normal_at_point(\
		hit_record->point, *sphere);
}

/*
** Calculate normal for cylinder objects
*/
void	compute_cylinder_normal(t_hit_record *hit_record)
{
	t_cylinder	*cylinder;

	cylinder = (t_cylinder *)(hit_record->object->data);
	hit_record->original_normal = cylinder_normal_at_point(\
		hit_record->point, *cylinder);
}

/*
** Calculate normal for plane objects with double-sided support
*/
void	compute_plane_normal(t_hit_record *hit_record, t_ray ray)
{
	t_plane	*plane;

	plane = (t_plane *)(hit_record->object->data);
	hit_record->original_normal = plane->normal;
	if (vec3_dot(ray.direction, hit_record->original_normal) > 0)
		hit_record->original_normal = vec3_negate(\
			hit_record->original_normal);
}

/*
** Calculate normal for cube objects
*/
void	compute_cube_normal(t_hit_record *hit_record)
{
	t_cube	*cube;

	cube = (t_cube *)(hit_record->object->data);
	hit_record->original_normal = cube_normal_at_point(\
		hit_record->point, *cube);
}

/*
** Calculate normal for cone objects
*/
void	compute_cone_normal(t_hit_record *hit_record)
{
	t_cone	*cone;

	cone = (t_cone *)(hit_record->object->data);
	hit_record->original_normal = cone_normal_at_point(\
		hit_record->point, *cone);
}
