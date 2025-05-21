/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_material.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 12:20:00 by abillote          #+#    #+#             */
/*   Updated: 2025/05/21 17:27:00 by abillote         ###   ########.fr       */
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
 * Extract a color value from a material block
 *
 * @param material_block The material block string
 * @param property The property name to look for
 * @param color Pointer to the color structure to fill
 * @return 1 if property was found and parsed, 0 otherwise
 */
int get_property_color(const char *material_block, const char *property, t_color *color)
{
    char *prop_pos;
    char property_with_colon[50];
    int r = 0, g = 0, b = 0;
    int values_read = 0;
    char *p;

    // Create the property string with colon (e.g., "checker_color:")
    ft_strlcpy(property_with_colon, property, sizeof(property_with_colon));
    ft_strlcat(property_with_colon, ":", sizeof(property_with_colon));

    // Find the property in the material block
    prop_pos = ft_strstr(material_block, property_with_colon);
    if (!prop_pos)
    {
        printf("Property '%s' not found in material block\n", property);
        return (0);
    }

    // Move past the property name and colon
    prop_pos += ft_strlen(property_with_colon);

    // Debug the entire material block and the position
    printf("Material block: %s\n", material_block);
    printf("Position after property: %s\n", prop_pos);

    // Try to manually parse the RGB values
    // Parse first value (red)
    r = strtol(prop_pos, &p, 10);
    if (p == prop_pos) {
        printf("Failed to parse red component\n");
        return 0;
    }
    values_read++;

    // Find and parse second value (green)
    if (*p == ',') {
        prop_pos = p + 1;
        g = strtol(prop_pos, &p, 10);
        if (p == prop_pos) {
            printf("Failed to parse green component\n");
            return 0;
        }
        values_read++;
    }

    // Find and parse third value (blue)
    if (*p == ',') {
        prop_pos = p + 1;
        b = strtol(prop_pos, &p, 10);
        if (p == prop_pos) {
            printf("Failed to parse blue component\n");
            return 0;
        }
        values_read++;
    }

    // Check if we got all three components
    if (values_read == 3) {
        printf("Successfully parsed RGB: %d,%d,%d\n", r, g, b);
        color->r = r;
        color->g = g;
        color->b = b;
        return 1;
    } else {
        printf("Only parsed %d values instead of 3\n", values_read);
        return 0;
    }
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
	double	value;
	t_color	checker_color;

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

	// Parse checker size
	value = get_property_value(material_block, "checker_size");
	if (value > 0)
		material->checker_size = value;

	// Parse checker color
	if(get_property_color(material_block, "checker_color", &checker_color))
	{
		material->checker_color = checker_color;
		material->has_checker = 1;
	}
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
