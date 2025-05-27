/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cone.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 18:30:00 by abillote          #+#    #+#             */
/*   Updated: 2025/05/21 15:41:01 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

void	parse_cone_vectors(t_scene *scene, char **parts, t_vec3 *apex, t_vec3 *axis)
{
	if (!read_vector(parts[1], apex))
	{
		free_split(parts);
		parse_error(scene, "Invalid format for cone apex. Expected: x,y,z");
	}
	if (!read_vector(parts[2], axis))
	{
		free_split(parts);
		parse_error(scene, "Invalid format for cone axis. Expected: nx,ny,nz");
	}
	if (!check_vector_normalization(*axis))
	{
		free_split(parts);
		parse_error(scene, "Invalid format for cone axis. Expected: nx,ny,nz");
	}
}

void	parse_cone_data(t_scene *scene, char **parts, double *radius, double *height, t_color *color)
{
	*radius = ft_atof(parts[3]);
	*height = ft_atof(parts[4]);
	if (!check_range(*radius, 0.0, INFINITY))
	{
		free_split(parts);
		parse_error(scene, "Invalid format for cone radius. Expected: positive number");
	}
	if (!check_range(*height, 0.0, INFINITY))
	{
		free_split(parts);
		parse_error(scene, "Invalid format for cone height. Expected: positive number");
	}
	if (!read_color(parts[5], color))
	{
		free_split(parts);
		parse_error(scene, "Invalid format for cone color. Expected: r,g,b");
	}
}

int	parse_cone(t_scene *scene, char *line)
{
	char	**parts;
	t_object	*cone;
	t_vec3		apex;
	t_vec3		axis;
	double		radius;
	double		height;
	t_color		color;
	char		*material_block;

	material_block = extract_material_block(line);
	if (material_block)
		trim_material_block(line);
	parts = ft_split(line, ' ');
	if (!parts)
		parse_error(scene, "Failed to split line");
	check_parts_count(scene, parts, 6, "cone");
	parse_cone_vectors(scene, parts, &apex, &axis);
	parse_cone_data(scene, parts, &radius, &height, &color);
	free_split(parts);
	cone = create_cone(apex, axis, radius, height);
	if(!cone)
		parse_error(scene, "Failed to create cone");
	cone->material.color = color;
	if(material_block)
	{
		parse_material_properties(material_block, &cone->material);
		free(material_block);
	}
	add_object(scene, cone);
	return (1);
}
