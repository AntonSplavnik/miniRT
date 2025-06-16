/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_bumpmap.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 14:06:23 by abillote          #+#    #+#             */
/*   Updated: 2025/06/05 11:02:22 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

// Helper function to skip whitespace and find next number
static char *skip_whitespace(char *str)
{
	while (*str && (*str == ' ' || *str == '\t' || *str == '\n' || *str == '\r'))
		str++;
	return (str);
}

// Helper function to parse integer from string and advance pointer
static int parse_next_int(char **str)
{
	int result = 0;
	int sign = 1;

	*str = skip_whitespace(*str);

	if (**str == '-')
	{
		sign = -1;
		(*str)++;
	}
	else if (**str == '+')
		(*str)++;

	while (**str >= '0' && **str <= '9')
	{
		result = result * 10 + (**str - '0');
		(*str)++;
	}

	return (result * sign);
}

// Fast bulk reading function
static char *read_remaining_file(int fd, size_t *size)
{
	char buffer[8192];
	char *result = NULL;
	char *temp;
	size_t total_size = 0;
	ssize_t bytes_read;

	while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0)
	{
		temp = realloc(result, total_size + bytes_read + 1);
		if (!temp)
		{
			free(result);
			return (NULL);
		}
		result = temp;
		ft_memcpy(result + total_size, buffer, bytes_read);
		total_size += bytes_read;
	}

	if (result)
	{
		result[total_size] = '\0';
		*size = total_size;
	}

	return (result);
}

t_bump_map	*create_bump_map(t_scene *scene, const char *filename)
{
	t_bump_map	*bump_map;
	int			fd;
	char		*line;
	int			width, height;
	int			inline_position;
	char		*data_buffer;
	char		*current_pos;
	size_t		buffer_size;
	int			i, total_pixels;

	bump_map = malloc(sizeof(t_bump_map));
	if (!bump_map)
		return (NULL);
	bump_map->filename = ft_strdup(filename);
	if (!bump_map->filename)
	{
		free(bump_map);
		parse_error(scene, "Could not allocate memory for bump map filename");
	}
	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		free(bump_map->filename);
		free(bump_map);
		parse_error(scene, "Could not open bump map file");
	}

	// Read magic number
	line = get_next_line(fd);
	if (!line || ft_strncmp(line, "P3", 2) != 0)
	{
		if (line)
			free(line);
		free(bump_map->filename);
		free(bump_map);
		close(fd);
		parse_error(scene, "Invalid PPM file format");
	}
	free(line);

	// Skip comments
	line = get_next_line(fd);
	while (line && line[0] == '#')
	{
		free(line);
		line = get_next_line(fd);
	}

	// Read width and height
	if (!line)
	{
		close(fd);
		free(bump_map->filename);
		free(bump_map);
		parse_error(scene, "Could not read width and height from PPM file");
	}
	width = ft_atoi(line);
	inline_position = ft_strchr(line, ' ') - line + 1;
	height = ft_atoi(line + inline_position);
	free(line);

	if (width <= 0 || height <= 0 || width > 4096 || height > 4096)
	{
		free(bump_map->filename);
		free(bump_map);
		close(fd);
		parse_error(scene, "Invalid width or height in PPM file");
	}
	bump_map->width = width;
	bump_map->height = height;

	// Allocate height data
	total_pixels = width * height;
	bump_map->height_data = malloc(total_pixels * sizeof(float));
	if (!bump_map->height_data)
	{
		free(bump_map->filename);
		free(bump_map);
		close(fd);
		parse_error(scene, "Could not allocate memory for height data in PPM file");
	}

	// Read max color value
	line = get_next_line(fd);
	if(!line || ft_strncmp(line, "255", 3) != 0)
	{
		if (line)
			free(line);
		free_bump_map(bump_map);
		close(fd);
		parse_error(scene, "Invalid max color value in PPM file");
	}
	free(line);

	// Read remaining file data in bulk
	data_buffer = read_remaining_file(fd, &buffer_size);
	close(fd);

	if (!data_buffer)
	{
		free_bump_map(bump_map);
		parse_error(scene, "Could not read RGB data from PPM file");
	}

	// Process RGB values directly from buffer
	current_pos = data_buffer;
	i = 0;
	while (i < total_pixels && current_pos < data_buffer + buffer_size)
	{
		int r = parse_next_int(&current_pos);
		int g = parse_next_int(&current_pos);
		int b = parse_next_int(&current_pos);

		// Validate RGB values
		if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
		{
			free(data_buffer);
			free_bump_map(bump_map);
			parse_error(scene, "Invalid RGB values in PPM file");
		}

		// Convert RGB to grayscale height value
		float normalized_value = (r + g + b) / (3.0f * 255.0f);
		// Scale to [-0.05, 0.05] range because bump maps typically use small height variations
		bump_map->height_data[i] = (normalized_value - 0.5f) * 0.1f;

		i++;
	}

	free(data_buffer);

	if (i != total_pixels)
	{
		free_bump_map(bump_map);
		parse_error(scene, "Invalid number of RGB values in PPM file");
	}

	return (bump_map);
}

// Free bump map memory
void free_bump_map(t_bump_map *bump_map)
{
	if (bump_map)
	{
		if (bump_map->height_data)
			free(bump_map->height_data);
		if (bump_map->filename)
			free(bump_map->filename);
		free(bump_map);
	}
}
