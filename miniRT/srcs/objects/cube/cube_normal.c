/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cube_normal.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/19 13:18:59 by abillote          #+#    #+#             */
/*   Updated: 2025/06/20 10:15:15 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/miniRT.h"

t_vec3	create_cube_normal(t_vec3 abs_center_to_point, t_vec3 center_to_point, \
				double max_component)
{
	t_vec3	normal;

	normal = vec3_create(0, 0, 0);
	if (max_component == abs_center_to_point.x)
	{
		if (center_to_point.x > 0)
			normal.x = 1;
		else
			normal.x = -1;
	}
	else if (max_component == abs_center_to_point.y)
	{
		if (center_to_point.y > 0)
			normal.y = 1;
		else
			normal.y = -1;
	}
	else
	{
		if (center_to_point.z > 0)
			normal.z = 1;
		else
			normal.z = -1;
	}
	return (normal);
}

// Calculate normal at the intersection point
t_vec3	cube_normal_at_point(t_vec3 point, t_cube cube)
{
	t_vec3	center_to_point;
	t_vec3	abs_center_to_point;
	double	max_component;
	t_vec3	normal;

	center_to_point = vec3_subtract(point, cube.center);
	abs_center_to_point.x = fabs(center_to_point.x);
	abs_center_to_point.y = fabs(center_to_point.y);
	abs_center_to_point.z = fabs(center_to_point.z);
	max_component = fmax(abs_center_to_point.x, \
					fmax(abs_center_to_point.y, abs_center_to_point.z));
	normal = create_cube_normal(abs_center_to_point, \
					center_to_point, max_component);
	return (vec3_normalize(normal));
}
