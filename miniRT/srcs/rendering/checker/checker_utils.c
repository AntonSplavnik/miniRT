/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 12:00:00 by abillote          #+#    #+#             */
/*   Updated: 2025/07/14 02:38:04 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

/*
** Project point onto plane coordinate system for UV mapping
*/
t_vec2	project_to_plane_uv(t_plane *plane, t_vec3 point, t_vec3 u_axis)
{
	t_vec3	to_point;
	t_vec3	v_axis;
	double	scale;
	double	u;
	t_vec2	uv;

	to_point = vec3_subtract(point, plane->point);
	v_axis = vec3_normalize(vec3_cross(plane->normal, u_axis));
	scale = 0.1;
	u = vec3_dot(to_point, u_axis) * scale;
	uv.u = u;
	uv.v = vec3_dot(to_point, v_axis) * scale;
	return (uv);
}

/*
** Get UV coordinates for cylinder checker pattern
*/
t_vec2	get_cylinder_checker_uv(t_object *object, t_vec3 point)
{
	t_cylinder	*cylinder;
	t_vec3		cylinder_local_point;

	cylinder = (t_cylinder *)object->data;
	cylinder_local_point = vec3_subtract(point, cylinder->center);
	return (cylindrical_map(cylinder_local_point, cylinder->radius,
			cylinder->axis));
}

/*
** Maps a 3D point to 2D UV coordinates for cylindrical mapping
*/
t_vec2	cylindrical_map(t_vec3 point, double radius, t_vec3 axis)
{
	axis = vec3_normalize(axis);
	return (calculate_cylinder_checker_uv(point, radius, axis));
}

/*
** Calculate UV coordinates for cylindrical checker mapping
*/
t_vec2	calculate_cylinder_checker_uv(t_vec3 point, double radius, t_vec3 axis)
{
	t_vec3	u_axis;
	t_vec3	v_axis;
	double	u_proj;
	double	v_proj;
	t_vec2	uv;

	if (fabs(axis.z) > 0.1 || fabs(axis.y) > 0.1)
		u_axis = vec3_normalize(vec3_cross(axis, vec3_create(1, 0, 0)));
	else
		u_axis = vec3_normalize(vec3_cross(axis, vec3_create(0, 1, 0)));
	v_axis = vec3_normalize(vec3_cross(axis, u_axis));
	u_proj = vec3_dot(point, u_axis);
	v_proj = vec3_dot(point, v_axis);
	uv.u = (atan2(v_proj, u_proj) + M_PI) / (2.0 * M_PI);
	uv.v = vec3_dot(point, axis) / (2.0 * M_PI * radius);
	return (uv);
}

/*
** Get UV coordinates for cube checker pattern
*/
t_vec2	get_cube_checker_uv(t_object *object, t_vec3 point)
{
	t_cube	*cube;
	t_vec3	local_point;

	cube = (t_cube *)object->data;
	local_point = vec3_subtract(point, cube->center);
	return (cubic_map(cube, local_point));
}
