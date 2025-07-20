/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder_uv.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 01:09:40 by antonsplavn       #+#    #+#             */
/*   Updated: 2025/07/20 20:02:16 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

static t_vec2	cylinder_calc_coords(t_vec3 local_point, t_cylinder cylinder,
	t_vec3 u_axis, t_vec3 v_axis)
{
	t_vec2	uv;
	double	u_proj;
	double	v_proj;
	double	theta;
	double	height_coord;

	u_proj = vec3_dot(local_point, u_axis);
	v_proj = vec3_dot(local_point, v_axis);
	theta = atan2(v_proj, u_proj);
	uv.u = (theta + M_PI) / (2.0 * M_PI);
	height_coord = vec3_dot(local_point, cylinder.axis);
	uv.v = (height_coord + cylinder.height / 2.0) / cylinder.height;
	if (uv.v < 0)
		uv.v = 0;
	if (uv.v > 1)
		uv.v = 1;
	return (uv);
}

static t_vec3	cylinder_get_u_axis(t_cylinder cylinder)
{
	t_vec3	u_axis;

	if (fabs(cylinder.axis.z) > 0.1 || fabs(cylinder.axis.y) > 0.1)
		u_axis = vec3_normalize(vec3_cross(cylinder.axis, \
			vec3_create(1, 0, 0)));
	else
		u_axis = vec3_normalize(vec3_cross(cylinder.axis, \
			vec3_create(0, 1, 0)));
	return (u_axis);
}

t_vec2	cylinder_uv_mapping(t_vec3 point, t_cylinder cylinder)
{
	t_vec3	local_point;
	t_vec3	u_axis;
	t_vec3	v_axis;

	local_point = vec3_subtract(point, cylinder.center);
	u_axis = cylinder_get_u_axis(cylinder);
	v_axis = vec3_normalize(vec3_cross(cylinder.axis, u_axis));
	return (cylinder_calc_coords(local_point, cylinder, u_axis, v_axis));
}
