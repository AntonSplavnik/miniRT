/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 13:07:03 by abillote          #+#    #+#             */
/*   Updated: 2025/06/20 12:32:34 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

// Sample texture at UV coordinates - works with MLX-loaded textures
t_color sample_texture(t_texture *texture, double u, double v)
{
    t_color color = {255, 255, 255}; // Default white

    if (!texture || !texture->data)
        return color;

    // Wrap UV coordinates to [0, 1] range
    u = u - floor(u);
    v = v - floor(v);
    if (u < 0) u += 1.0;
    if (v < 0) v += 1.0;

    // Convert UV to pixel coordinates
    int x = (int)(u * (texture->width - 1));
    int y = (int)(v * (texture->height - 1));

    // Clamp to texture bounds (safety check)
    if (x < 0) x = 0;
    if (x >= texture->width) x = texture->width - 1;
    if (y < 0) y = 0;
    if (y >= texture->height) y = texture->height - 1;

    // Calculate pixel index
    // MLX textures are typically RGBA (4 channels)
    int index = (y * texture->width + x) * 4;

    color.r = texture->data[index + 0]; // Red
    color.g = texture->data[index + 1]; // Green
    color.b = texture->data[index + 2]; // Blue

    return color;
}

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
t_color int_to_color(int color)
{
    t_color c;
    c.r = (color >> 16) & 0xFF;  // Keep in [0, 255] range
    c.g = (color >> 8) & 0xFF;
    c.b = color & 0xFF;
    return c;
}
// Updated color sampling function with texture support
t_color get_surface_color_with_texture(t_hit_record *hit_record)
{
    t_material *material = &hit_record->object->material;

    // If the material has a texture, sample it
    if (material->has_texture && material->texture)
    {
        return sample_texture(material->texture, hit_record->uv.u, hit_record->uv.v);
    }

    // Fall back to existing color logic (checkerboard or solid color)
    if (material->has_checker)
    {
        return get_checker_color(*material, hit_record->object, hit_record->point);
    }

    return material->color;
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
int get_pixel_color(t_hit_record *hit_record, double light_intensity,
                               t_color light_color,
                               double specular_intensity)
{
    // Get the base color from the material, considering checkerboard pattern
    t_color base_color = get_surface_color_with_texture(hit_record);

    // Convert from sRGB to linear space for physically accurate lighting
    float base_r_linear = base_color.r <= 10 ? base_color.r / 255.0f / 12.92f :
                         powf((base_color.r / 255.0f + 0.055f) / 1.055f, 2.4f);
    float base_g_linear = base_color.g <= 10 ? base_color.g / 255.0f / 12.92f :
                         powf((base_color.g / 255.0f + 0.055f) / 1.055f, 2.4f);
    float base_b_linear = base_color.b <= 10 ? base_color.b / 255.0f / 12.92f :
                         powf((base_color.b / 255.0f + 0.055f) / 1.055f, 2.4f);

    // Convert light color to linear space
    float light_r_linear = light_color.r <= 10 ? light_color.r / 255.0f / 12.92f :
                          powf((light_color.r / 255.0f + 0.055f) / 1.055f, 2.4f);
    float light_g_linear = light_color.g <= 10 ? light_color.g / 255.0f / 12.92f :
                          powf((light_color.g / 255.0f + 0.055f) / 1.055f, 2.4f);
    float light_b_linear = light_color.b <= 10 ? light_color.b / 255.0f / 12.92f :
                          powf((light_color.b / 255.0f + 0.055f) / 1.055f, 2.4f);

    // Calculate diffuse component (subtract specular from total light)
    double diffuse_component = light_intensity - specular_intensity;
    if (diffuse_component < 0)
        diffuse_component = 0;

    // Apply diffuse lighting calculations in linear space
    float r_linear = base_r_linear * diffuse_component * light_r_linear;
    float g_linear = base_g_linear * diffuse_component * light_g_linear;
    float b_linear = base_b_linear * diffuse_component * light_b_linear;

    // Add specular component (white highlight) in linear space
    float specular_r_linear = specular_intensity * light_r_linear;
    float specular_g_linear = specular_intensity * light_g_linear;
    float specular_b_linear = specular_intensity * light_b_linear;

    // Combine diffuse and specular in linear space
    r_linear += specular_r_linear;
    g_linear += specular_g_linear;
    b_linear += specular_b_linear;

    // Clamp to valid range
    r_linear = r_linear > 0.0f ? r_linear : 0.0f;
    g_linear = g_linear > 0.0f ? g_linear : 0.0f;
    b_linear = b_linear > 0.0f ? b_linear : 0.0f;

    // --- Reinhard tone mapping ---
    r_linear = r_linear / (r_linear + 1.0f);
    g_linear = g_linear / (g_linear + 1.0f);
    b_linear = b_linear / (b_linear + 1.0f);
    // ----------------------------

    float exposure = 1.2f; // try values from 1.0 to 2.0 or more
    r_linear = exposure * r_linear / (exposure * r_linear + 1.0f);
    g_linear = exposure * g_linear / (exposure * g_linear + 1.0f);
    b_linear = exposure * b_linear / (exposure * b_linear + 1.0f);

    // Convert back to sRGB space for output
    int r = (int)(r_linear <= 0.0031308f ? 12.92f * r_linear * 255.0f :
                  (1.055f * powf(r_linear, 1.0f/2.4f) - 0.055f) * 255.0f);
    int g = (int)(g_linear <= 0.0031308f ? 12.92f * g_linear * 255.0f :
                  (1.055f * powf(g_linear, 1.0f/2.4f) - 0.055f) * 255.0f);
    int b = (int)(b_linear <= 0.0031308f ? 12.92f * b_linear * 255.0f :
                  (1.055f * powf(b_linear, 1.0f/2.4f) - 0.055f) * 255.0f);

    // Clamp final values
    r = r > 255 ? 255 : (r < 0 ? 0 : r);
    g = g > 255 ? 255 : (g < 0 ? 0 : g);
    b = b > 255 ? 255 : (b < 0 ? 0 : b);

    // Convert to integer
    return (r << 16) | (g << 8) | b;
}




// // Sample texture at UV coordinates - works with MLX-loaded textures
// t_color sample_texture(t_texture *texture, double u, double v)
// {
//     t_color color = {255, 255, 255}; // Default white

//     if (!texture || !texture->data)
//         return color;

//     // Wrap UV coordinates to [0, 1] range
//     u = u - floor(u);
//     v = v - floor(v);
//     if (u < 0) u += 1.0;
//     if (v < 0) v += 1.0;

//     // Convert UV to pixel coordinates
//     int x = (int)(u * (texture->width - 1));
//     int y = (int)(v * (texture->height - 1));

//     // Clamp to texture bounds (safety check)
//     if (x < 0) x = 0;
//     if (x >= texture->width) x = texture->width - 1;
//     if (y < 0) y = 0;
//     if (y >= texture->height) y = texture->height - 1;

//     // Calculate pixel index
//     // MLX textures are typically RGBA (4 channels)
//     int index = (y * texture->width + x) * 4;

//     color.r = texture->data[index + 0]; // Red
//     color.g = texture->data[index + 1]; // Green
//     color.b = texture->data[index + 2]; // Blue

//     return color;
// }

// int	valid_color_range(int c)
// {
// 	if (c < 0)
// 		return 0;
// 	else if (c > 255)
// 		return 255;
// 	else
// 		return (c);
// }

// t_color	create_color(int r, int g, int b)
// {
// 	t_color color;

// 	color.r = valid_color_range(r);
// 	color.g = valid_color_range(g);
// 	color.b = valid_color_range(b);
// 	return (color);
// }

// /**
//  * Convert t_color to an integer RGB value
//  *
//  * @param color The color structure
//  * @return Integer representation (0xRRGGBB)
//  */
// int color_to_int(t_color color)
// {
// 	int r = (int)color.r;
// 	int g = (int)color.g;
// 	int b = (int)color.b;

// 	return (r << 16) | (g << 8) | b;
// }
// t_color int_to_color(int color)
// {
//     t_color c;
//     c.r = (color >> 16) & 0xFF;  // Keep in [0, 255] range
//     c.g = (color >> 8) & 0xFF;
//     c.b = color & 0xFF;
//     return c;
// }
// // Updated color sampling function with texture support
// t_color get_surface_color_with_texture(t_hit_record *hit_record)
// {
//     t_material *material = &hit_record->object->material;

//     // If the material has a texture, sample it
//     if (material->has_texture && material->texture)
//     {
//         return sample_texture(material->texture, hit_record->uv.u, hit_record->uv.v);
//     }

//     // Fall back to existing color logic (checkerboard or solid color)
//     if (material->has_checker)
//     {
//         return get_checker_color(*material, hit_record->object, hit_record->point);
//     }

//     return material->color;
// }

// /**
//  * Get pixel color for a material at a specific point with lighting applied
//  * Supports checkerboard textures
//  *
//  * @param hit_object The object that was hit
//  * @param light_intensity The amount of light at the point
//  * @param light_color The color of the light
//  * @param hit_point The point of intersection
//  * @return The final pixel color
//  */
// int get_pixel_color(t_hit_record *hit_record, double light_intensity,
//                                t_color light_color,
//                                double specular_intensity)
// {
//     // Get the base color from the material, considering checkerboard pattern
//     t_color base_color = get_surface_color_with_texture(hit_record);

//     // Calculate diffuse component (subtract specular from total light)
//     double diffuse_component = light_intensity - specular_intensity;
//     if (diffuse_component < 0)
//         diffuse_component = 0;

//     // Apply diffuse lighting calculations
//     int r = (int)(base_color.r * diffuse_component * light_color.r / 255.0);
//     int g = (int)(base_color.g * diffuse_component * light_color.g / 255.0);
//     int b = (int)(base_color.b * diffuse_component * light_color.b / 255.0);

//     // Add specular component (white highlight)
//     int specular_r = (int)(255.0 * specular_intensity * light_color.r / 255.0);
//     int specular_g = (int)(255.0 * specular_intensity * light_color.g / 255.0);
//     int specular_b = (int)(255.0 * specular_intensity * light_color.b / 255.0);

//     // Combine diffuse and specular
//     r += specular_r;
//     g += specular_g;
//     b += specular_b;

//     // Clamp values to valid range
//     r = r > 255 ? 255 : r;
//     g = g > 255 ? 255 : g;
//     b = b > 255 ? 255 : b;

//     // Convert to integer
//     return (r << 16) | (g << 8) | b;
// }

/**
 * Blend two colors with specified weights
 *
 * @param base_color The base color (integer RGB format)
 * @param blend_color The color to blend with (integer RGB format)
 * @param base_weight Weight for the base color (0.0 to 1.0)
 * @param blend_weight Weight for the blend color (0.0 to 1.0)
 * @return Blended color in integer RGB format
 */
int blend_colors(int base_color, int blend_color, double base_weight, double blend_weight)
{
    // Extract RGB components from base color
    int base_r = (base_color >> 16) & 0xFF;
    int base_g = (base_color >> 8) & 0xFF;
    int base_b = base_color & 0xFF;

    // Extract RGB components from blend color
    int blend_r = (blend_color >> 16) & 0xFF;
    int blend_g = (blend_color >> 8) & 0xFF;
    int blend_b = blend_color & 0xFF;

    // Blend the colors
    int final_r = (int)(base_r * base_weight + blend_r * blend_weight);
    int final_g = (int)(base_g * base_weight + blend_g * blend_weight);
    int final_b = (int)(base_b * base_weight + blend_b * blend_weight);

    // Clamp to valid range and return
    return (valid_color_range(final_r) << 16) |
           (valid_color_range(final_g) << 8) |
           valid_color_range(final_b);
}

/**
 * Add two colors together (useful for accumulating light contributions)
 *
 * @param color1 First color (integer RGB format)
 * @param color2 Second color (integer RGB format)
 * @return Sum of the colors in integer RGB format
 */
int add_colors(int color1, int color2)
{
    // Extract RGB components
    int r1 = (color1 >> 16) & 0xFF;
    int g1 = (color1 >> 8) & 0xFF;
    int b1 = color1 & 0xFF;

    int r2 = (color2 >> 16) & 0xFF;
    int g2 = (color2 >> 8) & 0xFF;
    int b2 = color2 & 0xFF;

    // Add components
    int final_r = r1 + r2;
    int final_g = g1 + g2;
    int final_b = b1 + b2;

    // Clamp to valid range and return
    return (valid_color_range(final_r) << 16) |
           (valid_color_range(final_g) << 8) |
           valid_color_range(final_b);
}

// Convert sRGB to linear
double srgb_to_linear(double srgb)
{
    if (srgb <= 0.04045)
        return srgb / 12.92;
    return pow((srgb + 0.055) / 1.055, 2.4);
}

// Convert linear to sRGB
double linear_to_srgb(double linear)
{
    if (linear <= 0.0031308)
        return 12.92 * linear;
    return 1.055 * pow(linear, 1.0/2.4) - 0.055;
}

// Create floating-point color
t_color_f create_color_f(double r, double g, double b)
{
    t_color_f color;
    color.r = r;
    color.g = g;
    color.b = b;
    return color;
}

// Convert integer color to linear floating-point
t_color_f color_to_linear(t_color color)
{
    t_color_f linear;
    linear.r = srgb_to_linear(color.r / 255.0);
    linear.g = srgb_to_linear(color.g / 255.0);
    linear.b = srgb_to_linear(color.b / 255.0);
    return linear;
}

// Convert floating-point color to display integer with tone mapping and gamma
int color_f_to_display(t_color_f color)
{
    // Apply tone mapping (Reinhard)
    double exposure = 1.2;
    double r = exposure * color.r / (exposure * color.r + 1.0);
    double g = exposure * color.g / (exposure * color.g + 1.0);
    double b = exposure * color.b / (exposure * color.b + 1.0);

    // Apply gamma correction
    r = linear_to_srgb(r);
    g = linear_to_srgb(g);
    b = linear_to_srgb(b);

    // Clamp and convert to integer
    int ir = (int)(r * 255.0 + 0.5);
    int ig = (int)(g * 255.0 + 0.5);
    int ib = (int)(b * 255.0 + 0.5);

    ir = ir > 255 ? 255 : (ir < 0 ? 0 : ir);
    ig = ig > 255 ? 255 : (ig < 0 ? 0 : ig);
    ib = ib > 255 ? 255 : (ib < 0 ? 0 : ib);

    return (ir << 16) | (ig << 8) | ib;
}

// Utility conversions
t_vec3 color_f_to_vec3(t_color_f color)
{
    return vec3_create(color.r, color.g, color.b);
}

t_color_f vec3_to_color_f(t_vec3 v)
{
    return create_color_f(v.x, v.y, v.z);
}

// Sample texture in linear space
t_color_f sample_texture_linear(t_texture *texture, double u, double v)
{
    t_color srgb_color = sample_texture(texture, u, v);  // Your existing function
    return color_to_linear(srgb_color);
}

// Get surface color in linear space
t_color_f get_surface_color_linear(t_hit_record *hit_record)
{
    t_material *material = &hit_record->object->material;

    // If the material has a texture, sample it
    if (material->has_texture && material->texture)
    {
        return sample_texture_linear(material->texture, hit_record->uv.u, hit_record->uv.v);
    }

    // Fall back to existing color logic (checkerboard or solid color)
    t_color srgb_color;
    if (material->has_checker)
    {
        srgb_color = get_checker_color(*material, hit_record->object, hit_record->point);
    }
    else
    {
        srgb_color = material->color;
    }

    return color_to_linear(srgb_color);
}

// Updated pixel color calculation in linear space
t_color_f get_pixel_color_linear(t_hit_record *hit_record, double light_intensity,
                                 t_color_f light_color, double specular_intensity)
{
    // Get the base color in linear space
    t_color_f base_color = get_surface_color_linear(hit_record);

    // Calculate diffuse component
    double diffuse_component = light_intensity - specular_intensity;
    if (diffuse_component < 0)
        diffuse_component = 0;

    // Apply diffuse lighting in linear space
    t_color_f result;
    result.r = base_color.r * diffuse_component * light_color.r;
    result.g = base_color.g * diffuse_component * light_color.g;
    result.b = base_color.b * diffuse_component * light_color.b;

    // Add specular component
    result.r += specular_intensity * light_color.r;
    result.g += specular_intensity * light_color.g;
    result.b += specular_intensity * light_color.b;

    return result;
}


