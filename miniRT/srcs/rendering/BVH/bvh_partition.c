/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh_partition.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/01 00:00:00 by abillote          #+#    #+#             */
/*   Updated: 2024/01/01 00:00:00 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

static int	find_left_partition(t_triangle_centroid *centroids,
		t_partition_params *params, int left)
{
	double	centroid_val;

	while (left <= params->end)
	{
		centroid_val = get_centroid_axis_value(&centroids[left],
				params->axis);
		if (centroid_val >= params->threshold)
			break ;
		left++;
	}
	return (left);
}

static int	find_right_partition(t_triangle_centroid *centroids,
		t_partition_params *params, int right)
{
	double	centroid_val;

	while (right >= params->start)
	{
		centroid_val = get_centroid_axis_value(&centroids[right],
				params->axis);
		if (centroid_val < params->threshold)
			break ;
		right--;
	}
	return (right);
}

int	partition_by_threshold_fast(t_triangle_centroid *centroids,
		t_partition_params *params)
{
	int	left;
	int	right;

	left = params->start;
	right = params->end;
	while (left <= right)
	{
		left = find_left_partition(centroids, params, left);
		right = find_right_partition(centroids, params, right);
		if (left < right)
			ft_swap_centroid(&centroids[left], &centroids[right]);
		left++;
		right--;
	}
	return (right + 1);
}
