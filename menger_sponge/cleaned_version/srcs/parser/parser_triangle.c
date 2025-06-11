/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_triangle.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 18:30:00 by abillote          #+#    #+#             */
/*   Updated: 2025/06/11 10:04:43 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

t_object	*create_triangle(t_vec3 v0, t_vec3 v1, t_vec3 v2, t_color color)
{
	t_object	*object;
	t_triangle	*triangle;

	object = malloc(sizeof(t_object));
	if (!object)
		return (NULL);
	triangle = malloc(sizeof(t_triangle));
	if (!triangle)
	{
		free(object);
		return (NULL);
	}
	triangle->v0 = v0;
	triangle->v1 = v1;
	triangle->v2 = v2;

	// Precompute normal for efficiency
	t_vec3 edge1 = vec3_subtract(v1, v0);
	t_vec3 edge2 = vec3_subtract(v2, v0);
	triangle->normal = vec3_normalize(vec3_cross(edge1, edge2));
	object->data = triangle;
	object->material = create_material(color);
	object->type = TRIANGLE;
	object->next = NULL;
	return (object);
}

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
