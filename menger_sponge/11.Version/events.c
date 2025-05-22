/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   events.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asplavni <asplavni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 13:08:43 by asplavni          #+#    #+#             */
/*   Updated: 2024/10/14 14:16:25 by asplavni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "platform.h"
#include <stdio.h>

// Helper function to print status messages
void display_status(t_fractal *fractal)
{
	char status[100];
	
	// Format status text based on fractal type
	if (fractal->is_3d)
	{
		// 3D mode status
		snprintf(status, 100, "3D Mode | Iterations: %d | Resolution: %d", 
				fractal->menger.iterations, fractal->resolution_factor);
	}
	else
	{
		// 2D mode status
		snprintf(status, 100, "Fractal: %s | Zoom: %.2f | Iterations: %d", 
				fractal->name, fractal->zoom, fractal->iterations_defintion);
	}
	
	// Clear the window and display the status with a new image
	if (fractal->mlx_connection && fractal->mlx_window)
	{
		// Create a status bar at the top using a separate string display
		// Draw a black rectangle at coordinates (0,0) with width WIDTH and height 30
		mlx_string_put(fractal->mlx_connection, fractal->mlx_window, 
					10, 20, 0xFFFF00, status);
	}
}

int	close_handler(t_fractal *fractal)
{
	static int	freed = 0;

	if (freed)
		return (0);
	freed = 1;
	
	// Free BVH for Menger sponge if it exists
	if (!ft_strncmp(fractal->name, "menger", 6) && fractal->menger.bvh_root)
	{
		free_bvh(fractal->menger.bvh_root);
		fractal->menger.bvh_root = NULL;
	}
	
	// Clear all other resources
	if (fractal->mlx_window && fractal->mlx_connection)
		mlx_destroy_window(fractal->mlx_connection, fractal->mlx_window);
	
	if (fractal->img.img_ptr && fractal->mlx_connection)
		mlx_destroy_image(fractal->mlx_connection, fractal->img.img_ptr);
	
	// Free the MLX pointer too
	if (fractal->mlx_connection)
	{
#ifdef __linux__
		mlx_destroy_display(fractal->mlx_connection);
#endif
		free(fractal->mlx_connection);
	}
	
	// Exit the program cleanly
	return (0);
}

int	key_handler(int keysym, t_fractal *fractal)
{
#ifdef __APPLE__
	if (keysym == KEY_ESC)
#else
	if (keysym == XK_Escape)
#endif
	{
		// Call close_handler to clean up resources
		close_handler(fractal);
		// Exit directly since we can't end the mlx loop gracefully
		exit(EXIT_SUCCESS);
	}
	
	if (!ft_strncmp(fractal->name, "menger", 6))
	{
		int camera_changed = 0;
		// Camera movement - slower to be more precise
#ifdef __APPLE__
		if (keysym == KEY_W)
		{
			fractal->camera.position.z += 0.2;
			camera_changed = 1;
		}
		else if (keysym == KEY_S)
		{
			fractal->camera.position.z -= 0.2;
			camera_changed = 1;
		}
		else if (keysym == KEY_A)
		{
			fractal->camera.position.x -= 0.2;
			camera_changed = 1;
		}
		else if (keysym == KEY_D)
		{
			fractal->camera.position.x += 0.2;
			camera_changed = 1;
		}
		else if (keysym == KEY_Q)
		{
			fractal->camera.position.y += 0.2;
			camera_changed = 1;
		}
		else if (keysym == KEY_E)
		{
			fractal->camera.position.y -= 0.2;
			camera_changed = 1;
		}
		else if (keysym == KEY_LEFT)
		{
			fractal->camera.rotation.y -= 0.05;
			camera_changed = 1;
		}
		else if (keysym == KEY_RIGHT)
		{
			fractal->camera.rotation.y += 0.05;
			camera_changed = 1;
		}
		else if (keysym == KEY_UP)
		{
			fractal->camera.rotation.x -= 0.05;
			camera_changed = 1;
		}
		else if (keysym == KEY_DOWN)
		{
			fractal->camera.rotation.x += 0.05;
			camera_changed = 1;
		}
#else
		if (keysym == XK_w)
		{
			fractal->camera.position.z += 0.2;
			camera_changed = 1;
		}
		else if (keysym == XK_s)
		{
			fractal->camera.position.z -= 0.2;
			camera_changed = 1;
		}
		else if (keysym == XK_a)
		{
			fractal->camera.position.x -= 0.2;
			camera_changed = 1;
		}
		else if (keysym == XK_d)
		{
			fractal->camera.position.x += 0.2;
			camera_changed = 1;
		}
		else if (keysym == XK_q)
		{
			fractal->camera.position.y += 0.2;
			camera_changed = 1;
		}
		else if (keysym == XK_e)
		{
			fractal->camera.position.y -= 0.2;
			camera_changed = 1;
		}
		else if (keysym == XK_Left)
		{
			fractal->camera.rotation.y -= 0.05;
			camera_changed = 1;
		}
		else if (keysym == XK_Right)
		{
			fractal->camera.rotation.y += 0.05;
			camera_changed = 1;
		}
		else if (keysym == XK_Up)
		{
			fractal->camera.rotation.x -= 0.05;
			camera_changed = 1;
		}
		else if (keysym == XK_Down)
		{
			fractal->camera.rotation.x += 0.05;
			camera_changed = 1;
		}
#endif
		
		// Re-render if camera has changed
		if (camera_changed)
		{
			render_menger_sponge(fractal);
			return (0);
		}
		// Change Menger sponge iterations
#ifdef __APPLE__
		else if (keysym == KEY_PLUS || keysym == KEY_EQUAL)
#else
		else if (keysym == XK_plus || keysym == XK_equal)
#endif
		{
			if (fractal->menger.iterations < 10)
			{
				// Show loading message before reconstruction
				display_progress(fractal, "Rebuilding Menger sponge...");
				draw_image_to_window(fractal);
				
				// Update iterations and rebuild BVH
				fractal->menger.iterations++;
				if (fractal->menger.bvh_root)
					free_bvh(fractal->menger.bvh_root);
				fractal->menger.bvh_root = build_menger_bvh(fractal->menger.iterations);
				if (!fractal->menger.bvh_root)
				{
					fractal->menger.iterations--;
				}
				
				// Use direct Menger rendering instead of generic fractal_render
				render_menger_sponge(fractal);
			}
		}
#ifdef __APPLE__
		else if (keysym == KEY_MINUS)
#else
		else if (keysym == XK_minus)
#endif
		{
			if (fractal->menger.iterations > 0)
			{
				// Show loading message before reconstruction
				display_progress(fractal, "Rebuilding Menger sponge...");
				draw_image_to_window(fractal);
				
				// Update iterations and rebuild BVH
				fractal->menger.iterations--;
				if (fractal->menger.bvh_root)
					free_bvh(fractal->menger.bvh_root);
				fractal->menger.bvh_root = build_menger_bvh(fractal->menger.iterations);
				if (!fractal->menger.bvh_root)
				{
					fractal->menger.iterations++;
				}
				
				// Use direct Menger rendering instead of generic fractal_render
				render_menger_sponge(fractal);
			}
		}
		// Resolution control for performance
#ifdef __APPLE__
		else if (keysym == KEY_bracketleft && fractal->resolution_factor < 16)
#else
		else if (keysym == XK_bracketleft && fractal->resolution_factor < 16)
#endif
		{
			fractal->resolution_factor *= 2;  // Lower resolution (faster)
			render_menger_sponge(fractal);    // Re-render with new resolution
		}
#ifdef __APPLE__
		else if (keysym == KEY_bracketright && fractal->resolution_factor > 1)
#else
		else if (keysym == XK_bracketright && fractal->resolution_factor > 1)
#endif
		{
			fractal->resolution_factor /= 2;  // Higher resolution (slower)
			render_menger_sponge(fractal);    // Re-render with new resolution
		}
		// Reset camera position
#ifdef __APPLE__
		else if (keysym == KEY_r)
#else
		else if (keysym == XK_r)
#endif
		{
			fractal->camera.position = (t_vec3){0, 0, -3};
			fractal->camera.rotation = (t_vec3){0, 0, 0};
			render_menger_sponge(fractal);
		}
		// Debug camera positions
#ifdef __APPLE__
		else if (keysym == KEY_1)
#else
		else if (keysym == XK_1)
#endif
		{
			fractal->camera.position = (t_vec3){0.0, 0.0, -4.0};
			fractal->camera.rotation = (t_vec3){0.0, 0.0, 0.0};
			render_menger_sponge(fractal);
		}
#ifdef __APPLE__
		else if (keysym == KEY_2)
#else
		else if (keysym == XK_2)
#endif
		{
			fractal->camera.position = (t_vec3){4.0, 0.0, 0.0};
			fractal->camera.rotation = (t_vec3){0.0, -1.57, 0.0};
			render_menger_sponge(fractal);
		}
#ifdef __APPLE__
		else if (keysym == KEY_3)
#else
		else if (keysym == XK_3)
#endif
		{
			fractal->camera.position = (t_vec3){0.0, 4.0, -0.0};
			fractal->camera.rotation = (t_vec3){1.6, 0.0, 0.0};
			render_menger_sponge(fractal);
		}
#ifdef __APPLE__
		else if (keysym == KEY_4)
#else
		else if (keysym == XK_4)
#endif
		{
			fractal->camera.position = (t_vec3){2.4, 0.4, -2.8};
			fractal->camera.rotation = (t_vec3){0.15, -0.7, 0.0};
			render_menger_sponge(fractal);
		}
#ifdef __APPLE__
		else if (keysym == KEY_5)
#else
		else if (keysym == XK_5)
#endif
		{
			fractal->camera.position = (t_vec3){2.4, 2.0, -2.8};
			fractal->camera.rotation = (t_vec3){0.45, -0.7, 0.0};
			render_menger_sponge(fractal);
		}
		
		// Ensure we're in 3D mode and render
		fractal->is_3d = 1;
		// The camera position keys will handle rendering when needed
		return (0);
	}

	// Original 2D fractal controls
#ifdef __APPLE__
	if (keysym == KEY_RIGHT)
		fractal->shift_x += (0.5 * fractal->zoom);
	else if (keysym == KEY_LEFT)
		fractal->shift_x -= (0.5 * fractal->zoom);
	else if (keysym == KEY_DOWN)
		fractal->shift_y -= (0.5 * fractal->zoom);
	else if (keysym == KEY_UP)
		fractal->shift_y += (0.5 * fractal->zoom);
	else if (keysym == KEY_PLUS)
		fractal->iterations_defintion += 10;
	else if (keysym == KEY_MINUS)
		fractal->iterations_defintion -= 10;
	else if (keysym == KEY_M)
		fractal->mouse_control = !fractal->mouse_control;
#else
	if (keysym == XK_Right)
		fractal->shift_x += (0.5 * fractal->zoom);
	else if (keysym == XK_Left)
		fractal->shift_x -= (0.5 * fractal->zoom);
	else if (keysym == XK_Down)
		fractal->shift_y -= (0.5 * fractal->zoom);
	else if (keysym == XK_Up)
		fractal->shift_y += (0.5 * fractal->zoom);
	else if (keysym == XK_plus)
		fractal->iterations_defintion += 10;
	else if (keysym == XK_minus)
		fractal->iterations_defintion -= 10;
	else if (keysym == XK_m)
		fractal->mouse_control = !fractal->mouse_control;
#endif
	// Ensure we're in 2D mode for these fractals
	fractal->is_3d = 0;
	// Render with appropriate method based on zoom level
	fractal_render(fractal);
	return (0);
}

int	mouse_handler(int button, int x, int y, t_fractal *fractal)
{
	// For 3D mode - handle camera controls
	if (fractal->is_3d)
	{
		// Adjust camera position based on mouse scroll
#ifdef __APPLE__
		if (button == MOUSE_SCROLL_DOWN)  // Zoom out
		{
			fractal->camera.position.z *= 1.1;
		}
		else if (button == MOUSE_SCROLL_UP)  // Zoom in
		{
			fractal->camera.position.z *= 0.9;
		}
#else
		if (button == Button5)  // Zoom out
		{
			fractal->camera.position.z *= 1.1;
		}
		else if (button == Button4)  // Zoom in
		{
			fractal->camera.position.z *= 0.9;
		}
#endif
		
		render_menger_sponge(fractal);
		return (0);
	}
	
	// For 2D fractal modes
#ifdef __APPLE__
	if (button == MOUSE_SCROLL_DOWN)
		fractal->zoom *= 0.95;
	else if (button == MOUSE_SCROLL_UP)
		fractal->zoom *= 1.05;
	else if (button == MOUSE_LEFT_BUTTON && !ft_strncmp(fractal->name, "mandelbrot", 10))
#else
	if (button == Button5)
		fractal->zoom *= 0.95;
	else if (button == Button4)
		fractal->zoom *= 1.05;
	else if (button == Button1 && !ft_strncmp(fractal->name, "mandelbrot", 10))
#endif
	{
		fractal->is_dragging = 1;
		fractal->prev_mouse_x = x;
		fractal->prev_mouse_y = y;
	}
	// Render with appropriate method
	fractal_render(fractal);
	return (0);
}

int	mouse_release(int button, int x, int y, t_fractal *fractal)
{
	(void)x;
	(void)y;
#ifdef __APPLE__
	if (button == MOUSE_LEFT_BUTTON)
#else
	if (button == Button1)
#endif
		fractal->is_dragging = 0;
	return (0);
}

static void	update_julia_coords(int x, int y, t_fractal *fractal)
{
	t_bounds	bounds_x;
	t_bounds	bounds_y;

	bounds_x = (t_bounds){-2, +2, 0, WIDTH};
	bounds_y = (t_bounds){+2, -2, 0, HEIGHT};
	fractal->julia_x = (map(x, bounds_x) * fractal->zoom) + fractal->shift_x;
	fractal->julia_y = (map(y, bounds_y) * fractal->zoom) + fractal->shift_y;
}

static void	handle_mandelbrot_drag(int x, int y, t_fractal *fractal)
{
	double	dx;
	double	dy;
	double	scaled_dx;
	double	scaled_dy;

	if (fractal->is_dragging)
	{
		dx = x - fractal->prev_mouse_x;
		dy = y - fractal->prev_mouse_y;
		
		// Scale the movement based on zoom level
		scaled_dx = (dx / (double)WIDTH) * 4.0 * fractal->zoom;
		scaled_dy = (dy / (double)HEIGHT) * 4.0 * fractal->zoom;
		
		fractal->shift_x -= scaled_dx;
		fractal->shift_y += scaled_dy;  // Inverted Y axis
		
		fractal->prev_mouse_x = x;
		fractal->prev_mouse_y = y;
		
		fractal_render(fractal);
	}
}

int	julia_track(int x, int y, t_fractal *fractal)
{
	// Skip mouse tracking for Menger sponge mode
	if (!ft_strncmp(fractal->name, "menger", 6))
		return (0);
	
	// Julia set tracking
	if (!ft_strncmp(fractal->name, "julia", 5) && fractal->mouse_control)
	{
		update_julia_coords(x, y, fractal);
		fractal_render(fractal);
	}
	// Mandelbrot dragging
	else if (!ft_strncmp(fractal->name, "mandelbrot", 10))
	{
		handle_mandelbrot_drag(x, y, fractal);
	}
	return (0);
}
