/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_pixel.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: parallels <parallels@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/13 20:09:32 by asplavni          #+#    #+#             */
/*   Updated: 2025/04/11 17:49:53 by parallels        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

static t_complex	get_mapped_complex(int x, int y, t_fractal *fractal)
{
	t_complex	z;
	t_bounds	bounds_x;
	t_bounds	bounds_y;

	bounds_x = (t_bounds){-2, +2, 0, WIDTH};
	bounds_y = (t_bounds){+2, -2, 0, HEIGHT};
	z.x = (map(x, bounds_x) * fractal->zoom) + fractal->shift_x;
	z.y = (map(y, bounds_y) * fractal->zoom) + fractal->shift_y;
	return (z);
}

static int	get_pixel_color(int i, t_fractal *fractal)
{
	t_bounds	color_bounds;

	color_bounds = (t_bounds){BLACK, WHITE,
		0, fractal->iterations_defintion};
	return (map(i, color_bounds));
}

static void	handle_fractal_iteration(t_complex *z,
		t_complex c, int *i, t_fractal *fractal)
{
	double	zx;
	double	zy;
	double	zx_sq;
	double	zy_sq;

	zx = z->x;
	zy = z->y;
	zx_sq = zx * zx;
	zy_sq = zy * zy;
	while (*i < fractal->iterations_defintion)
	{
		if (zx_sq + zy_sq > fractal->escape_value)
			break ;
		zy = 2 * zx * zy + c.y;
		zx = zx_sq - zy_sq + c.x;
		zx_sq = zx * zx;
		zy_sq = zy * zy;
		(*i)++;
	}
}

void	handle_pixel(int x, int y, t_fractal *fractal)
{
	t_complex	z;
	t_complex	c;
	int			i;
	int			color;

	// Safety check for null pointer
	if (!fractal || !fractal->img.pixels_ptr)
	{
		return;
	}

	// Safety check for coordinates
	if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
	{
		return;
	}

	i = 0;
	z = get_mapped_complex(x, y, fractal);
	mandelbrot_vs_julia(&z, &c, fractal);
	handle_fractal_iteration(&z, c, &i, fractal);
	
	if (i < fractal->iterations_defintion)
	{
		color = get_pixel_color(i, fractal);
	}
	else
	{
		color = BLACK;
	}
	
	// Write the pixel to the image buffer
	pixel_put(x, y, &fractal->img, color);
}
