/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_sphere.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 18:30:00 by abillote          #+#    #+#             */
/*   Updated: 2025/06/06 15:03:19 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

int	parse_sphere_data(t_scene *scene, char **parts, \
						t_sphere *sphere, t_color *color)
{
	if (!read_vector(parts[1], &sphere->center))
	{
		free_split(parts);
		parse_error(scene, "Expected for sphere position: x,y,z");
	}
	sphere->diameter = ft_atof(parts[2]);
	if (!check_range(sphere->diameter, 0.0, INFINITY))
	{
		free_split(parts);
		parse_error(scene, "Sphere diameter must be a positive number");
	}
	if (!read_color(parts[3], color))
	{
		free_split(parts);
		parse_error(scene, "Invalid format for sphere color. Expected: r,g,b");
	}
	return (1);
}

/*
** Parses a sphere definition line from the scene file.
** Expected format: "sp position diameter color [material_properties]"
*/
int	parse_sphere(t_scene *scene, char *line)
{
	char		**parts;
	t_object	*sphere_obj;
	t_sphere	sphere_data;
	t_color		color;
	char		*material_block;

	material_block = extract_material_block(line);
	if (material_block)
		trim_material_block(line);
	parts = ft_split_line(line, ' ');
	if (!parts)
		parse_error(scene, "Failed to split line");
	check_parts_count(scene, parts, 4, "sphere");
	if (!parse_sphere_data(scene, parts, &sphere_data, &color))
		return (0);
	free_split(parts);
	sphere_obj = create_sphere(sphere_data.center, sphere_data.diameter, color);
	if (!sphere_obj)
		parse_error(scene, "Failed to create sphere");
	if (material_block)
		parse_material_properties(scene, material_block, &sphere_obj->material);
	add_object(scene, sphere_obj);
	return (1);
}
