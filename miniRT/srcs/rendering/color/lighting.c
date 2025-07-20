/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lighting.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 21:23:46 by antonsplavn       #+#    #+#             */
/*   Updated: 2025/07/20 21:24:21 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/miniRT.h"

t_vec3	color_f_to_vec3(t_color_f color)
{
	return (vec3_create(color.r, color.g, color.b));
}

t_color_f	vec3_to_color_f(t_vec3 v)
{
	return (create_color_f(v.x, v.y, v.z));
}

int	add_colors(int color1, int color2)
{
	int	r1;
	int	g1;
	int	b1;
	int	r2;
	int	g2;

	r1 = (color1 >> 16) & 0xFF;
	g1 = (color1 >> 8) & 0xFF;
	b1 = color1 & 0xFF;
	r2 = (color2 >> 16) & 0xFF;
	g2 = (color2 >> 8) & 0xFF;
	return ((valid_color_range(r1 + r2) << 16) \
		| (valid_color_range(g1 + g2) << 8) \
		| valid_color_range(b1 + (color2 & 0xFF)));
}

int	blend_colors(int base_color, int blend_color,
		double base_weight, double blend_weight)
{
	int	base_r;
	int	base_g;
	int	base_b;
	int	blend_r;
	int	blend_g;

	base_r = (base_color >> 16) & 0xFF;
	base_g = (base_color >> 8) & 0xFF;
	base_b = base_color & 0xFF;
	blend_r = (blend_color >> 16) & 0xFF;
	blend_g = (blend_color >> 8) & 0xFF;
	return ((valid_color_range((int)(base_r * base_weight \
					+ blend_r * blend_weight)) << 16) \
		| (valid_color_range((int)(base_g * base_weight \
					+ blend_g * blend_weight)) << 8) \
		| valid_color_range((int)(base_b * base_weight \
				+ (blend_color & 0xFF) * blend_weight)));
}
