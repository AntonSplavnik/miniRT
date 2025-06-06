/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_light.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 18:30:00 by abillote          #+#    #+#             */
/*   Updated: 2025/06/06 14:38:10 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

t_light	*parse_light_properties(t_scene *scene, char **parts)
{
	t_vec3	position;
	double	intensity;
	t_color	color;
	int		parse_color_result;
	t_light	*new_light;

	if (!read_vector(parts[1], &position))
	{
		free_split(parts);
		parse_error(scene, "Expected for light position: x,y,z");
	}
	intensity = ft_atof(parts[2]);
	if (!check_range(intensity, 0.0, 1.0))
	{
		free_split(parts);
		parse_error(scene, "Light ratio must be between 0.0 and 1.0");
	}
	parse_color_result = read_color(parts[3], &color);
	free_split(parts);
	if (!parse_color_result)
		parse_error(scene, "Invalid format for light color. Expected: r,g,b");
	new_light = create_light(position, intensity, color);
	if (!new_light)
		parse_error(scene, "Failed to create light");
	return (new_light);
}

int	parse_light(t_scene *scene, char *line)
{
	char	**parts;
	t_light	*new_light;

	parts = ft_split_line(line, ' ');
	if (!parts)
		parse_error(scene, "Failed to split line");
	check_parts_count(scene, parts, 4, "light");
	new_light = parse_light_properties(scene, parts);
	add_light(scene, new_light);
	return (1);
}
