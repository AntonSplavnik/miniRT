/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 16:20:01 by abillote          #+#    #+#             */
/*   Updated: 2025/07/20 22:31:17 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

/*
** Maps a 3D point to 2D UV coordinates for spherical mapping
*/
t_vec2	spherical_map(t_vec3 point)
{
	double	theta;
	double	phi;
	double	u;
	double	v;
	t_vec2	uv;

	point = vec3_normalize(point);
	theta = atan2(point.x, point.z);
	phi = acos(point.y);
	u = theta / (2 * M_PI);
	u = u + 0.5;
	if (u < 0)
		u += 1.0;
	if (u > 1)
		u -= 1.0;
	v = phi / M_PI;
	v /= 2.0;
	uv.u = u;
	uv.v = v;
	return (uv);
}

/*
** Determines whether a point should use the primary or checker color
** for a 2D checkerboard pattern
*/
int	is_checker_point_2d(double u, double v, double checker_size)
{
	double	scaled_u;
	double	scaled_v;
	int		cell_u;
	int		cell_v;

	scaled_u = u * checker_size;
	scaled_v = v * checker_size;
	cell_u = (int)floor(scaled_u);
	cell_v = (int)floor(scaled_v);
	return ((cell_u + cell_v) % 2);
}

/*
** Get UV coordinates for plane checker pattern
*/
t_vec2	get_plane_checker_uv(t_object *object, t_vec3 point)
{
	t_plane	*plane;
	t_vec3	normal;
	t_vec3	u_axis;

	plane = (t_plane *)object->data;
	normal = plane->normal;
	if (fabs(normal.z) > 0.1 || fabs(normal.y) > 0.1)
		u_axis = vec3_normalize(vec3_cross(normal, vec3_create(1, 0, 0)));
	else
		u_axis = vec3_normalize(vec3_cross(normal, vec3_create(0, 1, 0)));
	return (project_to_plane_uv(plane, point, u_axis));
}

/*
** Get UV coordinates for sphere checker pattern
*/
t_vec2	get_sphere_checker_uv(t_object *object, t_vec3 point)
{
	t_sphere	*sphere;
	t_vec3		sphere_local_point;

	sphere = (t_sphere *)object->data;
	sphere_local_point = vec3_subtract(point, sphere->center);
	return (spherical_map(sphere_local_point));
}

/*
** Gets the appropriate color at a point for a material with potential
** checkerboard pattern
*/
t_color	get_checker_color(t_material material, t_object *object, t_vec3 point)
{
	t_vec2	point_2d;

	if (!material.has_checker)
		return (material.color);
	if (object->type == PLANE)
		point_2d = get_plane_checker_uv(object, point);
	else if (object->type == SPHERE)
		point_2d = get_sphere_checker_uv(object, point);
	else if (object->type == CYLINDER)
		point_2d = get_cylinder_checker_uv(object, point);
	else if (object->type == CUBE)
		point_2d = get_cube_checker_uv(object, point);
	else
		point_2d = spherical_map(point);
	if (is_checker_point_2d(point_2d.u, point_2d.v, material.checker_size))
		return (material.checker_color);
	else
		return (material.color);
}
