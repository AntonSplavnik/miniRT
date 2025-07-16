/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_uv_main.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 01:08:05 by antonsplavn       #+#    #+#             */
/*   Updated: 2025/07/14 01:08:15 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

static t_vec2	calculate_sphere_uv(t_vec3 point, t_object *object)
{
	return (sphere_uv_mapping(point, *(t_sphere *)object->data));
}

static t_vec2	calculate_plane_uv(t_vec3 point, t_object *object)
{
	return (plane_uv_mapping(point, *(t_plane *)object->data));
}

static t_vec2	calculate_cylinder_uv(t_vec3 point, t_object *object)
{
	return (cylinder_uv_mapping(point, *(t_cylinder *)object->data));
}

static t_vec2	calculate_cube_uv(t_vec3 point, t_object *object)
{
	return (cube_uv_mapping(point, *(t_cube *)object->data));
}

t_vec2	calculate_uv_coordinates(t_vec3 point, t_object *object)
{
	t_vec2	uv;

	uv.u = 0.0;
	uv.v = 0.0;
	if (object->type == SPHERE)
		uv = calculate_sphere_uv(point, object);
	else if (object->type == PLANE)
		uv = calculate_plane_uv(point, object);
	else if (object->type == CYLINDER)
		uv = calculate_cylinder_uv(point, object);
	else if (object->type == CUBE)
		uv = calculate_cube_uv(point, object);
	else
	{
		uv.u = point.x * 0.1;
		uv.v = point.y * 0.1;
	}
	return (uv);
}
