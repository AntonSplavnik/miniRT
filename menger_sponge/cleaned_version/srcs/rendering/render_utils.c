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

#include "miniRT.h"
#include <sys/time.h>
#include <string.h>


void pixel_put(int x, int y, mlx_image_t *img, int color)
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

void	scene_render(t_scene *scene)
{
	// Display progress at start of rendering
	display_progress(scene, "Rendering...");

	// Check if the scene is 3D (Menger sponge)
	if (scene->is_3d && !ft_strncmp(scene->name, "menger", 6))
	{
		// Use the dedicated Menger sponge renderer
		render_menger_sponge(scene);
		return; // Exit early to prevent any 2D rendering
	}
}


// Function to display progress visually on the window
void display_progress(t_scene *scene, const char *status_text)
{
	// Only proceed if MLX is properly initialized
	if (!scene || !scene->mlx_connection || !scene->mlx_window)
		return;

	// Special case: empty message means clear any previous message
	if (status_text[0] == '\0') {
		mlx_string_put(scene->mlx_connection, scene->mlx_window,
					10, HEIGHT - 15, 0x000000, "                                   ");
		return;
	}

	// For Menger sponge, keep displaying messages
	if (scene->is_3d && !ft_strncmp(scene->name, "menger", 6))
	{
		// Display the text with a bright color (yellow text)
		mlx_string_put(scene->mlx_connection, scene->mlx_window,
					10, HEIGHT - 15, 0xFFFF00, (char *)status_text);
	}
	else if (ft_strncmp((char *)status_text, "Rendering...", 12) == 0)
	{
		// For 2D scenes, only show "Rendering..." message
		// Display the text with a bright color (yellow text)
		mlx_string_put(scene->mlx_connection, scene->mlx_window,
					10, HEIGHT - 15, 0xFFFF00, (char *)status_text);
	}
	// For 2D scenes, don't show "Rendering complete" message

	// Force immediate processing of the window update
	mlx_do_sync(scene->mlx_connection);
}
