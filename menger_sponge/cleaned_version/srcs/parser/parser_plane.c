/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_plane.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 18:30:00 by abillote          #+#    #+#             */
/*   Updated: 2025/06/11 10:03:49 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

t_object	*create_plane(t_vec3 point, t_vec3 normal, t_color color)
{
	t_object	*object;
	t_plane		*plane;

	object = malloc(sizeof(t_object));
	if (!object)
		return (NULL);
	plane = malloc(sizeof(t_plane));
	if (!plane)
	{
		free(object);
		return (NULL);
	}
	plane->normal = vec3_normalize(normal);
	plane->point = point;
	object->data = plane;
	object->material = create_material(color);
	object->type = PLANE;
	object->next = NULL;
	return (object);
}

void	parse_plane_data(t_scene *scene, char **parts, \
							t_plane *plane, t_color *color)
{
	if (!read_vector(parts[1], &plane->point))
	{
		free_split(parts);
		parse_error(scene, "Expected format for plane position: x,y,z");
	}
	if (!read_vector(parts[2], &plane->normal))
	{
		free_split(parts);
		parse_error(scene, "Expected format for plane normal: nx,ny,nz");
	}
	if (!check_vector_normalization(plane->normal))
	{
		free_split(parts);
		parse_error(scene, "Expected format for plane normal: nx,ny,nz");
	}
	if (!read_color(parts[3], color))
	{
		free_split(parts);
		parse_error(scene, "Invalid format for plane color. Expected: r,g,b");
	}
}

int	parse_plane(t_scene *scene, char *line)
{
	char		**parts;
	t_object	*plane_obj;
	t_plane		plane_data;
	t_color		color;
	char		*material_block;

	material_block = extract_material_block(line);
	if (material_block)
		trim_material_block(line);
	parts = ft_split_line(line, ' ');
	if (!parts)
		parse_error(scene, "Failed to split line");
	check_parts_count(scene, parts, 4, "plane");
	parse_plane_data(scene, parts, &plane_data, &color);
	free_split(parts);
	plane_obj = create_plane(plane_data.point, plane_data.normal, color);
	if (!plane_obj)
		parse_error(scene, "Failed to create plane");
	if (material_block)
		parse_material_properties(scene, material_block, &plane_obj->material);
	add_object(scene, plane_obj);
	return (1);
}
