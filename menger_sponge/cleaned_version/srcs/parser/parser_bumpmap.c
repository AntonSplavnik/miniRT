/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_bumpmap.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 14:06:23 by abillote          #+#    #+#             */
/*   Updated: 2025/06/03 09:44:26 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

t_bump_map	*create_bump_map(const char *filename)
{
	t_bump_map	*bump_map;
	int			fd;
	char		*line;
	int			width;
	int			height;
	int			inline_position;

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
	line = get_next_line(fd);
	while (line && line[0] == '#')
	{
		free(line);
		line = get_next_line(fd);
	}
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
	bump_map->height_data = malloc(width * height * sizeof(float));
	if (!bump_map->height_data)
	{
		printf("Error: Could not allocate memory for height data in %s\n", filename);
		free_bump_map(bump_map);
		close(fd);
		return (NULL);
	}
	line = get_next_line(fd);
	if(ft_strncmp(line, "255", 3) != 0)
	{
		printf("Error: Invalid max color value in %s\n", filename);
		free_bump_map(bump_map);
		close(fd);
		return (NULL);
	}
	free(line);
	// Read pixel data & convert to height values
	int i = 0;
	while(i < width * height)
	{
		char *r_line, *g_line, *b_line;
		int r, g, b;

		// Read R value
		r_line = get_next_line(fd);
		if (!r_line)
		{
			printf("Error: Unexpected end of file while reading pixel data in %s\n", filename);
			free_bump_map(bump_map);
			close(fd);
			return (NULL);
		}
		r = ft_atoi(r_line);
		free(r_line);

		// Read G value
		g_line = get_next_line(fd);
		if (!g_line)
		{
			printf("Error: Unexpected end of file while reading pixel data in %s\n", filename);
			free_bump_map(bump_map);
			close(fd);
			return (NULL);
		}
		g = ft_atoi(g_line);
		free(g_line);

		// Read B value
		b_line = get_next_line(fd);
		if (!b_line)
		{
			printf("Error: Unexpected end of file while reading pixel data in %s\n", filename);
			free_bump_map(bump_map);;
			close(fd);
			return (NULL);
		}
		b = ft_atoi(b_line);
		free(b_line);

		// Validate RGB values
		if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
		{
			printf("Error: Invalid RGB values in %s\n", filename);
			free_bump_map(bump_map);
			close(fd);
			return (NULL);
		}

		// Convert to height value and normalize to [0.0, 1.0]
		bump_map->height_data[i] = (r + g + b) / 3.0f / 255.0f;
		i += 3;
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
