/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_triangle.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 18:30:00 by abillote          #+#    #+#             */
/*   Updated: 2025/06/06 15:45:58 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

void	parse_triangle_data(t_scene *scene, char **parts, \
								t_triangle *triangle, t_color *color)
{
	if (!read_vector(parts[1], &triangle->v0))
	{
		free_split(parts);
		parse_error(scene, "Expected for triangle vertex 1: x,y,z");
	}
	if (!read_vector(parts[2], &triangle->v1))
	{
		free_split(parts);
		parse_error(scene, "Expected for triangle vertex 2: x,y,z");
	}
	if (!read_vector(parts[3], &triangle->v2))
	{
		free_split(parts);
		parse_error(scene, "Expected for triangle vertex 3: x,y,z");
	}
	if (!read_color(parts[4], color))
	{
		free_split(parts);
		parse_error(scene, "Expected for triangle color: r,g,b");
	}
}

int	parse_triangle(t_scene *scene, char *line)
{
	char		**parts;
	t_object	*triangle;
	t_triangle	triangle_data;
	t_color		color;
	char		*material_block;

	material_block = extract_material_block(line);
	if (material_block)
		trim_material_block(line);
	parts = ft_split_line(line, ' ');
	if (!parts)
		parse_error(scene, "Failed to split line");
	check_parts_count(scene, parts, 5, "triangle");
	parse_triangle_data(scene, parts, &triangle_data, &color);
	free_split(parts);
	triangle = create_triangle(triangle_data.v0, triangle_data.v1, \
								triangle_data.v2, color);
	if (!triangle)
		parse_error(scene, "Failed to create triangle");
	if (material_block)
		parse_material_properties(scene, material_block, &triangle->material);
	add_object(scene, triangle);
	return (1);
}
