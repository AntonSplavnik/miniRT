/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_fractal_progressive.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: parallels <parallels@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 14:44:42 by asplavni          #+#    #+#             */
/*   Updated: 2025/04/11 17:49:42 by parallels        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

void	fractal_render_progressive(t_fractal *fractal)
{
	int	step;
	int	x;
	int	y;

	step = 4;
	while (step > 0)
	{
		y = 0;
		while (y < HEIGHT)
		{
			x = 0;
			while (x < WIDTH)
			{
				handle_pixel(x, y, fractal);
				x += step;
			}
			y += step;
		}
		draw_image_to_window(fractal);
		step /= 2;
	}
}
