/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calculate_object_aabb_5.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 00:00:00 by antonsplavn      #+#    #+#             */
/*   Updated: 2025/06/23 00:00:00 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"
#include "../../includes/bvh.h"

void	pad_triangle_aabb_x(t_aabb *bounds, double epsilon)
{
	if (bounds->max.x - bounds->min.x < epsilon)
	{
		bounds->min.x -= epsilon;
		bounds->max.x += epsilon;
	}
}

void	pad_triangle_aabb_y(t_aabb *bounds, double epsilon)
{
	if (bounds->max.y - bounds->min.y < epsilon)
	{
		bounds->min.y -= epsilon;
		bounds->max.y += epsilon;
	}
}

void	pad_triangle_aabb_z(t_aabb *bounds, double epsilon)
{
	if (bounds->max.z - bounds->min.z < epsilon)
	{
		bounds->min.z -= epsilon;
		bounds->max.z += epsilon;
	}
}

void	pad_triangle_aabb(t_aabb *bounds)
{
	double	epsilon;

	epsilon = 0.0001;
	pad_triangle_aabb_x(bounds, epsilon);
	pad_triangle_aabb_y(bounds, epsilon);
	pad_triangle_aabb_z(bounds, epsilon);
}
