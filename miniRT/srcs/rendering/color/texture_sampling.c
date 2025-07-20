/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_sampling.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 21:23:25 by antonsplavn       #+#    #+#             */
/*   Updated: 2025/07/20 21:23:29 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/miniRT.h"

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

t_color_f	sample_texture_linear(t_texture *texture, double u, double v)
{
	t_color	srgb_color;

	srgb_color = sample_texture(texture, u, v);
	return (color_to_linear(srgb_color));
}
