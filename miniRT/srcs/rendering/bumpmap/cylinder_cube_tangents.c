/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder_cube_tangents.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 11:09:53 by abillote          #+#    #+#             */
/*   Updated: 2025/07/20 22:41:29 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

void	calculate_cylinder_tangent_vectors(t_hit_record *hit_record,
										t_vec3 *tangent, t_vec3 *bitangent)
{
	t_cylinder	*cylinder;
	t_vec3		local_point;
	double		height_projection;
	double		half_height;
	t_vec3		radial;

	cylinder = (t_cylinder *)hit_record->object->data;
	local_point = vec3_subtract(hit_record->point, cylinder->center);
	height_projection = vec3_dot(local_point, cylinder->axis);
	half_height = cylinder->height / 2.0;
	if (fabs(fabs(height_projection) - half_height) < 0.001)
		calculate_tangent_vectors(hit_record->normal, tangent, bitangent);
	else
	{
		*tangent = cylinder->axis;
		radial = vec3_subtract(local_point, \
			vec3_scale(cylinder->axis, height_projection));
		radial = vec3_normalize(radial);
		*bitangent = vec3_normalize(vec3_cross(cylinder->axis, radial));
	}
}

static void	set_cube_tangent_x(t_vec3 local_point, t_vec3 *tangent,
							t_vec3 *bitangent)
{
	if (local_point.x > 0)
		*tangent = vec3_create(0, 0, -1);
	else
		*tangent = vec3_create(0, 0, 1);
	*bitangent = vec3_create(0, -1, 0);
}

static void	set_cube_tangent_y(t_vec3 local_point, t_vec3 *tangent,
							t_vec3 *bitangent)
{
	*tangent = vec3_create(1, 0, 0);
	if (local_point.y > 0)
		*bitangent = vec3_create(0, 0, 1);
	else
		*bitangent = vec3_create(0, 0, -1);
}

static void	set_cube_tangent_z(t_vec3 local_point, t_vec3 *tangent,
							t_vec3 *bitangent)
{
	if (local_point.z > 0)
		*tangent = vec3_create(1, 0, 0);
	else
		*tangent = vec3_create(-1, 0, 0);
	*bitangent = vec3_create(0, -1, 0);
}

void	calculate_cube_tangent_vectors(t_hit_record *hit_record,
									t_vec3 *tangent, t_vec3 *bitangent)
{
	t_cube	*cube;
	t_vec3	local_point;
	double	abs_x;
	double	abs_y;
	double	abs_z;

	cube = (t_cube *)hit_record->object->data;
	local_point = vec3_subtract(hit_record->point, cube->center);
	abs_x = fabs(local_point.x);
	abs_y = fabs(local_point.y);
	abs_z = fabs(local_point.z);
	if (abs_x >= abs_y && abs_x >= abs_z)
		set_cube_tangent_x(local_point, tangent, bitangent);
	else if (abs_y >= abs_x && abs_y >= abs_z)
		set_cube_tangent_y(local_point, tangent, bitangent);
	else
		set_cube_tangent_z(local_point, tangent, bitangent);
	*tangent = vec3_normalize(*tangent);
	*bitangent = vec3_normalize(*bitangent);
}
