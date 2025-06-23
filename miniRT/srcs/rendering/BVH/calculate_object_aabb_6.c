/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calculate_object_aabb_6.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 00:00:00 by antonsplavn      #+#    #+#             */
/*   Updated: 2025/06/23 00:00:00 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"
#include "../../includes/bvh.h"

t_aabb	calculate_cylinder_aabb_helper(t_vec3 top, t_vec3 bottom, double radius)
{
	t_aabb	bounds;

	bounds.min.x = fmin(top.x - radius, bottom.x - radius);
	bounds.min.y = fmin(top.y - radius, bottom.y - radius);
	bounds.min.z = fmin(top.z - radius, bottom.z - radius);
	bounds.max.x = fmax(top.x + radius, bottom.x + radius);
	bounds.max.y = fmax(top.y + radius, bottom.y + radius);
	bounds.max.z = fmax(top.z + radius, bottom.z + radius);
	return (bounds);
}
