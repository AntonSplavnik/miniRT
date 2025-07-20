/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cubic_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 12:00:00 by abillote          #+#    #+#             */
/*   Updated: 2025/07/20 22:30:55 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

/*
** Maps a 3D point to 2D UV coordinates for cubic mapping
*/
t_vec2	cubic_map(t_cube *cube, t_vec3 local_point)
{
	double	abs_x;
	double	abs_y;
	double	abs_z;

	abs_x = fabs(local_point.x);
	abs_y = fabs(local_point.y);
	abs_z = fabs(local_point.z);
	if (abs_x >= abs_y && abs_x >= abs_z)
		return (map_x_face(cube, local_point));
	else if (abs_y >= abs_x && abs_y >= abs_z)
		return (map_y_face(cube, local_point));
	else
		return (map_z_face(cube, local_point));
}

/*
** Map UV coordinates for X-faces of the cube
*/
t_vec2	map_x_face(t_cube *cube, t_vec3 local_point)
{
	int		sign;
	double	u;
	double	v;
	double	half_size;
	t_vec2	uv;

	half_size = cube->side_length / 2.0;
	if (local_point.x > 0)
		sign = 1;
	else
		sign = -1;
	u = (local_point.z * sign) / half_size * 0.5 + 0.5;
	v = (local_point.y) / half_size * 0.5 + 0.5;
	uv.u = u;
	uv.v = v;
	return (uv);
}

/*
** Map UV coordinates for Y-faces of the cube
*/
t_vec2	map_y_face(t_cube *cube, t_vec3 local_point)
{
	int		sign;
	double	u;
	double	v;
	double	half_size;
	t_vec2	uv;

	half_size = cube->side_length / 2.0;
	if (local_point.y > 0)
		sign = 1;
	else
		sign = -1;
	u = (local_point.x) / half_size * 0.5 + 0.5;
	v = (local_point.z * sign) / half_size * 0.5 + 0.5;
	uv.u = u;
	uv.v = v;
	return (uv);
}

/*
** Map UV coordinates for Z-faces of the cube
*/
t_vec2	map_z_face(t_cube *cube, t_vec3 local_point)
{
	int		sign;
	double	u;
	double	v;
	double	half_size;
	t_vec2	uv;

	half_size = cube->side_length / 2.0;
	if (local_point.z > 0)
		sign = 1;
	else
		sign = -1;
	u = (local_point.x * -sign) / half_size * 0.5 + 0.5;
	v = (local_point.y) / half_size * 0.5 + 0.5;
	uv.u = u;
	uv.v = v;
	return (uv);
}
