/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_plane.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 18:30:00 by abillote          #+#    #+#             */
/*   Updated: 2025/06/04 16:08:05 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

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
	char		*material_block;

	material_block = extract_material_block(line);
	if (material_block)
		trim_material_block(line);
	parts = ft_split_line(line, ' ');
	if (!parts)
		parse_error(scene, "Failed to split line");
	check_parts_count(scene, parts, 4, "plane");
	parse_plane_data(scene, parts, &point, &normal, &color);
	free_split(parts);
	plane = create_plane(point, normal, color);
	if(!plane)
		parse_error(scene, "Failed to create plane");
	if(material_block)
	{
		parse_material_properties(scene, material_block, &plane->material);
		free(material_block);
	}
	add_object(scene, plane);
	return (1);
}
