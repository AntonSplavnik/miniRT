/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_bumpmap_ppm_file.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 17:27:52 by abillote          #+#    #+#             */
/*   Updated: 2025/06/11 17:36:21 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

void	load_bump_map_file(t_scene *scene, t_bump_map **bump_map, \
							const char *filename)
{
	int		fd;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		free_bump_map(*bump_map);
		parse_error(scene, "Could not open bump map file");
	}
	parse_ppm_header(scene, bump_map, fd);
	allocate_elevation(scene, bump_map, fd);
}

int	append_buffer_to_result(char **result, char *buffer,
						size_t *total_size, ssize_t bytes_read)
{
	char	*temp;

	temp = realloc(*result, *total_size + bytes_read + 1);
	if (!temp)
	{
		free(*result);
		*result = NULL;
		return (0);
	}
	*result = temp;
	ft_memcpy(*result + *total_size, buffer, bytes_read);
	*total_size += bytes_read;
	return (1);
}

// Read the remaining content of the file after the header
// and return it as a string
char	*read_remaining_file(int fd, size_t *size)
{
	char	buffer[8192];
	char	*result;
	size_t	total_size;
	ssize_t	bytes_read;

	result = NULL;
	total_size = 0;
	bytes_read = read(fd, buffer, sizeof(buffer));
	while (bytes_read > 0)
	{
		if (!append_buffer_to_result(&result, buffer, &total_size, bytes_read))
			return (NULL);
		bytes_read = read(fd, buffer, sizeof(buffer));
	}
	if (result)
	{
		result[total_size] = '\0';
		*size = total_size;
	}
	return (result);
}

// Free bump map memory
void	free_bump_map(t_bump_map *bump_map)
{
	if (bump_map)
	{
		if (bump_map->elevation)
			free(bump_map->elevation);
		if (bump_map->filename)
			free(bump_map->filename);
		free(bump_map);
	}
}
