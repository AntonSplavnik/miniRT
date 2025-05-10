/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 16:08:35 by abillote          #+#    #+#             */
/*   Updated: 2025/05/10 17:12:31 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "platform.h"
#include <fcntl.h>

void parse_error(char *message)
{
	write_string_to_file_descriptor("Error\n", STDERR_FILENO);
	write_string_to_file_descriptor(message, STDERR_FILENO);
	write_string_to_file_descriptor("\n", STDERR_FILENO);
	exit(EXIT_FAILURE);
}

static int	process_buffer_segment(char *buffer, int *i, int bytes_read)
{
	int	line_end;

	line_end = *i;
	while (line_end < bytes_read && buffer[line_end] != '\n')
		line_end++;
	return (line_end);
}

static int	copy_to_line(char *buffer, int *pos, char *line)
{
	int	start;
	int	end;

	start = pos[0];
	end = pos[1];
	if (pos[2] + (end - start) >= MAX_LINE_LEN)
		parse_error("Line too long in scene file");
	while (start < end)
	{
		line[pos[2]] = buffer[start];
		pos[2]++;
		start++;
	}
	line[pos[2]] = '\0';
	return (1);
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
			copy_to_line(buffer, pos, line);
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
int	open_scene_file(char *filename)
{
	int	fd;

	if (!is_valid_filename(filename))
	{
		parse_error("Invalid filename");
		return (-1);
	}
	fd = open(filename, O_RDONLY);
	if (fd < 0)
		parse_error("Failed to open file");
	return (fd);
}

/*
*	Parse the scene file and return a t_scene struct
*	Handle the case where the last line of the file does not end with a newline
*	Return 1 on success, 0 on failure
*/
int	parse_scene_file(t_scene *scene, char *filename)
{
	int			fd;
	int			bytes_read;
	static char	line[MAX_LINE_LEN + 1];
	int			line_pos;

	fd = open_scene_file(filename);
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
	return (1);
}
