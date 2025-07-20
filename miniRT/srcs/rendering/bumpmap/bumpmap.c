/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bumpmap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 01:59:12 by antonsplavn       #+#    #+#             */
/*   Updated: 2025/07/20 22:41:52 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

static void	wrap_uv_coordinates(double *u, double *v)
{
	*u = *u - floor(*u);
	*v = *v - floor(*v);
	if (*u < 0)
		*u += 1.0;
	if (*v < 0)
		*v += 1.0;
}

static void	get_pixel_coords(t_bump_map *bump_map, double u, double v,
							t_pixel_coords *coords)
{
	coords->px = u * (bump_map->width - 1);
	coords->py = v * (bump_map->height - 1);
	coords->x0 = (int)floor(coords->px);
	coords->y0 = (int)floor(coords->py);
	coords->x1 = coords->x0 + 1;
}

static void	clamp_coordinates(t_bump_map *bump_map, t_pixel_coords *coords)
{
	if (coords->x0 < 0)
		coords->x0 = 0;
	if (coords->x0 >= bump_map->width)
		coords->x0 = bump_map->width - 1;
	if (coords->y0 < 0)
		coords->y0 = 0;
	if (coords->y0 >= bump_map->height)
		coords->y0 = bump_map->height - 1;
	if (coords->x1 < 0)
		coords->x1 = 0;
	if (coords->x1 >= bump_map->width)
		coords->x1 = bump_map->width - 1;
}

static float	bilinear_interpolation(t_bump_map *bump_map,
									t_pixel_coords *coords)
{
	float	h00;
	float	h10;
	float	h01;
	float	h11;
	double	fx;

	fx = coords->px - coords->x0;
	h00 = bump_map->elevation[coords->y0 * bump_map->width + coords->x0];
	h10 = bump_map->elevation[coords->y0 * bump_map->width + coords->x1];
	h01 = bump_map->elevation[(coords->y0 + 1) * bump_map->width + coords->x0];
	h11 = bump_map->elevation[(coords->y0 + 1) * bump_map->width + coords->x1];
	return ((h00 * (1 - fx) + h10 * fx) * (1 - (coords->py - coords->y0))
		+ (h01 * (1 - fx) + h11 * fx) * (coords->py - coords->y0));
}

float	sample_bump_map(t_bump_map *bump_map, double u, double v)
{
	t_pixel_coords	coords;

	if (!bump_map || !bump_map->elevation)
		return (0.0f);
	wrap_uv_coordinates(&u, &v);
	get_pixel_coords(bump_map, u, v, &coords);
	clamp_coordinates(bump_map, &coords);
	return (bilinear_interpolation(bump_map, &coords));
}
