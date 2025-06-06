/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cube.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 18:30:00 by abillote          #+#    #+#             */
/*   Updated: 2025/06/06 16:05:18 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

void	parse_cube_data(t_scene *scene, char **parts, \
							t_cube *cube_data, t_color *color)
{
	if (!read_vector(parts[1], &cube_data->center))
	{
		free_split(parts);
		parse_error(scene, "Invalid format for cube center. Expected: x,y,z");
	}
	cube_data->side_length = ft_atof(parts[2]);
	if (!check_range(cube_data->side_length, 0.0, INFINITY))
	{
		free_split(parts);
		parse_error(scene, "Expected positive number for cube side length.");
	}
	if (!read_color(parts[3], color))
	{
		free_split(parts);
		parse_error(scene, "Invalid format for cube color. Expected: r,g,b");
	}
}

int	parse_cube(t_scene *scene, char *line)
{
	char		**parts;
	t_object	*cube;
	t_cube		cube_data;
	t_color		color;
	char		*material_block;

	material_block = extract_material_block(line);
	if (material_block)
		trim_material_block(line);
	parts = ft_split_line(line, ' ');
	if (!parts)
		parse_error(scene, "Failed to split line");
	check_parts_count(scene, parts, 4, "cube");
	parse_cube_data(scene, parts, &cube_data, &color);
	free_split(parts);
	cube = create_cube(cube_data.center, cube_data.side_length, color);
	if (!cube)
		parse_error(scene, "Failed to create cube");
	if (material_block)
		parse_material_properties(scene, material_block, &cube->material);
	add_object(scene, cube);
	return (1);
}
