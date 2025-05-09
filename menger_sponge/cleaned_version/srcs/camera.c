/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 10:14:49 by abillote          #+#    #+#             */
/*   Updated: 2025/05/09 10:15:05 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "platform.h"

t_vec3	rotate_point(t_vec3 point, t_vec3 rotation)
{
	t_vec3	result;
	double	cos_x, sin_x, cos_y, sin_y, cos_z, sin_z;
	t_vec3  temp;

	cos_x = cos(rotation.x);
	sin_x = sin(rotation.x);
	cos_y = cos(rotation.y);
	sin_y = sin(rotation.y);
	cos_z = cos(rotation.z);
	sin_z = sin(rotation.z);

	// Rotate around X axis
	temp.x = point.x;
	temp.y = point.y * cos_x - point.z * sin_x;
	temp.z = point.y * sin_x + point.z * cos_x;

	// Rotate around Y axis
	result.x = temp.x * cos_y + temp.z * sin_y;
	result.y = temp.y;
	result.z = -temp.x * sin_y + temp.z * cos_y;

	// Rotate around Z axis (using result as input)
	temp = result;
	result.x = temp.x * cos_z - temp.y * sin_z;
	result.y = temp.x * sin_z + temp.y * cos_z;
	result.z = temp.z;

	return (result);
}
