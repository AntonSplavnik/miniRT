/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_cylinder.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 18:30:00 by abillote          #+#    #+#             */
/*   Updated: 2025/05/13 18:32:39 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "platform.h"

void	parse_cylinder_vectors(t_scene *scene, char **parts, t_vec3 *center, t_vec3 *axis)
{
	if (!read_vector(parts[1], center))
	{
		free_split(parts);
		parse_error(scene, "Invalid format for cylinder center Expected: x,y,z");
	}
	if (!read_vector(parts[2], axis))
	{
		free_split(parts);
		parse_error(scene, "Invalid format for cylinder axis. Expected: nx,ny,nz");
	}
	if (!check_vector_normalization(*axis))
	{
		free_split(parts);
		parse_error(scene, "Invalid format for cylinder axis. Expected: nx,ny,nz");
	}
}

void	parse_cylinder_data(t_scene *scene, char **parts, double *diameter, double *height, t_color *color)
{
	*diameter = ft_atof(parts[3]);
	*height = ft_atof(parts[4]);
	if (!check_range(*diameter, 0.0, INFINITY))
	{
		free_split(parts);
		parse_error(scene, "Invalid format for cylinder diameter. Expected: positive number");
	}
	if (!check_range(*height, 0.0, INFINITY))
	{
		free_split(parts);
		parse_error(scene, "Invalid format for cylinder height. Expected: positive number");
	}
	if (!read_color(parts[5], color))
	{
		free_split(parts);
		parse_error(scene, "Invalid format for cylinder color. Expected: r,g,b");
	}
}

int	parse_cylinder(t_scene *scene, char *line)
{
	char	**parts;
	t_object	*cylinder;
	t_vec3		center;
	t_vec3		axis;
	double		diameter;
	double		height;
	t_color		color;

	parts = ft_split(line, ' ');
	if (!parts)
		parse_error(scene, "Failed to split line");
	check_parts_count(scene, parts, 6, "cylinder");
	parse_cylinder_vectors(scene, parts, &center, &axis);
	parse_cylinder_data(scene, parts, &diameter, &height, &color);
	free_split(parts);
	cylinder = create_cylinder(center, axis, diameter, height);
	if(!cylinder)
		parse_error(scene, "Failed to create cylinder");
	cylinder->material.color = color;
	add_object(scene, cylinder);
	return (1);
}
