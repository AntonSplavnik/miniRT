/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mesh_partition_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 05:02:23 by antonsplavn       #+#    #+#             */
/*   Updated: 2025/07/21 05:02:24 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

static int	find_left_partition(t_partition_params params, int left,
				int range_end)
{
	int		current_left;
	double	centroid_val;

	current_left = left;
	while (current_left <= range_end)
	{
		centroid_val = get_centroid_axis_value(&params.centroids[current_left],
				params.axis);
		if (centroid_val >= params.threshold)
			break ;
		current_left++;
	}
	return (current_left);
}

static int	find_right_partition(t_partition_params params, int right,
				int range_start)
{
	int		current_right;
	double	centroid_val;

	current_right = right;
	while (current_right >= range_start)
	{
		centroid_val = get_centroid_axis_value(&params.centroids[current_right],
				params.axis);
		if (centroid_val < params.threshold)
			break ;
		current_right--;
	}
	return (current_right);
}

int	partition_by_threshold(t_partition_params params, t_centroid_range range)
{
	int	left;
	int	right;

	left = range.start;
	right = range.end;
	while (left <= right)
	{
		left = find_left_partition(params, left, range.end);
		right = find_right_partition(params, right, range.start);
		if (left < right)
			ft_swap_centroid(&params.centroids[left], &params.centroids[right]);
		left++;
		right--;
	}
	return (right + 1);
}
