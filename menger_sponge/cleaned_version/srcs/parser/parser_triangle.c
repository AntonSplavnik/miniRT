/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_triangle.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 18:30:00 by abillote          #+#    #+#             */
/*   Updated: 2025/06/06 15:01:06 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

void	parse_triangle_data(t_scene *scene, char **parts, t_vec3 *v0, t_vec3 *v1, t_vec3 *v2)
{
	if (!read_vector(parts[1], v0))
	{
		free_split(parts);
		parse_error(scene, "Invalid format for triangle vertex 1. Expected: x,y,z");
	}
	if (!read_vector(parts[2], v1))
	{
		free_split(parts);
		parse_error(scene, "Invalid format for triangle vertex 2. Expected: x,y,z");
	}
	if (!read_vector(parts[3], v2))
	{
		free_split(parts);
		parse_error(scene, "Invalid format for triangle vertex 3. Expected: x,y,z");
	}
}

int	parse_triangle(t_scene *scene, char *line)
{
	char	**parts;
	t_object	*triangle;
	t_vec3		v0;
	t_vec3		v1;
	t_vec3		v2;
	t_color		color;
	char		*material_block;

	material_block = extract_material_block(line);
	if (material_block)
		trim_material_block(line);
	parts = ft_split_line(line, ' ');
	if (!parts)
		parse_error(scene, "Failed to split line");
	check_parts_count(scene, parts, 5, "triangle");
	parse_triangle_data(scene, parts, &v0, &v1, &v2);
	if (!read_color(parts[4], &color))
	{
		free_split(parts);
		parse_error(scene, "Invalid format for triangle color. Expected: r,g,b");
	}
	free_split(parts);
	triangle = create_triangle(v0, v1, v2, color);
	if (!triangle)
		parse_error(scene, "Failed to create triangle");
	if(material_block)
		parse_material_properties(scene, material_block, &triangle->material);
	add_object(scene, triangle);
	return (1);
}
