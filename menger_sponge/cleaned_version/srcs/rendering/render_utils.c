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

// Function to display progress visually on the window
void display_progress(t_scene *scene, const char *status_text)
{
	   static mlx_image_t *text_img = NULL;
    
    // Only proceed if MLX is properly initialized
    if (!scene || !scene->app.mlx)
        return;
    
    // Delete previous text image if it exists
    if (text_img != NULL) {
        mlx_delete_image(scene->app.mlx, text_img);
        text_img = NULL;
    }
    
    // Special case: empty message means clear any previous message
    if (status_text[0] == '\0') {
        return;
    }
    
    // Create new text image with the status message
    // MLX42 uses white text by default with black outline
    text_img = mlx_put_string(scene->app.mlx, (char *)status_text, 10, HEIGHT - 25);
    
    // If we want to change the color (optional)
    if (text_img && ft_strncmp((char *)status_text, "Rendering...", 12) == 0) {
        // Change text color to yellow for rendering message
        for (uint32_t i = 0; i < text_img->width * text_img->height; i++) {
            uint8_t *pixel = &text_img->pixels[i * 4];
            if (pixel[3] != 0) { // If pixel is not transparent
                pixel[0] = 255; // R
                pixel[1] = 255; // G
                pixel[2] = 0;   // B
                pixel[3] = 255; // A
            }
        }
    }
}
