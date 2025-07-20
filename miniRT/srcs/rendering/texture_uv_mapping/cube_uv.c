/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cube_uv.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 01:10:05 by antonsplavn       #+#    #+#             */
/*   Updated: 2025/07/20 20:01:13 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

static t_vec2	cube_face_y(t_vec3 local_point, double half_size)
{
	t_vec2	uv;

	if (local_point.y > 0)
	{
		uv.u = (local_point.x / half_size + 1.0) / 2.0;
		uv.v = (local_point.z / half_size + 1.0) / 2.0;
	}
	else
	{
		uv.u = (local_point.x / half_size + 1.0) / 2.0;
		uv.v = (-local_point.z / half_size + 1.0) / 2.0;
	}
	return (uv);
}

static t_vec2	cube_face_z(t_vec3 local_point, double half_size)
{
	t_vec2	uv;

	if (local_point.z > 0)
	{
		uv.u = (local_point.x / half_size + 1.0) / 2.0;
		uv.v = (-local_point.y / half_size + 1.0) / 2.0;
	}
	else
	{
		uv.u = (-local_point.x / half_size + 1.0) / 2.0;
		uv.v = (-local_point.y / half_size + 1.0) / 2.0;
	}
	return (uv);
}

static t_vec2	cube_face_x(t_vec3 local_point, double half_size)
{
	t_vec2	uv;

	if (local_point.x > 0)
	{
		uv.u = (-local_point.z / half_size + 1.0) / 2.0;
		uv.v = (-local_point.y / half_size + 1.0) / 2.0;
	}
	else
	{
		uv.u = (local_point.z / half_size + 1.0) / 2.0;
		uv.v = (-local_point.y / half_size + 1.0) / 2.0;
	}
	return (uv);
}

t_vec2	cube_uv_mapping(t_vec3 point, t_cube cube)
{
	t_vec3	local_point;
	double	abs_x;
	double	abs_y;
	double	abs_z;
	double	half_size;

	local_point = vec3_subtract(point, cube.center);
	abs_x = fabs(local_point.x);
	abs_y = fabs(local_point.y);
	abs_z = fabs(local_point.z);
	half_size = cube.side_length / 2.0;
	if (abs_x >= abs_y && abs_x >= abs_z)
		return (cube_face_x(local_point, half_size));
	else if (abs_y >= abs_x && abs_y >= abs_z)
		return (cube_face_y(local_point, half_size));
	else
		return (cube_face_z(local_point, half_size));
}
