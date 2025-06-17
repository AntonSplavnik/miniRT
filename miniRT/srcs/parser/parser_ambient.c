/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_ambient.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 18:30:00 by abillote          #+#    #+#             */
/*   Updated: 2025/06/11 10:16:42 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

int	read_ambient_ratio(t_scene *scene, char **str, double *ratio)
{
	if (!str)
		return (0);
	*ratio = ft_atof(str[1]);
	if (!check_range(*ratio, 0.0, 1.0))
	{
		free_split(str);
		parse_error(scene, "Ambient ratio must be between 0.0 and 1.0");
		return (0);
	}
	return (1);
}

int	parse_ambient(t_scene *scene, char *line)
{
	char	**parts;
	int		parts_count;
	int		parse_color;

	if (scene->ambient.has_ambient)
		parse_error(scene, "Ambient lighting can only be declared once");
	parts = ft_split_line(line, ' ');
	if (!parts)
		parse_error(scene, "Failed to split line");
	parts_count = 0;
	while (parts[parts_count])
		parts_count++;
	if (parts_count != 3)
	{
		free_split(parts);
		parse_error(scene, "Expected for ambient lighting: A ratio r,g,b");
	}
	read_ambient_ratio(scene, parts, &scene->ambient.ratio);
	parse_color = read_color(parts[2], &scene->ambient.color);
	free_split(parts);
	if (!parse_color)
		parse_error(scene, "Expected color format for ambient lighting: r,g,b");
	scene->ambient.has_ambient = 1;
	return (1);
}
