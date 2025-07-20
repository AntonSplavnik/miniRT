/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 01:41:53 by antonsplavn       #+#    #+#             */
/*   Updated: 2025/07/21 01:42:51 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

void	pixel_put(int x, int y, mlx_image_t *img, int color)
{
	uint8_t		r;
	uint8_t		g;
	uint8_t		b;
	uint32_t	rgba;

	if (!img || x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
		return ;
	r = (color >> 16) & 0xFF;
	g = (color >> 8) & 0xFF;
	b = color & 0xFF;
	rgba = (r << 24) | (g << 16) | (b << 8) | 0xFF;
	mlx_put_pixel(img, x, y, rgba);
}
