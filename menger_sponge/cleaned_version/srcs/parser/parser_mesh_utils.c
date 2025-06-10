/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_mesh_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 16:30:19 by abillote          #+#    #+#             */
/*   Updated: 2025/06/10 12:25:44 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

static void	process_line_count(char *line, int *vertex_count, int *face_count)
{
	if (line[0] == 'v' && line[1] == ' ')
		(*vertex_count)++;
	else if (line[0] == 'f' && line[1] == ' ')
		(*face_count)++;
}

void	count_obj_elements(int fd, int *vertex_count, int *face_count)
{
	char	*line;

	*vertex_count = 0;
	*face_count = 0;
	line = get_next_line(fd);
	while (line)
	{
		process_line_count(line, vertex_count, face_count);
		free(line);
		line = get_next_line(fd);
	}
	get_next_line(-1);
}

static int	validate_vertex_parts(char **parts)
{
	if (!parts)
		return (0);
	if (!parts[1])
		return (0);
	if (!parts[2])
		return (0);
	if (!parts[3])
		return (0);
	return (1);
}

void	parse_vertex_line(char *line, t_vec3 *vertex)
{
	char	**parts;

	parts = ft_split_line(line, ' ');
	if (!validate_vertex_parts(parts))
	{
		if (parts)
			free_split(parts);
		return ;
	}
	vertex->x = ft_atof(parts[1]);
	vertex->y = ft_atof(parts[2]);
	vertex->z = ft_atof(parts[3]);
	free_split(parts);
}
