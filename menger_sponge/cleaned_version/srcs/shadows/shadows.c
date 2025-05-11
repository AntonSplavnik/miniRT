/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shadows.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 10:05:53 by abillote          #+#    #+#             */
/*   Updated: 2025/05/05 10:29:53 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "platform.h"

int	is_in_shadow(t_scene *scene, t_vec3 hit_point, t_vec3 light_dir, double light_distance)
{
	t_ray		shadow_ray;
	t_object	*hit_object;
	double		t;

	//Create a shadow ray from the hit point towards the light
	shadow_ray.origin = vec3_add(hit_point, vec3_scale(light_dir, 0.001)); //offset to avoid self intersection
	shadow_ray.direction = light_dir;

	if (find_closest_intersection(scene, shadow_ray, &t, &hit_object) && t < light_distance)
		return (1);
	return (0);
}
