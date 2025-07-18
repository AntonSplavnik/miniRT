/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_file_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 00:00:00 by abillote          #+#    #+#             */
/*   Updated: 2025/07/18 15:55:39 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"
#include "../../libft/libft.h"
#include <fcntl.h>

int	read_scene_file_bonus(int fd, t_scene *scene)
{
	char	*line;

	line = get_next_line(fd);
	while (line)
	{
		if (!parse_line_bonus(scene, line))
		{
			free(line);
			get_next_line(-1);
			return (0);
		}
		free(line);
		line = get_next_line(fd);
	}
	get_next_line(-1);
	return (1);
}

int	parse_scene_file_bonus(char *filename, t_scene *scene)
{
	int	fd;
	int	success;

	fd = open_scene_file(scene, filename);
	success = read_scene_file_bonus(fd, scene);
	close(fd);
	if (!success)
		return (1);
	if (!scene->camera.has_camera)
		parse_error(scene, "Camera not found");
	if (!scene->ambient.has_ambient)
		parse_error(scene, "Ambient light not found");
	return (0);
}
