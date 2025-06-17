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

// Helper function to parse integer from string and advance pointer
static int	parse_next_int(char **str)
{
	int	result;
	int	sign;

	result = 0;
	sign = 1;
	while (**str && (**str == ' ' || **str == '\t' \
			|| **str == '\n' || **str == '\r'))
		(*str)++;
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

float	calculate_rgb_normalized_value(char **current_pos)
{
	float	normalized_value;
	int		r;
	int		g;
	int		b;

	r = parse_next_int(current_pos);
	g = parse_next_int(current_pos);
	b = parse_next_int(current_pos);
	if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
		return (0.5);
	normalized_value = (r + g + b) / (3.0f * 255.0f);
	return (normalized_value);
}

// Process the buffer of RGB values
int	process_rgb_data(t_bump_map **bump_map, char *data_buffer,
							size_t buffer_size, int total_pixels)
{
	char	*current_pos;
	int		i;
	float	normalized_value;

	current_pos = data_buffer;
	i = 0;
	while (i < total_pixels && current_pos < data_buffer + buffer_size)
	{
		normalized_value = calculate_rgb_normalized_value(&current_pos);
		(*bump_map)->elevation[i] = (normalized_value - 0.5f) * 0.1f;
		i++;
	}
	return (i == total_pixels);
}

void	allocate_elevation(t_scene *scene, t_bump_map **bump_map, int fd)
{
	char	*data_buffer;
	size_t	buffer_size;
	int		total_pixels;

	total_pixels = (*bump_map)->width * (*bump_map)->height;
	if (!allocate_elevation_memory(*bump_map, fd, total_pixels))
	{
		free_bump_map(*bump_map);
		parse_error(scene, "Failed to allocate memory for elevation data");
	}
	data_buffer = read_remaining_file(fd, &buffer_size);
	close(fd);
	if (!data_buffer)
	{
		free_bump_map(*bump_map);
		parse_error(scene, "Could not read RGB data from PPM file");
	}
	if (!process_rgb_data(bump_map, data_buffer, buffer_size, total_pixels))
	{
		free(data_buffer);
		free_bump_map(*bump_map);
		parse_error(scene, "Invalid RGB values or count in PPM file");
	}
	free(data_buffer);
}

t_bump_map	*create_bump_map(t_scene *scene, const char *filename)
{
	t_bump_map	*bump_map;

	bump_map = malloc(sizeof(t_bump_map));
	if (!bump_map)
		return (NULL);
	*bump_map = (t_bump_map){.filename = NULL, .elevation = NULL, \
						.width = 0, .height = 0};
	bump_map->filename = ft_strdup(filename);
	if (!bump_map->filename)
	{
		free(bump_map);
		parse_error(scene, "Could not allocate memory for bump map filename");
	}
	load_bump_map_file(scene, &bump_map, filename);
	return (bump_map);
}
