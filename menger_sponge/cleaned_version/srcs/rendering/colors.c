/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 13:07:03 by abillote          #+#    #+#             */
/*   Updated: 2025/05/27 10:42:43 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

int	valid_color_range(int c)
{
	if (c < 0)
		return 0;
	else if (c > 255)
		return 255;
	else
		return (c);
}

t_color	create_color(int r, int g, int b)
{
	t_color color;

	color.r = valid_color_range(r);
	color.g = valid_color_range(g);
	color.b = valid_color_range(b);
	return (color);
}

/**
 * Convert t_color to an integer RGB value
 *
 * @param color The color structure
 * @return Integer representation (0xRRGGBB)
 */
int color_to_int(t_color color)
{
	int r = (int)color.r;
	int g = (int)color.g;
	int b = (int)color.b;

	return (r << 16) | (g << 8) | b;
}

/**
 * Get pixel color for a material at a specific point with lighting applied
 * Supports checkerboard textures
 *
 * @param hit_object The object that was hit
 * @param light_intensity The amount of light at the point
 * @param light_color The color of the light
 * @param hit_point The point of intersection
 * @return The final pixel color
 */
int get_pixel_color(t_hit_record hit_record, double light_intensity,
                               t_color light_color,
                               double specular_intensity)
{
    // Get the base color from the material, considering checkerboard pattern
    t_color base_color = get_checker_color(hit_record.object->material, hit_record.object, hit_record.point);

    // Calculate diffuse component (subtract specular from total light)
    double diffuse_component = light_intensity - specular_intensity;
    if (diffuse_component < 0)
        diffuse_component = 0;

    // Apply diffuse lighting calculations
    int r = (int)(base_color.r * diffuse_component * light_color.r / 255.0);
    int g = (int)(base_color.g * diffuse_component * light_color.g / 255.0);
    int b = (int)(base_color.b * diffuse_component * light_color.b / 255.0);

    // Add specular component (white highlight)
    int specular_r = (int)(255.0 * specular_intensity * light_color.r / 255.0);
    int specular_g = (int)(255.0 * specular_intensity * light_color.g / 255.0);
    int specular_b = (int)(255.0 * specular_intensity * light_color.b / 255.0);

    // Combine diffuse and specular
    r += specular_r;
    g += specular_g;
    b += specular_b;

    // Clamp values to valid range
    r = r > 255 ? 255 : r;
    g = g > 255 ? 255 : g;
    b = b > 255 ? 255 : b;

    // Convert to integer
    return (r << 16) | (g << 8) | b;
}
