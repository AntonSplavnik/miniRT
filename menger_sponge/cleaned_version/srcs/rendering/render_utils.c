/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 13:17:50 by asplavni          #+#    #+#             */
/*   Updated: 2025/04/26 08:41:53 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

void	pixel_put(int x, int y, mlx_image_t *img, int color)
{
    // Safety checks
    if (!img || x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
        return;

    // Convert RGB to RGBA (MLX42 uses RGBA format)
    uint8_t r = (color >> 16) & 0xFF;
    uint8_t g = (color >> 8) & 0xFF;
    uint8_t b = color & 0xFF;
    uint32_t rgba = (r << 24) | (g << 16) | (b << 8) | 0xFF; // Full alpha
    
    // MLX42's put_pixel handles bounds checking internally
    mlx_put_pixel(img, x, y, rgba);
}

