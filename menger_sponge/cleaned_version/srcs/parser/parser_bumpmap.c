/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_bumpmap.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 14:06:23 by abillote          #+#    #+#             */
/*   Updated: 2025/06/03 13:42:01 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

// Helper function to read all remaining data from file and tokenize it
char **read_and_tokenize_ppm_data(int fd)
{
	char *buffer;
	char *temp_line;
	char **tokens;
	size_t buffer_size = 0;
	size_t buffer_capacity = 4096;

	// Allocate initial buffer
	buffer = malloc(buffer_capacity);
	if (!buffer)
		return (NULL);
	buffer[0] = '\0';

	// Read all remaining lines and concatenate them
	while ((temp_line = get_next_line(fd)) != NULL)
	{
		size_t line_len = ft_strlen(temp_line);

		// Ensure buffer has enough space
		while (buffer_size + line_len + 2 > buffer_capacity)
		{
			buffer_capacity *= 2;
			char *new_buffer = realloc(buffer, buffer_capacity);
			if (!new_buffer)
			{
				free(buffer);
				free(temp_line);
				return (NULL);
			}
			buffer = new_buffer;
		}

		// Append line to buffer with space separator
		if (buffer_size > 0)
		{
			buffer[buffer_size] = ' ';
			buffer_size++;
		}
		ft_strlcpy(buffer + buffer_size, temp_line, line_len + 1);
		buffer_size += line_len;

		free(temp_line);
	}

	// Tokenize the entire buffer by spaces/whitespace
	tokens = ft_split(buffer, ' ');
	free(buffer);

	return (tokens);
}

// Helper function to count tokens in array
int count_tokens(char **tokens)
{
	int count = 0;

	if (!tokens)
		return (0);

	while (tokens[count])
		count++;

	return (count);
}

t_bump_map	*create_bump_map(const char *filename)
{
	t_bump_map	*bump_map;
	int			fd;
	char		*line;
	int			width;
	int			height;
	int			inline_position;
	char		**rgb_tokens;
	int			token_count;
	int			expected_values;

	bump_map = malloc(sizeof(t_bump_map));
	if (!bump_map)
		return (NULL);
	bump_map->filename = ft_strdup(filename);
	if (!bump_map->filename)
	{
		free_bump_map(bump_map);
		return (NULL);
	}
	fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		printf("Error: Could not open file %s\n", filename);
		free_bump_map(bump_map);
		return (NULL);
	}

	// Read magic number
	line = get_next_line(fd);
	if (!line || ft_strncmp(line, "P3", 2) != 0)
	{
		printf("Error: Invalid PPM file format in %s\n", filename);
		free(line);
		free_bump_map(bump_map);
		close(fd);
		return (NULL);
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
		printf("Error: Could not read width and height from %s\n", filename);
		free_bump_map(bump_map);
		close(fd);
		return (NULL);
	}
	width = ft_atoi(line);
	inline_position = ft_strchr(line, ' ') - line + 1;
	height = ft_atoi(line + inline_position);
	free(line);

	if (width <= 0 || height <= 0 || width > 4096 || height > 4096)
	{
		printf("Error: Invalid width or height in %s\n", filename);
		free_bump_map(bump_map);
		close(fd);
		return (NULL);
	}
	bump_map->width = width;
	bump_map->height = height;

	// Allocate height data
	bump_map->height_data = malloc(width * height * sizeof(float));
	if (!bump_map->height_data)
	{
		printf("Error: Could not allocate memory for height data in %s\n", filename);
		free_bump_map(bump_map);
		close(fd);
		return (NULL);
	}

	// Read max color value
	line = get_next_line(fd);
	if(!line || ft_strncmp(line, "255", 3) != 0)
	{
		printf("Error: Invalid max color value in %s\n", filename);
		if (line)
			free(line);
		free_bump_map(bump_map);
		close(fd);
		return (NULL);
	}
	free(line);

	// Read and tokenize all RGB data
	expected_values = width * height * 3; // 3 values per pixel
	rgb_tokens = read_and_tokenize_ppm_data(fd);
	close(fd);

	if (!rgb_tokens)
	{
		printf("Error: Could not read RGB data from %s\n", filename);
		free_bump_map(bump_map);
		return (NULL);
	}

	token_count = count_tokens(rgb_tokens);
	if (token_count != expected_values)
	{
		printf("Error: Expected %d RGB values, got %d in %s\n",
			expected_values, token_count, filename);
		free_split(rgb_tokens);
		free_bump_map(bump_map);
		return (NULL);
	}

	// Process RGB values
	int i = 0;
	int token_idx = 0;
	while (i < width * height && token_idx + 2 < token_count)
	{
		int r = ft_atoi(rgb_tokens[token_idx]);
		int g = ft_atoi(rgb_tokens[token_idx + 1]);
		int b = ft_atoi(rgb_tokens[token_idx + 2]);

		// Validate RGB values
		if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
		{
			printf("Error: Invalid RGB values (%d, %d, %d) at pixel %d in %s\n",
				r, g, b, i, filename);
			free_split(rgb_tokens);
			free_bump_map(bump_map);
			return (NULL);
		}

		// Convert RGB to grayscale height value
		float normalized_value = (r + g + b) / 3.0f / 255.0f; // Should be [0.0, 1.0]
		bump_map->height_data[i] = (normalized_value - 0.5f) * 0.1f; // Convert to [-0.05, 0.05]

		i++;
		token_idx += 3;
	}

	// Clean up tokens
	free_split(rgb_tokens);

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
