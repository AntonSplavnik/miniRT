/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_calculations.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 21:20:31 by antonsplavn       #+#    #+#             */
/*   Updated: 2025/07/20 21:20:59 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/miniRT.h"

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
								double light_intensity, t_color_f light_color, \
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
