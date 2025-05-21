/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_material.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 12:20:00 by abillote          #+#    #+#             */
/*   Updated: 2025/05/21 14:52:58 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/**
 * Extracts the material block (if any) from a line in the RT file
 * The material block is expected to be in the format: {property1:value1,property2:value2,...}
 *
 * @param line The full line from the RT file
 * @return A newly allocated string containing just the material block, or NULL if none found
 */
char *extract_material_block(char *line)
{
    char *start;
    char *end;
    char *material_block;
    int length;

    // Find opening brace
    start = ft_strchr(line, '{');
    if (!start)
        return (NULL);

    // Find closing brace
    end = ft_strchr(start, '}');
    if (!end)
        return (NULL);

    // Calculate length of material block (including braces)
    length = end - start + 1;

    // Allocate and copy the material block
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
 * Extracts a property value from a material block string
 *
 * @param material_block The material block string
 * @param property The property name to look for
 * @return The property value as a double, or -1.0 if not found
 */
double get_property_value(const char *material_block, const char *property)
{
    char *prop_pos;
    char property_with_colon[50];

    // Create the property string with colon (e.g., "reflectivity:")
    ft_strlcpy(property_with_colon, property, sizeof(property_with_colon));
    ft_strlcat(property_with_colon, ":", sizeof(property_with_colon));

    // Find the property in the material block
    prop_pos = ft_strstr(material_block, property_with_colon);
    if (!prop_pos)
        return (-1.0);

    // Move past the property name and colon
    prop_pos += ft_strlen(property_with_colon);

    // Parse the value
    return (ft_atof(prop_pos));
}

/**
 * Parses material properties from a material block and applies them to a material
 *
 * @param material_block The material block string
 * @param material Pointer to the material to update
 */
void parse_material_properties(const char *material_block, t_material *material)
{
    double value;

    // Parse reflectivity
    value = get_property_value(material_block, "reflectivity");
    if (value >= 0.0)
        material->reflectivity = ft_clamp(value, 0.0, 1.0);

    // Parse transparency
    value = get_property_value(material_block, "transparency");
    if (value >= 0.0)
        material->transparency = ft_clamp(value, 0.0, 1.0);

    // Parse refractive index
    value = get_property_value(material_block, "refractive_index");
    if (value >= 0.0)
        material->refractive_index = value > 1.0 ? value : 1.0;

    // Parse specular
    value = get_property_value(material_block, "specular");
    if (value >= 0.0)
        material->specular = ft_clamp(value, 0.0, 1.0);

    // Parse shininess
    value = get_property_value(material_block, "shininess");
    if (value >= 0.0)
        material->shininess = value;
}

/**
 * Utility function to clamp a value between min and max
 *
 * @param value The value to clamp
 * @param min The minimum allowed value
 * @param max The maximum allowed value
 * @return The clamped value
 */
double ft_clamp(double value, double min, double max)
{
    if (value < min)
        return (min);
    if (value > max)
        return (max);
    return (value);
}
