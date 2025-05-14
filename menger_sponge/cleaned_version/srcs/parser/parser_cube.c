/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cube.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 18:30:00 by abillote          #+#    #+#             */
/*   Updated: 2025/05/14 21:44:25 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "platform.h"

void	parse_cube_data(t_scene *scene, char **parts, t_vec3 *center, double *side_length, t_color *color)
{
	if (!read_vector(parts[1], center))
	{
		free_split(parts);
		parse_error(scene, "Invalid format for cube center. Expected: x,y,z");
	}
	*side_length = ft_atof(parts[2]);
	if (!check_range(*side_length, 0.0, INFINITY))
	{
		free_split(parts);
		parse_error(scene, "Invalid format for cube side length. Expected: positive number");
	}
	if (!read_color(parts[3], color))
	{
		free_split(parts);
		parse_error(scene, "Invalid format for cube color. Expected: r,g,b");
	}
}

int	parse_cube(t_scene *scene, char *line)
{
	char	**parts;
	t_object	*cube;
	t_vec3		center;
	double		side_length;
	t_color		color;

	parts = ft_split(line, ' ');
	if (!parts)
		parse_error(scene, "Failed to split line");
	check_parts_count(scene, parts, 5, "cube");
	parse_cube_data(scene, parts, &center, &side_length, &color);
	free_split(parts);
	cube = create_cube(center, side_length, color);
	if (!cube)
		parse_error(scene, "Failed to create cube");
	add_object(scene, cube);
	return (1);
}
