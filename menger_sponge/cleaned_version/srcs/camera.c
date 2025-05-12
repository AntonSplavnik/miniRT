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

// Function to calculate forward/backward movement vector based on camera rotation
t_vec3 get_forward_vector(t_vec3 rotation)
{
	// Create a forward vector (0, 0, 1) and rotate it according to camera rotation
	t_vec3 forward = {0, 0, 1};
	forward = rotate_point(forward, rotation);
	return vec3_normalize(forward);
}

// Function to calculate right/left movement vector based on camera rotation
t_vec3 get_right_vector(t_vec3 rotation)
{
	// Create a right vector (1, 0, 0) and rotate it according to camera rotation
	t_vec3 right = {1, 0, 0};
	right = rotate_point(right, rotation);
	return vec3_normalize(right);
}

// Function to calculate up/down movement vector based on camera rotation
t_vec3 get_up_vector(t_vec3 rotation)
{
	// Create an up vector (0, 1, 0) and rotate it according to camera rotation
	t_vec3 up = {0, 1, 0};
	up = rotate_point(up, rotation);
	return vec3_normalize(up);
}
