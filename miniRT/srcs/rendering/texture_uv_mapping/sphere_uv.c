/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere_uv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 01:09:04 by antonsplavn       #+#    #+#             */
/*   Updated: 2025/07/20 20:02:27 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

static t_vec2	sphere_uv_calc(t_vec3 local_point)
{
	t_vec2	uv;
	double	theta;
	double	phi;

	theta = atan2(local_point.x, local_point.z);
	phi = acos(-local_point.y);
	uv.u = 0.5 + theta / (2.0 * M_PI);
	uv.v = phi / M_PI;
	return (uv);
}

static t_vec2	sphere_uv_adjust(t_vec2 uv)
{
	if (uv.u < 0)
		uv.u += 1.0;
	if (uv.u > 1)
		uv.u -= 1.0;
	uv.v = 1.0 - uv.v;
	uv.u = 1.0 - uv.u;
	return (uv);
}

t_vec2	sphere_uv_mapping(t_vec3 point, t_sphere sphere)
{
	t_vec2	uv;
	t_vec3	local_point;

	local_point = vec3_subtract(point, sphere.center);
	local_point = vec3_normalize(local_point);
	uv = sphere_uv_calc(local_point);
	uv = sphere_uv_adjust(uv);
	return (uv);
}
