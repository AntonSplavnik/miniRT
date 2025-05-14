/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_line.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 18:30:00 by abillote          #+#    #+#             */
/*   Updated: 2025/05/14 21:42:38 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "platform.h"

//Copy the current line from the buffer to the line array
int	copy_to_line(t_scene *scene, char *buffer, int *pos, char *line)
{
	int	start;
	int	end;

	start = pos[0];
	end = pos[1];
	if (pos[2] + (end - start) >= MAX_LINE_LEN)
		parse_error(scene, "Line too long in scene file");
	while (start < end)
	{
		line[pos[2]] = buffer[start];
		pos[2]++;
		start++;
	}
	line[pos[2]] = '\0';
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
		parse_error(scene, "Unknown element identifier");
		return (0);
	}
	free(trimmed);
	return (result);
}
