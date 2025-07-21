/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_ray_intersect_bvh_5.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 04:24:24 by antonsplavn       #+#    #+#             */
/*   Updated: 2025/07/21 04:41:06 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"
#include "../../includes/bvh.h"

int	intersect_triangle(t_ray ray, t_triangle *triangle, double *t_temp)
{
	return (ray_triangle_intersect(ray, *triangle, t_temp));
}

int	intersect_triangle_bary(t_ray ray, t_triangle *triangle, double *t_temp, \
							t_vec3 *bary)
{
	return (ray_triangle_intersect_bary(ray, *triangle, t_temp, bary));
}

int	intersect_mesh(t_ray ray, t_mesh *mesh, double *t_temp, int *triangle_idx)
{
	return (ray_mesh_intersect(ray, *mesh, t_temp, triangle_idx));
}

int	intersect_mesh_bary(t_ray ray, t_mesh *mesh, double *t_temp,
		t_mesh_bary_params out_params)
{
	t_mesh_intersect_params	mesh_params;

	mesh_params.ray = ray;
	mesh_params.mesh = *mesh;
	mesh_params.t = t_temp;
	mesh_params.triangle_idx = out_params.triangle_idx;
	mesh_params.bary = out_params.bary;
	return (ray_mesh_intersect_bary(mesh_params));
}
