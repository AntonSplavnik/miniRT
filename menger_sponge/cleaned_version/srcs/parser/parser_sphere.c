/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_sphere.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 18:30:00 by abillote          #+#    #+#             */
/*   Updated: 2025/05/13 18:32:13 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "platform.h"

int	parse_sphere_data(t_scene *scene, char **parts, t_vec3 *center, double *diameter, t_color *color)
{
	if (!read_vector(parts[1], center))
	{
		free_split(parts);
		parse_error(scene, "Invalid format for sphere position. Expected: x,y,z");
	}
	*diameter = ft_atof(parts[2]);
	if (!check_range(*diameter, 0.0, INFINITY))
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

int	parse_sphere(t_scene *scene, char *line)
{
	char		**parts;
	int			parse_result;
	t_object	*sphere;
	t_vec3		center;
	double		diameter;
	t_color		color;

	parts = ft_split(line, ' ');
	if (!parts)
		parse_error(scene, "Failed to split line");
	check_parts_count(scene, parts, 4, "sphere");
	parse_result = parse_sphere_data(scene, parts, &center, &diameter, &color);
	if (!parse_result)
		return (0);
	free_split(parts);
	sphere = create_sphere(center, diameter, color);
	if(!sphere)
		parse_error(scene, "Failed to create sphere");
	add_object(scene, sphere);
	return (1);
}
