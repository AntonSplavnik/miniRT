/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_plane.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 18:30:00 by abillote          #+#    #+#             */
/*   Updated: 2025/05/21 14:56:18 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "platform.h"

void	parse_plane_data(t_scene *scene, char **parts, t_vec3 *point, t_vec3 *normal, t_color *color)
{
	if (!read_vector(parts[1], point))
	{
		free_split(parts);
		parse_error(scene, "Invalid format for plane position. Expected: x,y,z");
	}
	if (!read_vector(parts[2], normal))
	{
		free_split(parts);
		parse_error(scene, "Invalid format for plane normal. Expected: nx,ny,nz");
	}
	if (!check_vector_normalization(*normal))
	{
		free_split(parts);
		parse_error(scene, "Invalid format for plane normal. Expected: nx,ny,nz");
	}
	if (!read_color(parts[3], color))
	{
		free_split(parts);
		parse_error(scene, "Invalid format for plane color. Expected: r,g,b");
	}
}

int	parse_plane(t_scene *scene, char *line)
{
	char	**parts;
	t_object	*plane;
	t_vec3		point;
	t_vec3		normal;
	t_color		color;

	parts = ft_split(line, ' ');
	if (!parts)
		parse_error(scene, "Failed to split line");
	check_parts_count(scene, parts, 4, "plane");
	parse_plane_data(scene, parts, &point, &normal, &color);
	free_split(parts);
	plane = create_plane(point, normal, color);
	if(!plane)
		parse_error(scene, "Failed to create plane");
	add_object(scene, plane);
	return (1);
}
