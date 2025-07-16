/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 13:07:03 by abillote          #+#    #+#             */
/*   Updated: 2025/07/16 03:14:32 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

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

t_color_f create_color_f(double r, double g, double b)
{
    t_color_f color;
    color.r = r;
    color.g = g;
    color.b = b;
    return color;
}

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
    c.r = (color >> 16) & 0xFF;
    c.g = (color >> 8) & 0xFF;
    c.b = color & 0xFF;
    return c;
}

double srgb_to_linear(double srgb)
{
    if (srgb <= 0.04045)
        return srgb / 12.92;
    return pow((srgb + 0.055) / 1.055, 2.4);
}

double linear_to_srgb(double linear)
{
    if (linear <= 0.0031308)
        return 12.92 * linear;
    return 1.055 * pow(linear, 1.0/2.4) - 0.055;
}

t_color_f color_to_linear(t_color color)
{
    t_color_f linear;
    linear.r = srgb_to_linear(color.r / 255.0);
    linear.g = srgb_to_linear(color.g / 255.0);
    linear.b = srgb_to_linear(color.b / 255.0);
    return linear;
}

t_vec3 color_f_to_vec3(t_color_f color)
{
    return vec3_create(color.r, color.g, color.b);
}

t_color_f vec3_to_color_f(t_vec3 v)
{
    return create_color_f(v.x, v.y, v.z);
}

int add_colors(int color1, int color2)
{
    int r1 = (color1 >> 16) & 0xFF;
    int g1 = (color1 >> 8) & 0xFF;
    int b1 = color1 & 0xFF;

    int r2 = (color2 >> 16) & 0xFF;
    int g2 = (color2 >> 8) & 0xFF;
    int b2 = color2 & 0xFF;

    int final_r = r1 + r2;
    int final_g = g1 + g2;
    int final_b = b1 + b2;

    return (valid_color_range(final_r) << 16) |
           (valid_color_range(final_g) << 8) |
           valid_color_range(final_b);
}

int blend_colors(int base_color, int blend_color, double base_weight, double blend_weight)
{
    int base_r = (base_color >> 16) & 0xFF;
    int base_g = (base_color >> 8) & 0xFF;
    int base_b = base_color & 0xFF;

    int blend_r = (blend_color >> 16) & 0xFF;
    int blend_g = (blend_color >> 8) & 0xFF;
    int blend_b = blend_color & 0xFF;

    int final_r = (int)(base_r * base_weight + blend_r * blend_weight);
    int final_g = (int)(base_g * base_weight + blend_g * blend_weight);
    int final_b = (int)(base_b * base_weight + blend_b * blend_weight);

    return (valid_color_range(final_r) << 16) |
           (valid_color_range(final_g) << 8) |
           valid_color_range(final_b);
}

/* Clamp color components to valid range */
t_color_f	clamp_color_components(t_color_f color)
{
	color.r = fmax(color.r, 0.0);
	color.g = fmax(color.g, 0.0);
	color.b = fmax(color.b, 0.0);
	return (color);
}

/* Apply saturation boost to enhance color vibrancy */
t_color_f	apply_saturation_boost(t_color_f color)
{
	double	max_component;
	double	saturation_boost;
	double	saturation_factor;

	max_component = fmax(color.r, fmax(color.g, color.b));
	if (max_component > 0.0)
	{
		saturation_boost = 1.05;
		saturation_factor = 1.0 + (saturation_boost - 1.0)
			* (1.0 - 1.0 / max_component);
		color.r = color.r * saturation_factor;
		color.g = color.g * saturation_factor;
		color.b = color.b * saturation_factor;
	}
	return (color);
}

/* Apply tone mapping using exponential function */
t_color_f	apply_tone_mapping(t_color_f color)
{
	t_color_f	result;

	result.r = 1.0 - exp(-color.r * 0.7);
	result.g = 1.0 - exp(-color.g * 0.7);
	result.b = 1.0 - exp(-color.b * 0.7);
	return (result);
}

/* Apply Reinhard tone mapping */
t_color_f	apply_reinhard_tone_mapping(t_color_f color)
{
	t_color_f	result;

	result.r = color.r / (1.0 + color.r);
	result.g = color.g / (1.0 + color.g);
	result.b = color.b / (1.0 + color.b);
	return (result);
}

/* Apply gamma correction using linear to sRGB conversion */
t_color_f	apply_gamma_correction(t_color_f color)
{
	t_color_f	result;

	result.r = linear_to_srgb(color.r);
	result.g = linear_to_srgb(color.g);
	result.b = linear_to_srgb(color.b);
	return (result);
}

/* Convert float color to integer with clamping and rounding */
int	convert_color_f_to_int(t_color_f color)
{
	int	ir;
	int	ig;
	int	ib;

	ir = (int)(fmin(fmax(color.r, 0.0), 1.0) * 255.0 + 0.5);
	ig = (int)(fmin(fmax(color.g, 0.0), 1.0) * 255.0 + 0.5);
	ib = (int)(fmin(fmax(color.b, 0.0), 1.0) * 255.0 + 0.5);
	return ((ir << 16) | (ig << 8) | ib);
}

/* Convert float color to display integer format */
int	color_f_to_display(t_color_f color)
{
	t_color_f	processed;

	processed = clamp_color_components(color);
	processed = apply_saturation_boost(processed);
	processed = apply_tone_mapping(processed);
	processed = apply_gamma_correction(processed);
	return (convert_color_f_to_int(processed));
}



/* Wrap UV coordinates to [0, 1] range */
void	wrap_uv_coordinates(double *u, double *v)
{
	*u = *u - floor(*u);
	*v = *v - floor(*v);
	if (*u < 0)
		*u += 1.0;
	if (*v < 0)
		*v += 1.0;
}

/* Clamp texture coordinates to valid bounds */
void	clamp_texture_coords(int *x, int *y, t_texture *texture)
{
	if (*x < 0)
		*x = 0;
	if (*x >= texture->width)
		*x = texture->width - 1;
	if (*y < 0)
		*y = 0;
	if (*y >= texture->height)
		*y = texture->height - 1;
}

/* Sample texture at UV coordinates */
t_color	sample_texture(t_texture *texture, double u, double v)
{
	t_color	color;
	int		x;
	int		y;
	int		index;

	color = create_color(255, 255, 255);
	if (!texture || !texture->data)
		return (color);
	wrap_uv_coordinates(&u, &v);
	x = (int)(u * (texture->width - 1));
	y = (int)(v * (texture->height - 1));
	clamp_texture_coords(&x, &y, texture);
	index = (y * texture->width + x) * 4;
	color.r = texture->data[index + 0];
	color.g = texture->data[index + 1];
	color.b = texture->data[index + 2];
	return (color);
}

/* Get surface color with texture support */
t_color	get_surface_color_with_texture(t_hit_record *hit_record)
{
	t_material	*material;

	material = &hit_record->object->material;
	if (material->has_texture && material->texture)
	{
		return (sample_texture(material->texture,
				hit_record->uv.u, hit_record->uv.v));
	}
	if (material->has_checker)
	{
		return (get_checker_color(*material,
				hit_record->object, hit_record->point));
	}
	return (material->color);
}

t_color_f sample_texture_linear(t_texture *texture, double u, double v)
{
    t_color srgb_color = sample_texture(texture, u, v);
    return color_to_linear(srgb_color);
}

/* Get surface color in linear space */
t_color_f	get_surface_color_linear(t_hit_record *hit_record)
{
	t_material	*material;
	t_color		srgb_color;

	material = &hit_record->object->material;
	if (material->has_texture && material->texture)
	{
		return (sample_texture_linear(material->texture,
				hit_record->uv.u, hit_record->uv.v));
	}
	if (material->has_checker)
	{
		srgb_color = get_checker_color(*material,
				hit_record->object, hit_record->point);
	}
	else
	{
		srgb_color = material->color;
	}
	return (color_to_linear(srgb_color));
}

/* Calculate diffuse lighting component */
t_color_f	calculate_diffuse_lighting(t_color_f base_color,
								double diffuse_component, t_color_f light_color)
{
	t_color_f	result;

	result.r = base_color.r * diffuse_component * light_color.r;
	result.g = base_color.g * diffuse_component * light_color.g;
	result.b = base_color.b * diffuse_component * light_color.b;
	return (result);
}

/* Calculate specular lighting component */
t_color_f	calculate_specular_lighting(double specular_intensity,
								t_color_f light_color)
{
	t_color_f	result;

	result.r = specular_intensity * light_color.r;
	result.g = specular_intensity * light_color.g;
	result.b = specular_intensity * light_color.b;
	return (result);
}

/* Add two color components together */
t_color_f	add_color_components(t_color_f color1, t_color_f color2)
{
	t_color_f	result;

	result.r = color1.r + color2.r;
	result.g = color1.g + color2.g;
	result.b = color1.b + color2.b;
	return (result);
}

/* Calculate pixel color in linear space */
t_color_f	get_pixel_color_linear(t_hit_record *hit_record,
								double light_intensity, t_color_f light_color,
								double specular_intensity)
{
	t_color_f	base_color;
	t_color_f	diffuse;
	t_color_f	specular;
	double		diffuse_component;

	base_color = get_surface_color_linear(hit_record);
	diffuse_component = light_intensity - specular_intensity;
	if (diffuse_component < 0)
		diffuse_component = 0;
	diffuse = calculate_diffuse_lighting(base_color,
			diffuse_component, light_color);
	specular = calculate_specular_lighting(specular_intensity, light_color);
	return (add_color_components(diffuse, specular));
}


