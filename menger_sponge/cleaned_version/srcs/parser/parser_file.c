/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_file.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 18:30:00 by abillote          #+#    #+#             */
/*   Updated: 2025/05/26 10:57:27 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"
#include <fcntl.h>

//Find the end of the current line in the buffer if it exists otherwise return the end of the buffer
static int	process_buffer_segment(char *buffer, int *i, int bytes_read)
{
	int	line_end;

	line_end = *i;
	while (line_end < bytes_read && buffer[line_end] != '\n')
		line_end++;
	return (line_end);
}

//Check if the filename ends with ".rt"
int is_valid_filename(char *filename)
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
	int			bytes_read;
	static char	line[MAX_LINE_LEN + 1];
	char		buffer[MAX_LINE_LEN + 1];
	int			pos[3];

	pos[2] = 0;
	while ((bytes_read = read(fd, buffer, MAX_LINE_LEN)) > 0)
	{
		buffer[bytes_read] = '\0';
		pos[0] = 0;
		while (pos[0] < bytes_read)
		{
			pos[1] = process_buffer_segment(buffer, &pos[0], bytes_read);
			copy_to_line(scene, buffer, pos, line);
			if (pos[1] < bytes_read && buffer[pos[1]] == '\n')
			{
				if (!parse_line(scene, line))
					return (0);
				pos[2] = 0;
			}
			pos[0] = pos[1] + 1;
		}
	}
	return (bytes_read);
}

int	parse_scene_file(char *filename, t_scene *scene)
{
	int			fd;
	int			bytes_read;
	static char	line[MAX_LINE_LEN + 1];
	int			line_pos;

	fd = open_scene_file(scene, filename);
	if (fd < 0)
		return (0);
	bytes_read = read_scene_file(fd, scene);
	line_pos = 0;
	while (line[line_pos])
		line_pos++;
	if (line_pos > 0 && !parse_line(scene, line))
	{
		close(fd);
		return (0);
	}
	if (bytes_read < 0)
	{
		close(fd);
		return (0);
	}
	close(fd);
	if (!scene->camera.has_camera)
		parse_error(scene, "Camera not found");
	return (1);
}
