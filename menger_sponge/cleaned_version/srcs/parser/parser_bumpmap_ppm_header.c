/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_bumpmap_ppm_header.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 17:26:31 by abillote          #+#    #+#             */
/*   Updated: 2025/06/11 17:27:10 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

void	read_width_and_height(t_scene *scene, char *line, \
								t_bump_map **bump_map, int fd)
{
	int	width;
	int	height;
	int	inline_position;

	if (!line)
	{
		close(fd);
		free_bump_map(*bump_map);
		parse_error(scene, "Could not read width and height from PPM file");
	}
	width = ft_atoi(line);
	inline_position = ft_strchr(line, ' ') - line + 1;
	height = ft_atoi(line + inline_position);
	free(line);
	if (width <= 0 || height <= 0 || width > 4096 || height > 4096)
	{
		free_bump_map(*bump_map);
		close(fd);
		parse_error(scene, "Invalid width or height in PPM file");
	}
	(*bump_map)->width = width;
	(*bump_map)->height = height;
}

void	validate_max_color_value(t_scene *scene, char *line, \
								t_bump_map **bump_map, int fd)
{
	line = get_next_line(fd);
	if (!line || ft_strncmp(line, "255", 3) != 0)
	{
		if (line)
			free(line);
		free_bump_map(*bump_map);
		close(fd);
		parse_error(scene, "Invalid max color value in PPM file");
	}
	free(line);
}

// Parse the PPM header and extract width and height
// Validate the format (P3), skip comments, and read dimensions
void	parse_ppm_header(t_scene *scene, t_bump_map **bump_map, int fd)
{
	char	*line;

	line = get_next_line(fd);
	if (!line || ft_strncmp(line, "P3", 2) != 0)
	{
		if (line)
			free(line);
		free_bump_map(*bump_map);
		close(fd);
		parse_error(scene, "Invalid PPM file format");
	}
	free(line);
	line = get_next_line(fd);
	while (line && line[0] == '#')
	{
		free(line);
		line = get_next_line(fd);
	}
	read_width_and_height(scene, line, bump_map, fd);
	validate_max_color_value(scene, line, bump_map, fd);
}

// Allocate memory for the elevation data
int	allocate_elevation_memory(t_bump_map *bump_map, \
										int fd, int total_pixels)
{
	bump_map->elevation = malloc(total_pixels * sizeof(float));
	if (!bump_map->elevation)
	{
		close(fd);
		return (0);
	}
	return (1);
}
