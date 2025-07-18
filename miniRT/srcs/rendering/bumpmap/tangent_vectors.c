/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tangent_vectors.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 11:09:53 by abillote          #+#    #+#             */
/*   Updated: 2025/07/14 01:59:55 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

void	calculate_tangent_vectors(t_vec3 normal, t_vec3 *tangent,
								t_vec3 *bitangent)
{
	t_vec3	arbitrary;

	if (fabs(normal.x) > 0.9)
		arbitrary = vec3_create(0, 1, 0);
	else
		arbitrary = vec3_create(1, 0, 0);
	*tangent = vec3_normalize(vec3_cross(normal, arbitrary));
	*bitangent = vec3_normalize(vec3_cross(normal, *tangent));
}

void	calculate_sphere_tangent_vectors(t_hit_record *hit_record,
										t_vec3 *tangent, t_vec3 *bitangent)
{
	t_sphere	*sphere;
	t_vec3		local_point;
	double		theta;

	sphere = (t_sphere *)hit_record->object->data;
	local_point = vec3_subtract(hit_record->point, sphere->center);
	local_point = vec3_normalize(local_point);
	theta = atan2(local_point.x, local_point.z);
	tangent->x = cos(theta);
	tangent->y = 0;
	tangent->z = -sin(theta);
	*tangent = vec3_normalize(*tangent);
	*bitangent = vec3_normalize(vec3_cross(hit_record->normal, *tangent));
}

static void	set_plane_tangent_z(t_vec3 *tangent, t_vec3 *bitangent)
{
	*tangent = vec3_create(1, 0, 0);
	*bitangent = vec3_create(0, 1, 0);
}

static void	set_plane_tangent_y(t_vec3 *tangent, t_vec3 *bitangent)
{
	*tangent = vec3_create(1, 0, 0);
	*bitangent = vec3_create(0, 0, 1);
}

void	calculate_plane_tangent_vectors(t_hit_record *hit_record,
										t_vec3 *tangent, t_vec3 *bitangent)
{
	t_plane	*plane;
	t_vec3	dot_product;

	plane = (t_plane *)hit_record->object->data;
	if (fabs(plane->normal.z) > 0.9)
		set_plane_tangent_z(tangent, bitangent);
	else if (fabs(plane->normal.y) > 0.9)
		set_plane_tangent_y(tangent, bitangent);
	else
	{
		*tangent = vec3_create(0, 1, 0);
		*bitangent = vec3_create(0, 0, 1);
	}
	dot_product = vec3_scale(plane->normal, vec3_dot(*tangent, plane->normal));
	*tangent = vec3_normalize(vec3_subtract(*tangent, dot_product));
	*bitangent = vec3_normalize(vec3_cross(plane->normal, *tangent));
}
