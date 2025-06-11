/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cylinder.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 18:30:00 by abillote          #+#    #+#             */
/*   Updated: 2025/06/11 17:58:01 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

t_object	*create_cylinder(t_vec3 center, t_vec3 axis, \
								double diameter, double height)
{
	t_object	*object;
	t_cylinder	*cylinder;

	object = malloc(sizeof(t_object));
	if (!object)
		return (NULL);
	cylinder = malloc(sizeof(t_cylinder));
	if (!cylinder)
	{
		free(object);
		return (NULL);
	}
	cylinder->center = center;
	cylinder->axis = vec3_normalize(axis);
	cylinder->diameter = diameter;
	cylinder->radius = diameter / 2.0;
	cylinder->height = height;
	object->data = cylinder;
	object->material = create_material(create_color(255, 255, 255));
	object->type = CYLINDER;
	object->next = NULL;
	return (object);
}

void	parse_cylinder_vectors(t_scene *scene, char **parts, \
								t_cylinder *cylinder)
{
	if (!read_vector(parts[1], &cylinder->center))
	{
		free_split(parts);
		parse_error(scene, "Expected for cylinder center: x,y,z");
	}
	if (!read_vector(parts[2], &cylinder->axis))
	{
		free_split(parts);
		parse_error(scene, "Expected for cylinder axis: nx,ny,nz");
	}
	if (!check_vector_normalization(cylinder->axis))
	{
		free_split(parts);
		parse_error(scene, "Expected for cylinder axis: nx,ny,nz");
	}
}

void	parse_cylinder_data(t_scene *scene, char **parts, \
								t_cylinder *cylinder, t_color *color)
{
	cylinder->diameter = ft_atof(parts[3]);
	cylinder->height = ft_atof(parts[4]);
	if (!check_range(cylinder->diameter, 0.0, INFINITY))
	{
		free_split(parts);
		parse_error(scene, "Expected positive number for cylinder diameter");
	}
	if (!check_range(cylinder->height, 0.0, INFINITY))
	{
		free_split(parts);
		parse_error(scene, "Expected positive number for cylinder height");
	}
	if (!read_color(parts[5], color))
	{
		free_split(parts);
		parse_error(scene, "Expected for cylinder color: r,g,b");
	}
}

int	parse_cylinder(t_scene *scene, char *line)
{
	char		**parts;
	t_object	*cylinder;
	t_cylinder	cylinder_data;
	t_color		color;
	char		*material_block;

	material_block = extract_material_block(line);
	if (material_block)
		trim_material_block(line);
	parts = ft_split_line(line, ' ');
	if (!parts)
		parse_error(scene, "Failed to split line");
	check_parts_count(scene, parts, 6, "cylinder");
	parse_cylinder_vectors(scene, parts, &cylinder_data);
	parse_cylinder_data(scene, parts, &cylinder_data, &color);
	free_split(parts);
	cylinder = create_cylinder(cylinder_data.center, cylinder_data.axis, \
								cylinder_data.diameter, cylinder_data.height);
	if (!cylinder)
		parse_error(scene, "Failed to create cylinder");
	cylinder->material.color = color;
	if (material_block)
		parse_material_properties(scene, material_block, &cylinder->material);
	add_object(scene, cylinder);
	return (1);
}
