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

#include "platform.h"
#include <sys/time.h>
#include <string.h>


void	pixel_put(int x, int y, t_img *img, int color)
{
	int	offset;

	// Safety checks
	if (!img || !img->pixels_ptr || x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)
		return;

	offset = (y * img->line_len) + (x * (img->bpp / 8));

	// Make sure offset is valid
	if (offset < 0 || offset >= img->line_len * HEIGHT)
		return;

	*(unsigned int *)(img->pixels_ptr + offset) = color;
}


void	draw_image_to_window(t_scene *scene)
{
	mlx_put_image_to_window(scene->mlx_connection, scene->mlx_window,
		scene->img.img_ptr, 0, 0);
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
	// For 2D scenes, clear any bottom message
	if (!scene->is_3d) {
		display_progress(scene, ""); // Empty message clears previous message
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
