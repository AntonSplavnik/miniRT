/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 10:14:49 by abillote          #+#    #+#             */
/*   Updated: 2025/07/13 23:08:49 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

/*
** Rotates a point around the X axis using given rotation values
*/
static t_vec3	rotate_x_axis(t_vec3 point, double cos_x, double sin_x)
{
	t_vec3	result;

	result.x = point.x;
	result.y = point.y * cos_x - point.z * sin_x;
	result.z = point.y * sin_x + point.z * cos_x;
	return (result);
}

/*
** Rotates a point around the Y axis using given rotation values
*/
static t_vec3	rotate_y_axis(t_vec3 point, double cos_y, double sin_y)
{
	t_vec3	result;

	result.x = point.x * cos_y + point.z * sin_y;
	result.y = point.y;
	result.z = -point.x * sin_y + point.z * cos_y;
	return (result);
}

/*
** Rotates a point around the Z axis using given rotation values
*/
static t_vec3	rotate_z_axis(t_vec3 point, double cos_z, double sin_z)
{
	t_vec3	result;

	result.x = point.x * cos_z - point.y * sin_z;
	result.y = point.x * sin_z + point.y * cos_z;
	result.z = point.z;
	return (result);
}

/*
** Applies 3D rotation to a point using Euler angles (X, Y, Z order)
*/
t_vec3	rotate_point(t_vec3 point, t_vec3 rotation)
{
	t_vec3	temp;
	double	trig_vals[6];

	trig_vals[0] = cos(rotation.x);
	trig_vals[1] = sin(rotation.x);
	trig_vals[2] = cos(rotation.y);
	trig_vals[3] = sin(rotation.y);
	trig_vals[4] = cos(rotation.z);
	trig_vals[5] = sin(rotation.z);
	temp = rotate_x_axis(point, trig_vals[0], trig_vals[1]);
	temp = rotate_y_axis(temp, trig_vals[2], trig_vals[3]);
	temp = rotate_z_axis(temp, trig_vals[4], trig_vals[5]);
	return (temp);
}
