/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec3_barycentric.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/16 16:30:00 by antonsplavn      #+#    #+#             */
/*   Updated: 2025/07/16 16:30:00 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

static t_vec3	compute_bary_coords(t_vec3 v0, t_vec3 v1, t_vec3 v2, \
									double dot00, double dot01);

t_vec3	compute_barycentric(t_vec3 p, t_vec3 a, t_vec3 b, t_vec3 c)
{
	t_vec3	v0;
	t_vec3	v1;
	t_vec3	v2;
	double	dot00;
	double	dot01;

	v0 = vec3_subtract(c, a);
	v1 = vec3_subtract(b, a);
	v2 = vec3_subtract(p, a);
	dot00 = vec3_dot(v0, v0);
	dot01 = vec3_dot(v0, v1);

	return (compute_bary_coords(v0, v1, v2, dot00, dot01));
}

static t_vec3	compute_bary_coords(t_vec3 v0, t_vec3 v1, t_vec3 v2, \
									double dot00, double dot01)
{
	double	dot02;
	double	dot11;
	double	dot12;
	double	inv_denom;
	t_vec3	bary;

	dot02 = vec3_dot(v0, v2);
	dot11 = vec3_dot(v1, v1);
	dot12 = vec3_dot(v1, v2);
	inv_denom = 1.0 / (dot00 * dot11 - dot01 * dot01);
	bary.y = (dot11 * dot02 - dot01 * dot12) * inv_denom;
	bary.z = (dot00 * dot12 - dot01 * dot02) * inv_denom;
	bary.x = 1.0 - bary.y - bary.z;
	return (bary);
}

t_vec3	interpolate_vertex_normal(t_vec3 bary, t_vec3 n0, t_vec3 n1, t_vec3 n2)
{
	t_vec3	interpolated;

	interpolated.x = bary.x * n0.x + bary.y * n1.x + bary.z * n2.x;
	interpolated.y = bary.x * n0.y + bary.y * n1.y + bary.z * n2.y;
	interpolated.z = bary.x * n0.z + bary.y * n1.z + bary.z * n2.z;
	return (vec3_normalize(interpolated));
}