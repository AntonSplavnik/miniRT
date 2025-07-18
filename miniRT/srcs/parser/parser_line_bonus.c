/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_line_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 00:00:00 by abillote          #+#    #+#             */
/*   Updated: 2025/07/18 15:55:39 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

int	parse_parameters_bonus(t_scene *scene, char *line)
{
	if (is_ambient(line))
		return (parse_ambient(scene, line));
	else if (is_camera(line))
		return (parse_camera(scene, line));
	else if (is_light(line))
		return (parse_light_bonus(scene, line));
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
static char	*remove_inline_comments(char *line)
{
	char	*comment_start;

	comment_start = ft_strchr(line, '#');
	if (comment_start)
		*comment_start = '\0';
	return (line);
}

int	parse_line_bonus(t_scene *scene, char *line)
{
	char	*trimmed;
	int		result;

	remove_inline_comments(line);
	trimmed = ft_strtrim_whitespace(line);
	if (!trimmed)
		return (0);
	if (trimmed[0] == '\0')
	{
		free(trimmed);
		return (1);
	}
	result = parse_parameters_bonus(scene, trimmed);
	if (result == 0)
	{
		free(trimmed);
		parse_error(scene, "Unknown element identifier");
		return (0);
	}
	free(trimmed);
	return (result);
}
