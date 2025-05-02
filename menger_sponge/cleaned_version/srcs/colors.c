/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 13:07:03 by abillote          #+#    #+#             */
/*   Updated: 2025/05/02 13:14:34 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "platform.h"

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

//to improve to add all lighting factors
int	get_final_color(t_scene *scene, double light_intensity)
{
	t_color	object_color;
	int		r;
	int		g;
	int		b;

	object_color = scene->objects->material.color;
	light_intensity = fmax(scene->ambient.ratio, light_intensity);
	r = valid_color_range((int)(object_color.r *light_intensity));
	g = valid_color_range((int)(object_color.g * light_intensity));
	b = valid_color_range((int)(object_color.b *light_intensity));
	return ((r << 16) | (g << 8) | b);
}
