/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hdr_processing.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 21:21:24 by antonsplavn       #+#    #+#             */
/*   Updated: 2025/07/20 21:21:29 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/miniRT.h"

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
	processed = apply_reinhard_tone_mapping(processed);
	processed = apply_gamma_correction(processed);
	return (convert_color_f_to_int(processed));
}
