/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_material.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 12:20:00 by abillote          #+#    #+#             */
/*   Updated: 2025/06/11 12:06:08 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"


t_material	create_material(t_color color)
{
	t_material	material;

	material.color = color;
	material.specular = 0.5;
	material.shininess = 64;
	material.reflectivity = 0.0;
	material.has_checker = 0;
	material.checker_size = 0;
	material.checker_color = create_color(0, 0, 0);
	material.has_texture = 0;
	material.texture = NULL;
	material.has_bump_map = 0;
	material.bump_map = NULL;
	return (material);
}

/**
 * Extracts the material block (if any) from a line in the RT file
 * The material block is expected to be in the format:
 * {property1:value1 property2:value2 ...}
 *
 * @param line The full line from the RT file
 * @return A newly allocated string containing just the material block,
 * or NULL if none found
 */
char	*extract_material_block(char *line)
{
	char	*start;
	char	*end;
	char	*material_block;
	int		length;

	start = ft_strchr(line, '{');
	if (!start)
		return (NULL);
	end = ft_strchr(start, '}');
	if (!end)
		return (NULL);
	length = end - start + 1;
	material_block = (char *)malloc(length + 1);
	if (!material_block)
		return (NULL);
	ft_strlcpy(material_block, start, length + 1);
	return (material_block);
}

/**
 * Trims the material block from a line, leaving only the standard RT format
 *
 * @param line The line to trim
 */
void trim_material_block(char *line)
{
	char *start = ft_strchr(line, '{');
	if (start)
		*start = '\0';
}

/**
 * Extract a color value from a material block
 *
 * @param material_block The material block string
 * @param property The property name to look for
 * @param color Pointer to the color structure to fill
 * @return 1 if property was found and parsed, 0 otherwise
 */
int get_property_color(t_scene *scene, const char *material_block, const char *property, t_color *color)
{
	char *prop_pos;
	char property_with_colon[50];
	char *color_str = NULL;
	char *end_pos;
	int length;

	// Create the property string with colon (e.g., "checker_color:")
	ft_strlcpy(property_with_colon, property, sizeof(property_with_colon));
	ft_strlcat(property_with_colon, ":", sizeof(property_with_colon));

	// Find the property in the material block
	prop_pos = ft_strstr(material_block, property_with_colon);
	if (!prop_pos)
		return (0);  // Property not found, but this is not an error

	// Move past the property name and colon
	prop_pos += ft_strlen(property_with_colon);

	// Find the end of the color string (should end with space or closing brace)
	end_pos = ft_strchr(prop_pos, ' ');
	if (!end_pos)
		end_pos = ft_strchr(prop_pos, '}');
	if (!end_pos)
	{
		parse_error(scene, "Invalid color format in material block");
		return (0);
	}

	// Calculate the length of the color string
	length = end_pos - prop_pos;
	if (length <= 0)
	{
		parse_error(scene, "Empty color value in material block");
		return (0);
	}

	// Allocate memory for the color string
	color_str = (char *)malloc(length + 1);
	if (!color_str)
		return (0);

	// Copy the color string
	ft_strlcpy(color_str, prop_pos, length + 1);

	// Use read_color to parse the color string
	int result = read_color(color_str, color);
	// Free the color string
	free(color_str);

	if (!result)
	{
		parse_error(scene, "Invalid color format in material block. Expected: r,g,b");
		return (0);
	}

	return (1);
}

/**
 * Extracts a property value from a material block string
 *
 * @param material_block The material block string
 * @param property The property name to look for
 * @return The property value as a double, or -1.0 if not found
 */
double get_property_value(const char *material_block, const char *property)
{
	char	*property_pos;

	property_pos = ft_strstr(material_block, property);
	if (!property_pos)
		return (-1.0);
	property_pos += ft_strlen(property);
	return (ft_atof(property_pos));
}

void	*get_property_filename(const char *material_block, const char *property, char **filename)
{
	char *prop_pos;
	char property_with_colon[50];

	// Create the property string with colon (e.g., "texture:")
	ft_strlcpy(property_with_colon, property, sizeof(property_with_colon));
	ft_strlcat(property_with_colon, ":", sizeof(property_with_colon));

	// Find the property in the material block
	prop_pos = ft_strstr(material_block, property_with_colon);
	if (!prop_pos)
		return (NULL);

	// Move past the property name and colon
	prop_pos += ft_strlen(property_with_colon);

	//Find the end of the texture filename
	char *end_pos = ft_strchr(prop_pos, ' ');
	if (!end_pos)
		end_pos = ft_strchr(prop_pos, '}'); // If no space, look for closing brace
	if (!end_pos)
	{
		printf("Error: missing closing bracket in material block\n");
		return (NULL);
	}
	// Calculate the length of the texture filename
	int length = end_pos - prop_pos;
	// Allocate memory for the texture filename
	if (length <= 0)
		return (NULL);
	*filename = (char *)malloc(length + 1);
	if (!*filename)
		return (NULL);
	// Copy the texture filename
	ft_strlcpy(*filename, prop_pos, length + 1);
	// Return the texture filename
	return (*filename);
}

void	get_texture_bump_map(t_scene *scene, char *material_block, t_material *material)
{
	char	*texture_filename;
	char	*bump_map_filename;

	if (get_property_filename(material_block, "texture", &texture_filename))
	{
		material->has_texture = 1;
		material->texture = create_texture(scene, texture_filename);
		free(texture_filename);
	}
	if (get_property_filename(material_block, "bumpmap", &bump_map_filename))
	{
		material->has_bump_map = 1;
		material->bump_map = create_bump_map(scene, bump_map_filename);
		free(bump_map_filename);
	}
}

/**
 * Parses material properties from a material block and applies them to a material
 * Expected material block format: {property1:value1 property2:value2 ...}
 * @param material_block The material block string
 * @param material Pointer to the material to update
 */
void parse_material_properties(t_scene *scene, char *material_block, t_material *material)
{
	double	value;
	t_color	checker_color;

	value = get_property_value(material_block, "reflectivity:");
	if (value >= 0.0)
		material->reflectivity = ft_clamp(value, 0.0, 1.0);
	value = get_property_value(material_block, "specular:");
	if (value >= 0.0)
		material->specular = ft_clamp(value, 0.0, 1.0);
	value = get_property_value(material_block, "shininess:");
	if (value >= 0.0)
		material->shininess = value;
	value = get_property_value(material_block, "checker_size:");
	if (value > 0)
		material->checker_size = value;
	if(get_property_color(scene, material_block, "checker_color", &checker_color))
	{
		material->checker_color = checker_color;
		material->has_checker = 1;
	}
	get_texture_bump_map(scene, material_block, material);
	free(material_block);
}


