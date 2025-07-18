/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_material_helpers.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 15:10:00 by abillote          #+#    #+#             */
/*   Updated: 2025/07/18 15:17:44 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

double	get_property_value(const char *material_block, const char *property)
{
	char	*property_pos;

	property_pos = ft_strstr(material_block, property);
	if (!property_pos)
		return (-1.0);
	property_pos += ft_strlen(property);
	return (ft_atof(property_pos));
}

void	*get_property_filename(t_scene *scene, const char *material_block, \
							const char *property, char **filename)
{
	char	*property_pos;
	char	*end_pos;
	int		length;

	property_pos = ft_strstr(material_block, property);
	if (!property_pos)
		return (0);
	property_pos += ft_strlen(property);
	end_pos = ft_strchr(property_pos, ' ');
	if (!end_pos)
		end_pos = ft_strchr(property_pos, '}');
	if (!end_pos)
		parse_error(scene, "Missing closing bracket in material block");
	length = end_pos - property_pos;
	if (length <= 0)
		parse_error(scene, "Empty filename in material block");
	*filename = (char *)malloc(length + 1);
	if (!*filename)
		return (NULL);
	ft_strlcpy(*filename, property_pos, length + 1);
	return (*filename);
}

void	get_texture_bump_map(t_scene *scene, char *material_block, \
						t_material *material)
{
	char	*texture_filename;
	char	*bump_map_filename;

	if (get_property_filename(scene, material_block, "texture:", \
							&texture_filename))
	{
		material->has_texture = 1;
		material->texture = create_texture(scene, texture_filename);
		free(texture_filename);
	}
	if (get_property_filename(scene, material_block, \
							"bumpmap:", &bump_map_filename))
	{
		material->has_bump_map = 1;
		material->bump_map = create_bump_map(scene, bump_map_filename);
		free(bump_map_filename);
	}
}
