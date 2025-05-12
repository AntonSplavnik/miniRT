/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 16:08:35 by abillote          #+#    #+#             */
/*   Updated: 2025/05/12 09:59:29 by abillote         ###   ########.fr       */
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

int	is_ambient(char *line)
{
	if (line[0] = 'A')
	{
		if (line[1] == ' ' || line[1] == '\t' || line[1] == '\0')
			return (1);
	}
	return (0);
}

int	is_camera(char *line)
{
	if (line[0] = 'C')
	{
		if (line[1] == ' ' || line[1] == '\t' || line[1] == '\0')
			return (1);
	}
	return (0);
}

int	is_light(char *line)
{
	if (line[0] = 'L')
	{
		if (line[1] == ' ' || line[1] == '\t' || line[1] == '\0')
			return (1);
	}
	return (0);
}

int	is_sphere(char *line)
{
	if (ft_strncmp(line, 'sp', 2) == 0)
	{
		if (line[2] == ' ' || line[2] == '\t' || line[2] == '\0')
			return (1);
	}
	return (0);
}

int	is_plane(char *line)
{
	if (ft_strncmp(line, 'pl', 2) == 0)
	{
		if (line[2] == ' ' || line[2] == '\t' || line[2] == '\0')
			return (1);
	}
	return (0);
}

int	is_cylinder(char *line)
{
	if (ft_strncmp(line, 'cy', 2) == 0)
	{
		if (line[2] == ' ' || line[2] == '\t' || line[2] == '\0')
			return (1);
	}
	return (0);
}

int	is_cone(char *line)
{
	if (ft_strncmp(line, 'co', 2) == 0)
	{
		if (line[2] == ' ' || line[2] == '\t' || line[2] == '\0')
			return (1);
	}
	return (0);
}

int	is_triangle(char *line)
{
	if (ft_strncmp(line, 'tr', 2) == 0)
	{
		if (line[2] == ' ' || line[2] == '\t' || line[2] == '\0')
			return (1);
	}
	return (0);
}

int	is_cube(char *line)
{
	if (ft_strncmp(line, 'cu', 2) == 0)
	{
		if (line[2] == ' ' || line[2] == '\t' || line[2] == '\0')
			return (1);
	}
	return (0);
}

int	check_ambient_uniqueness(t_scene *scene)
{
	if (scene->ambient.ratio != 0.1)
		return (0);
	if (scene->ambient.color.r != 255 || scene->ambient.color.g != 255 || scene->ambient.color.b != 255)
		return (0);
	return (1);
}

int	check_range(double value, double min, double max)
{
	if (value < min || value > max)
		return (0);
	return (1);
}

int	read_ambient_ratio(char **str, double *ratio)
{
	if (!str)
		return (0);
	*ratio = ft_atof(str[1]);
	if (!check_range(*ratio, 0.0, 1.0))
	{
		free_split(str);
		parse_error("Ambient ratio must be between 0.0 and 1.0");
		return (0);
	}
	return (1);
}

void	free_split(char **str)
{
	int	i;

	i = 0;
	while (str[i])
		free(str[i++]);
	free(str);
}

int	parse_rgb(char **parts, int *r, int *g, int *b)
{
	*r = ft_atoi(parts[0]);
	*g = ft_atoi(parts[1]);
	*b = ft_atoi(parts[2]);
	if (!check_range(*r, 0, 255) || !check_range(*g, 0, 255) || !check_range(*b, 0, 255))
		return (0);
	return (1);
}

int	read_color(char *str, t_color *color)
{
	char	**parts;
	int		parts_count;
	int		r;
	int		b;
	int		g;
	int		parse_result;

	parts = ft_split(str, ",");
	if (!parts)
		return (0);
	parts_count = 0;
	while (parts[parts_count])
		parts_count++;
	if (parts_count != 3)
	{
		free_split(parts);
		return (0);
	}
	parse_result = parse_rgb(parts, &r, &g, &b);
	free_split(parts);
	if (!parse_result)
		return (0);
	*color = create_color(r, g, b);
	return (1);
}
int	parse_ambient(t_scene *scene, char *line)
{
	char	**parts;
	int		parts_count;
	int		parse_color;

	if (!check_ambient_uniqueness(scene))
		parse_error("Ambient lighting can only be declared once");
	parts = ft_split(line, " ");
	if (!parts)
		parse_error("Failed to split line");
	parts_count = 0;
	while (parts[parts_count])
		parts_count++;
	if (parts_count != 3)
	{
		free_split(parts);
		parse_error("Invalid format for ambient lighting. Expected: A ratio r,g,b");
	}
	read_ambient_ratio(parts, &scene->ambient.ratio);
	parse_color = read_color(parts[2], &scene->ambient.color);
	if (!parse_color)
	{
		free_split(parts);
		parse_error("Invalid color format for ambient lighting. Expected: r,g,b");
	}
	return (1);
}

int	parse_parameters(t_scene *scene, char *line)
{
	if (is_ambient(line))
		return (parse_ambient(scene, line));
	else if (is_camera(line))
		return (parse_camera(scene, line));
	else if (is_light(line))
		return (parse_light(scene, line));
	else if (is_sphere(line))
		return (parse_sphere(scene, line));
	else if (is_plane(line))
		return (parse_plane(scene, line));
	else if (is_cylinder(line))
		return (parse_cylinder(scene, line));
	else if (is_cone(line))
		return (parse_cone(scene, line));
	else if (is_triangle(line))
		return (parse_triangle(scene, line));
	else if (is_cube(line))
		return (parse_cube(scene, line));
	else
		return (0);
}

int	parse_line(t_scene *scene, char *line)
{
	char	*trimmed;
	int		result;

	trimmed = ft_strtrim(line);
	if (!trimmed)
		return (0);
	if (trimmed[0] == '\0' || trimmed[0] == '#')
	{
		free(trimmed);
		return (1);
	}
	result = parse_parameters(scene, trimmed);
	if (result == 0)
	{
		free(trimmed);
		parse_error("Unknown element identifier");
		return (0);
	}
	free(trimmed);
	return (result);
}

//Find the end of the current line in the buffer if it exists otherwise return the end of the buffer
static int	process_buffer_segment(char *buffer, int *i, int bytes_read)
{
	int	line_end;

	line_end = *i;
	while (line_end < bytes_read && buffer[line_end] != '\n')
		line_end++;
	return (line_end);
}

//Copy the current line from the buffer to the line array
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

//Using an array of positions to reduce the number of parameters
//pos[0] is the current position in the buffer
//pos[1] is the end of the current line
//pos[2] is the current position in the line
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
