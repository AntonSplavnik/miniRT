/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_line.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 18:30:00 by abillote          #+#    #+#             */
/*   Updated: 2025/06/05 12:18:56 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"


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
	else if (is_mesh(line))
		return (parse_mesh(scene, line));
	else
		return (0);
}

// Remove inline comments from a line
static char *remove_inline_comments(char *line)
{
	char *comment_start;

	comment_start = ft_strchr(line, '#');
	if (comment_start)
		*comment_start = '\0';
	return (line);
}

int	parse_line(t_scene *scene, char *line)
{
	char	*trimmed;
	char	*final_trimmed;
	int		result;

	trimmed = ft_strtrim_whitespace(line);
	if (!trimmed)
		return (0);
	if (trimmed[0] == '\0')
	{
		free(trimmed);
		return (1);
	}
	// Remove inline comments before parsing
	remove_inline_comments(trimmed);
	final_trimmed = ft_strtrim_whitespace(trimmed); // Trim again after removing comments
	free(trimmed);
	if (!final_trimmed)
		return (0);
	if (final_trimmed[0] == '\0')
	{
		free(final_trimmed);
		return (1);
	}
	result = parse_parameters(scene, final_trimmed);
	if (result == 0)
	{
		free(final_trimmed);
		parse_error(scene, "Unknown element identifier");
		return (0);
	}
	free(final_trimmed);
	return (result);
}
