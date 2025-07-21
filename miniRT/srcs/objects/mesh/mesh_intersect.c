/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mesh_intersect.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 16:00:29 by antonsplavn       #+#    #+#             */
/*   Updated: 2025/07/21 10:49:50 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/miniRT.h"
#include "../../../includes/bvh.h"

/*
** Ray-mesh intersection using BVH or linear search
*/
int	ray_mesh_intersect(t_ray ray, t_mesh mesh, double *t, int *triangle_idx)
{
	if (mesh.bvh.nodes && mesh.bvh.node_count > 0)
		return (mesh_bvh_intersect(ray, (t_mesh *)&mesh, t, triangle_idx));
	else
		return (mesh_linear_intersect(ray, mesh, t, triangle_idx));
}

/*
** Ray-mesh intersection with barycentric coordinates
*/
int	ray_mesh_intersect_bary(t_mesh_intersect_params params)
{
	t_mesh_bary_params	mesh_bary_params;
	t_bvh_bary_params	bvh_bary_params;

	mesh_bary_params.t = params.t;
	mesh_bary_params.triangle_idx = params.triangle_idx;
	mesh_bary_params.bary = params.bary;
	if (params.mesh.bvh.nodes && params.mesh.bvh.node_count > 0)
	{
		bvh_bary_params.t = params.t;
		bvh_bary_params.tri_idx = params.triangle_idx;
		bvh_bary_params.bary = params.bary;
		return (mesh_bvh_intersect_bary(params.ray, (t_mesh *)&params.mesh, \
			&bvh_bary_params));
	}
	else
		return (mesh_linear_intersect_bary(params.ray, params.mesh, \
			mesh_bary_params));
}
