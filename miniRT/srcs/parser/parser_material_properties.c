/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_material_properties.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 13:46:37 by abillote          #+#    #+#             */
/*   Updated: 2025/06/11 13:57:15 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

/**
 * Extract a color value from a material block
 *
 * @param material_block The material block string
 * @param property The property name to look for
 * @param color Pointer to the color structure to fill
 * @return 1 if property was found and parsed, 0 otherwise
 */
int	get_property_color(t_scene *scene, const char *material_block, \
						const char *property, t_color *color)
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
		parse_error(scene, "Invalid color format in material block");
	length = end_pos - property_pos;
	if (length <= 0)
		parse_error(scene, "Empty color value in material block");
	parse_checker_color(scene, color, property_pos, length);
	return (1);
}

/**
 * Extracts a property value from a material block string
 *
 * @param material_block The material block string
 * @param property The property name to look for
 * @return The property value as a double, or -1.0 if not found
 */
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
		material->bump_map = create_bump_map(scene, \
										bump_map_filename);
		free(bump_map_filename);
	}
}

/**
 * Parses material properties from a material block
 * and applies them to a material.
 * Expected material block format: {property1:value1 property2:value2 ...}
 * @param material_block The material block string
 * @param material Pointer to the material to update
 */
void	parse_material_properties(t_scene *scene, char *material_block, \
								t_material *material)
{
	double	value;
	t_color	checker_color;

	value = get_property_value(material_block, "reflectivity:");
	if (value >= 0.0)
		material->reflectivity = ft_clamp(value, 0.0, 1.0);
	value = get_property_value(material_block, "transparency:");
	if (value >= 0.0)
		material->transparency = ft_clamp(value, 0.0, 1.0);
	value = get_property_value(material_block, "refractive_index:");
	if (value >= 0.0)
		material->refractive_index = value;
	// Set has_refraction if either transparency or refractive_index is specified
	if (material->transparency > 0.0 && material->refractive_index > 0.0)
		material->has_refraction = true;
	value = get_property_value(material_block, "specular:");
	if (value >= 0.0)
		material->specular = ft_clamp(value, 0.0, 1.0);
	value = get_property_value(material_block, "shininess:");
	if (value >= 0.0)
		material->shininess = value;
	value = get_property_value(material_block, "checker_size:");
	if (value > 0)
		material->checker_size = value;
	if (get_property_color(scene, material_block, "checker_color:", \
							&checker_color))
	{
		material->checker_color = checker_color;
		material->has_checker = 1;
	}
	get_texture_bump_map(scene, material_block, material);
	free(material_block);
}
