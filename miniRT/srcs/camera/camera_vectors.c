/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera_vectors.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 22:14:00 by antonsplavn      #+#    #+#             */
/*   Updated: 2025/07/13 22:14:00 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

/*
** Calculates forward/backward movement vector based on camera rotation
*/
t_vec3	get_forward_vector(t_vec3 rotation)
{
	t_vec3	forward;

	forward.x = 0;
	forward.y = 0;
	forward.z = 1;
	forward = rotate_point(forward, rotation);
	return (vec3_normalize(forward));
}

/*
** Calculates right/left movement vector based on camera rotation
*/
t_vec3	get_right_vector(t_vec3 rotation)
{
	t_vec3	right;

	right.x = 1;
	right.y = 0;
	right.z = 0;
	right = rotate_point(right, rotation);
	return (vec3_normalize(right));
}

/*
** Calculates up/down movement vector based on camera rotation
*/
t_vec3	get_up_vector(t_vec3 rotation)
{
	t_vec3	up;

	up.x = 0;
	up.y = 1;
	up.z = 0;
	up = rotate_point(up, rotation);
	return (vec3_normalize(up));
}

/*
** Updates all camera vectors based on current rotation
*/
void	update_camera_vectors(t_scene *scene)
{
	scene->camera.forwards = get_forward_vector(scene->camera.rotation);
	scene->camera.right = get_right_vector(scene->camera.rotation);
	scene->camera.up = get_up_vector(scene->camera.rotation);
}
