/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh_intersect_batch.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by abillote          #+#    #+#             */
/*   Updated: 2024/01/01 00:00:00 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

static int	test_batch_with_params(t_mesh *mesh, t_ray ray,
		t_batch_test_params *params)
{
	int					i;
	int					idx;
	t_intersect_result	result;

	if (params->count <= 0 || params->start < 0)
		return (params->hit_something);
	result.closest_t = params->closest_t;
	result.tri_idx = params->tri_idx;
	result.hit_something = params->hit_something;
	i = 0;
	while (i < params->count)
	{
		idx = params->start + i;
		result.hit_something = test_single_triangle(mesh, ray, idx, &result);
		i++;
	}
	return (result.hit_something);
}

int	test_leaf_triangles_batch(t_mesh *mesh, t_ray ray,
		t_leaf_batch_params *leaf_params)
{
	t_batch_test_params	params;

	params = (t_batch_test_params){leaf_params->start, leaf_params->count,
		leaf_params->closest_t, leaf_params->tri_idx,
		leaf_params->hit_something};
	return (test_batch_with_params(mesh, ray, &params));
}

static int	test_batch_bary_with_params(t_mesh *mesh, t_ray ray,
		t_batch_test_bary_params *params)
{
	int						i;
	int						idx;
	t_intersect_bary_result	result;

	if (params->count <= 0 || params->start < 0)
		return (params->bary_params->hit_something);
	result.closest_t = params->bary_params->closest_t;
	result.tri_idx = params->bary_params->tri_idx;
	result.closest_bary = params->bary_params->closest_bary;
	result.hit_something = params->bary_params->hit_something;
	i = 0;
	while (i < params->count)
	{
		idx = params->start + i;
		result.hit_something = test_single_triangle_bary(mesh, ray, idx,
				&result);
		i++;
	}
	return (result.hit_something);
}

int	test_leaf_triangles_batch_bary(t_mesh *mesh, t_ray ray,
		t_leaf_batch_bary_params *leaf_params)
{
	t_batch_test_bary_params	batch_params;

	batch_params = (t_batch_test_bary_params){leaf_params->start,
		leaf_params->count, leaf_params->bary_params};
	return (test_batch_bary_with_params(mesh, ray, &batch_params));
}
