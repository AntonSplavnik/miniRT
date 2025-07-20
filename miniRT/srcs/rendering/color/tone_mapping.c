/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tone_mapping.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 21:23:13 by antonsplavn       #+#    #+#             */
/*   Updated: 2025/07/20 21:23:18 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/miniRT.h"

double	srgb_to_linear(double srgb)
{
	if (srgb <= 0.04045)
		return (srgb / 12.92);
	return (pow((srgb + 0.055) / 1.055, 2.4));
}

double	linear_to_srgb(double linear)
{
	if (linear <= 0.0031308)
		return (12.92 * linear);
	return (1.055 * pow(linear, 1.0 / 2.4) - 0.055);
}

t_color_f	color_to_linear(t_color color)
{
	t_color_f	linear;

	linear.r = srgb_to_linear(color.r / 255.0);
	linear.g = srgb_to_linear(color.g / 255.0);
	linear.b = srgb_to_linear(color.b / 255.0);
	return (linear);
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
