/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_file.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 18:30:00 by abillote          #+#    #+#             */
/*   Updated: 2025/06/11 10:28:27 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"
#include "../../libft/libft.h"
#include <fcntl.h>

//Check if the filename ends with ".rt"
int	is_valid_filename(char *filename)
{
	if (!filename)
		return (0);
	if (ft_strlen(filename) < 3)
		return (0);
	if (ft_strcmp(filename + ft_strlen(filename) - 3, ".rt") != 0)
		return (0);
	return (1);
}

// Parse file part 1: Open file and prepare for reading
int	open_scene_file(t_scene *scene, char *filename)
{
	int	fd;

	if (!is_valid_filename(filename))
	{
		parse_error(scene, "Invalid filename");
		return (-1);
	}
	fd = open(filename, O_RDONLY);
	if (fd < 0)
		parse_error(scene, "Failed to open file");
	return (fd);
}

int	read_scene_file(int fd, t_scene *scene)
{
	char	*line;

	line = get_next_line(fd);
	while (line)
	{
		if (!parse_line(scene, line))
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

int	parse_scene_file(char *filename, t_scene *scene)
{
	int	fd;
	int	success;

	fd = open_scene_file(scene, filename);
	success = read_scene_file(fd, scene);
	close(fd);
	if (!success)
		return (1);
	if (!scene->camera.has_camera)
		parse_error(scene, "Camera not found");
	if (!scene->ambient.has_ambient)
		parse_error(scene, "Ambient light not found");
	return (0);
}
