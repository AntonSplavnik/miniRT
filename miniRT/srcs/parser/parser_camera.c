/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_camera.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 18:30:00 by abillote          #+#    #+#             */
/*   Updated: 2025/06/11 17:36:39 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

/*
** Extracts position, direction vector, and field of view from the input.
** Converts the direction vector to rotation angles (Euler angles)
for camera orientation: pitch (x), yaw (y), and roll (z).
** Format: parts[1]=position, parts[2]=direction, parts[3]=fov
*/

static void	parse_camera_properties(t_scene *scene, char **parts)
{
	int		parse_result;
	t_vec3	direction;

	direction = vec3_create(0.0, 0.0, 0.0);
	if (!read_vector(parts[1], &scene->camera.position))
	{
		free_split(parts);
		parse_error(scene, "Invalid format for camera position. \
Expected: x,y,z");
	}
	parse_result = read_vector(parts[2], &direction);
	if (!parse_result || !check_vector_normalization(direction))
	{
		free_split(parts);
		parse_error(scene, "Invalid format for camera direction. \
Expected: nx,ny,nz");
	}
	scene->camera.rotation.y = atan2(direction.x, direction.z);
	scene->camera.rotation.x = -atan2(direction.y, sqrt(direction.x * \
direction.x + direction.z * direction.z));
	scene->camera.rotation.z = 0.0;
	update_camera_vectors(scene);
	scene->camera.fov = ft_atof(parts[3]);
	if (!check_range(scene->camera.fov, 0.0, 180.0))
		parse_error(scene, "Camera FOV must be between 0.0 and 180.0");
}

int	parse_camera(t_scene *scene, char *line)
{
	char	**parts;

	if (scene->camera.has_camera)
		parse_error(scene, "Camera can only be declared once");
	parts = ft_split_line(line, ' ');
	if (!parts)
		parse_error(scene, "Failed to split line");
	check_parts_count(scene, parts, 4, "camera");
	parse_camera_properties(scene, parts);
	free_split(parts);
	scene->camera.has_camera = 1;
	return (1);
}
