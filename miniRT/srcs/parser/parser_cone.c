/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cone.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 18:30:00 by abillote          #+#    #+#             */
/*   Updated: 2025/06/11 10:00:11 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

t_object	*create_cone(t_vec3 apex, t_vec3 axis, double radius, double height)
{
	t_object	*object;
	t_cone		*cone;

	object = malloc(sizeof(t_object));
	if (!object)
		return (NULL);
	cone = malloc(sizeof(t_cone));
	if (!cone)
	{
		free(object);
		return (NULL);
	}
	cone->apex = apex;
	cone->axis = vec3_normalize(axis);
	cone->radius = radius;
	cone->height = height;
	cone->angle = atan2(cone->radius, cone->height);
	object->data = cone;
	object->material = create_material(create_color(255, 255, 255));
	object->type = CONE;
	object->next = NULL;
	return (object);
}

void	parse_cone_vectors(t_scene *scene, char **parts, t_cone *cone)
{
	if (!read_vector(parts[1], &cone->apex))
	{
		free_split(parts);
		parse_error(scene, "Invalid format for cone apex. Expected: x,y,z");
	}
	if (!read_vector(parts[2], &cone->axis))
	{
		free_split(parts);
		parse_error(scene, "Invalid format for cone axis. Expected: nx,ny,nz");
	}
	if (!check_vector_normalization(cone->axis))
	{
		free_split(parts);
		parse_error(scene, "Invalid format for cone axis. Expected: nx,ny,nz");
	}
}

void	parse_cone_data(t_scene *scene, char **parts, \
							t_cone *cone, t_color *color)
{
	cone->radius = ft_atof(parts[3]);
	cone->height = ft_atof(parts[4]);
	if (!check_range(cone->radius, 0.0, INFINITY))
	{
		free_split(parts);
		parse_error(scene, "Expected for cone radius: positive number");
	}
	if (!check_range(cone->height, 0.0, INFINITY))
	{
		free_split(parts);
		parse_error(scene, "Expected for cone height: positive number");
	}
	if (!read_color(parts[5], color))
	{
		free_split(parts);
		parse_error(scene, "Expected for cone color: r,g,b");
	}
}

int	parse_cone(t_scene *scene, char *line)
{
	char		**parts;
	t_object	*cone;
	t_cone		cone_data;
	t_color		color;
	char		*material_block;

	material_block = extract_material_block(line);
	if (material_block)
		trim_material_block(line);
	parts = ft_split_line(line, ' ');
	if (!parts)
		parse_error(scene, "Failed to split line");
	check_parts_count(scene, parts, 6, "cone");
	parse_cone_vectors(scene, parts, &cone_data);
	parse_cone_data(scene, parts, &cone_data, &color);
	free_split(parts);
	cone = create_cone(cone_data.apex, cone_data.axis, \
						cone_data.radius, cone_data.height);
	if (!cone)
		parse_error(scene, "Failed to create cone");
	cone->material.color = color;
	if (material_block)
		parse_material_properties(scene, material_block, &cone->material);
	add_object(scene, cone);
	return (1);
}
