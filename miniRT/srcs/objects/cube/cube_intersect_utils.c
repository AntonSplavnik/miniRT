/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cube_intersect_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 13:00:00 by abillote          #+#    #+#             */
/*   Updated: 2025/06/20 10:16:02 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/miniRT.h"

t_slab	calculate_slab_data(t_ray ray, t_vec3 min_bound, t_vec3 max_bound)
{
	t_slab	slab;

	slab.slab_x_dir = ray.direction.x;
	slab.slab_x_origin = ray.origin.x;
	slab.slab_x_max = max_bound.x;
	slab.slab_x_min = min_bound.x;
	slab.slab_y_dir = ray.direction.y;
	slab.slab_y_origin = ray.origin.y;
	slab.slab_y_max = max_bound.y;
	slab.slab_y_min = min_bound.y;
	slab.slab_z_dir = ray.direction.z;
	slab.slab_z_origin = ray.origin.z;
	slab.slab_z_max = max_bound.z;
	slab.slab_z_min = min_bound.z;
	return (slab);
}
