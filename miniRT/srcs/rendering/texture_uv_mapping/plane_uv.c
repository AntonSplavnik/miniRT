/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane_uv.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 01:09:26 by antonsplavn       #+#    #+#             */
/*   Updated: 2025/07/20 20:01:53 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

static t_vec3	plane_get_v_axis(t_plane plane)
{
	t_vec3	v_axis;

	if (fabs(plane.normal.z) > 0.9)
		v_axis = vec3_create(0, 1, 0);
	else if (fabs(plane.normal.y) > 0.9)
		v_axis = vec3_create(0, 0, 1);
	else
		v_axis = vec3_create(0, 0, 1);
	return (v_axis);
}

static t_vec2	plane_calc_uv(t_vec3 to_point, t_vec3 u_axis, t_vec3 v_axis)
{
	t_vec2	uv;
	double	scale;

	scale = 0.1;
	uv.u = vec3_dot(to_point, u_axis) * scale;
	uv.v = vec3_dot(to_point, v_axis) * scale;
	uv.u = uv.u - floor(uv.u);
	uv.v = uv.v - floor(uv.v);
	return (uv);
}

static t_vec3	plane_get_u_axis(t_plane plane)
{
	t_vec3	u_axis;

	if (fabs(plane.normal.z) > 0.9)
		u_axis = vec3_create(1, 0, 0);
	else if (fabs(plane.normal.y) > 0.9)
		u_axis = vec3_create(1, 0, 0);
	else
		u_axis = vec3_create(0, 1, 0);
	return (u_axis);
}

t_vec2	plane_uv_mapping(t_vec3 point, t_plane plane)
{
	t_vec3	u_axis;
	t_vec3	v_axis;
	t_vec3	to_point;

	u_axis = plane_get_u_axis(plane);
	v_axis = plane_get_v_axis(plane);
	u_axis = vec3_normalize(vec3_subtract(u_axis, \
		vec3_scale(plane.normal, vec3_dot(u_axis, plane.normal))));
	v_axis = vec3_normalize(vec3_cross(plane.normal, u_axis));
	to_point = vec3_subtract(point, plane.point);
	return (plane_calc_uv(to_point, u_axis, v_axis));
}
