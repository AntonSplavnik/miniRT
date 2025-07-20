/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   color_conversion.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 21:21:47 by antonsplavn       #+#    #+#             */
/*   Updated: 2025/07/20 21:21:57 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/miniRT.h"

int	valid_color_range(int c)
{
	if (c < 0)
		return (0);
	else if (c > 255)
		return (255);
	else
		return (c);
}

t_color	create_color(int r, int g, int b)
{
	t_color	color;

	color.r = valid_color_range(r);
	color.g = valid_color_range(g);
	color.b = valid_color_range(b);
	return (color);
}

t_color_f	create_color_f(double r, double g, double b)
{
	t_color_f	color;

	color.r = r;
	color.g = g;
	color.b = b;
	return (color);
}

int	color_to_int(t_color color)
{
	int	r;
	int	g;
	int	b;

	r = (int)color.r;
	g = (int)color.g;
	b = (int)color.b;
	return ((r << 16) | (g << 8) | b);
}

t_color	int_to_color(int color)
{
	t_color	c;

	c.r = (color >> 16) & 0xFF;
	c.g = (color >> 8) & 0xFF;
	c.b = color & 0xFF;
	return (c);
}
