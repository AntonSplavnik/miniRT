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
		if (!ft_strncmp(fractal->name, "menger", 6))
		{
			// Menger sponge status
			snprintf(status, 100, "3D Mode | Iterations: %d | Resolution: %d", 
					fractal->menger.iterations, fractal->resolution_factor);
		}
		else if (!ft_strncmp(fractal->name, "mandelbrot3d", 12))
		{
			// 3D Mandelbrot status
			snprintf(status, 100, "3D Mandelbrot | Iterations: %d | Resolution: %d", 
					fractal->iterations_defintion, fractal->resolution_factor);
		}
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
	
	// Handle 3D fractal controls
	if (fractal->is_3d)
	{
		int camera_changed = 0;
		// Camera movement - slower to be more precise
#ifdef __APPLE__
		if (keysym == KEY_W)
		{
			// Move forward along camera's viewing direction
			fractal->camera.position.z += 0.2;
			camera_changed = 1;
		}
		else if (keysym == KEY_S)
		{
			// Move backward along camera's viewing direction
			fractal->camera.position.z -= 0.2;
			camera_changed = 1;
		}
		else if (keysym == KEY_A)
		{
			// Strafe left
			fractal->camera.position.x -= 0.2;
			camera_changed = 1;
		}
		else if (keysym == KEY_D)
		{
			// Strafe right
			fractal->camera.position.x += 0.2;
			camera_changed = 1;
		}
		else if (keysym == KEY_Q)
		{
			// Move up
			fractal->camera.position.y += 0.2;
			camera_changed = 1;
		}
		else if (keysym == KEY_E)
		{
			// Move down
			fractal->camera.position.y -= 0.2;
			camera_changed = 1;
		}
		else if (keysym == KEY_LEFT)
		{
			// Rotate camera left around Y axis
			fractal->camera.rotation.y -= 0.05;
			camera_changed = 1;
		}
		else if (keysym == KEY_RIGHT)
		{
			// Rotate camera right around Y axis 
			fractal->camera.rotation.y += 0.05;
			camera_changed = 1;
		}
		else if (keysym == KEY_UP)
		{
			// Rotate camera up around X axis
			fractal->camera.rotation.x -= 0.05;
			camera_changed = 1;
		}
		else if (keysym == KEY_DOWN)
		{
			// Rotate camera down around X axis
			fractal->camera.rotation.x += 0.05;
			camera_changed = 1;
		}
#else
		if (keysym == XK_w)
		{
			// Move forward along camera's viewing direction
			fractal->camera.position.z += 0.2;
			camera_changed = 1;
		}
		else if (keysym == XK_s)
		{
			// Move backward along camera's viewing direction
			fractal->camera.position.z -= 0.2;
			camera_changed = 1;
		}
		else if (keysym == XK_a)
		{
			// Strafe left
			fractal->camera.position.x -= 0.2;
			camera_changed = 1;
		}
		else if (keysym == XK_d)
		{
			// Strafe right
			fractal->camera.position.x += 0.2;
			camera_changed = 1;
		}
		else if (keysym == XK_q)
		{
			// Move up
			fractal->camera.position.y += 0.2;
			camera_changed = 1;
		}
		else if (keysym == XK_e)
		{
			// Move down
			fractal->camera.position.y -= 0.2;
			camera_changed = 1;
		}
		else if (keysym == XK_Left)
		{
			// Rotate camera left around Y axis
			fractal->camera.rotation.y -= 0.05;
			camera_changed = 1;
		}
		else if (keysym == XK_Right)
		{
			// Rotate camera right around Y axis
			fractal->camera.rotation.y += 0.05;
			camera_changed = 1;
		}
		else if (keysym == XK_Up)
		{
			// Rotate camera up around X axis
			fractal->camera.rotation.x -= 0.05;
			camera_changed = 1;
		}
		else if (keysym == XK_Down)
		{
			// Rotate camera down around X axis
			fractal->camera.rotation.x += 0.05;
			camera_changed = 1;
		}
#endif
		
		// Re-render if camera has changed
		if (camera_changed)
		{
			if (!ft_strncmp(fractal->name, "menger", 6))
				render_menger_sponge(fractal);
			else if (!ft_strncmp(fractal->name, "mandelbrot3d", 12)) 
				render_mandelbrot3d(fractal);
				
			return (0);
		}
		// Change Menger sponge iterations
#ifdef __APPLE__
		else if (keysym == KEY_PLUS || keysym == KEY_EQUAL)
#else
		else if (keysym == XK_plus || keysym == XK_equal)
#endif
		{
			if (!ft_strncmp(fractal->name, "menger", 6))
			{
				// Increase iterations for Menger sponge
				if (fractal->menger.iterations < 5)
				{
					fractal->menger.iterations++;
					
					// Free the old BVH
					if (fractal->menger.bvh_root)
					{
						free_bvh(fractal->menger.bvh_root);
					}
					
					// Build a new BVH with the higher iteration count
					fractal->menger.bvh_root = build_menger_bvh(fractal->menger.iterations);
					
					// Re-render at a lower resolution for a quick preview
					fractal->resolution_factor = 4;
					render_menger_sponge(fractal);
					display_status(fractal);
				}
			}
			else if (!ft_strncmp(fractal->name, "mandelbrot3d", 12))
			{
				// Increase iterations for 3D Mandelbrot
				if (fractal->iterations_defintion < 20)
				{
					fractal->iterations_defintion++;
					
					// Re-render at a lower resolution for a quick preview
					fractal->resolution_factor = 4;
					render_mandelbrot3d(fractal);
					display_status(fractal);
				}
			}
			else
			{
				// Handle 2D fractals (existing code)
				fractal->iterations_defintion += 5;
				fractal_render(fractal);
				display_status(fractal);
			}
			
			return (0);
		}
#ifdef __APPLE__
		else if (keysym == KEY_MINUS)
#else
		else if (keysym == XK_minus)
#endif
		{
			if (!ft_strncmp(fractal->name, "menger", 6))
			{
				// Decrease iterations for Menger sponge (minimum 1)
				if (fractal->menger.iterations > 1)
				{
					fractal->menger.iterations--;
					
					// Free the old BVH
					if (fractal->menger.bvh_root)
					{
						free_bvh(fractal->menger.bvh_root);
					}
					
					// Build a new BVH with the lower iteration count
					fractal->menger.bvh_root = build_menger_bvh(fractal->menger.iterations);
					
					// Re-render at a lower resolution for a quick preview
					fractal->resolution_factor = 4;
					render_menger_sponge(fractal);
					display_status(fractal);
				}
			}
			else if (!ft_strncmp(fractal->name, "mandelbrot3d", 12))
			{
				// Decrease iterations for 3D Mandelbrot (minimum 3)
				if (fractal->iterations_defintion > 3)
				{
					fractal->iterations_defintion--;
					
					// Re-render at a lower resolution for a quick preview
					fractal->resolution_factor = 4;
					render_mandelbrot3d(fractal);
					display_status(fractal);
				}
			}
			else
			{
				// Handle 2D fractals (existing code)
				if (fractal->iterations_defintion > 10)
				{
					fractal->iterations_defintion -= 5;
					fractal_render(fractal);
					display_status(fractal);
				}
			}
			
			return (0);
		}
		// Resolution control for performance
#ifdef __APPLE__
		else if (keysym == KEY_bracketleft && fractal->resolution_factor < 16)
#else
		else if (keysym == XK_bracketleft && fractal->resolution_factor < 16)
#endif
		{
			// Double the resolution factor (lower quality, faster rendering)
			fractal->resolution_factor *= 2;
			
			if (!ft_strncmp(fractal->name, "menger", 6))
				render_menger_sponge(fractal);
			else if (!ft_strncmp(fractal->name, "mandelbrot3d", 12))
				render_mandelbrot3d(fractal);
				
			display_status(fractal);
			return (0);
		}
#ifdef __APPLE__
		else if (keysym == KEY_bracketright && fractal->resolution_factor > 1)
#else
		else if (keysym == XK_bracketright && fractal->resolution_factor > 1)
#endif
		{
			// Halve the resolution factor (higher quality, slower rendering)
			fractal->resolution_factor /= 2;
			
			if (!ft_strncmp(fractal->name, "menger", 6))
				render_menger_sponge(fractal);
			else if (!ft_strncmp(fractal->name, "mandelbrot3d", 12))
				render_mandelbrot3d(fractal);
				
			display_status(fractal);
			return (0);
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
	// Safety check
	if (!fractal)
	{
		return (0);
	}
	
	// For 3D fractal types
	if (fractal->is_3d)
	{
		if (button == 1) // Left mouse button
		{
			fractal->is_dragging = 1;
			fractal->prev_mouse_x = x;
			fractal->prev_mouse_y = y;
			return (0);
		}
		else if (button == 4) // Mouse wheel up
		{
			// Zoom in (move camera forward)
			fractal->camera.position.z += 0.3;
			
			if (!ft_strncmp(fractal->name, "menger", 6))
			{
				// Use a lower resolution for preview during movement
				fractal->resolution_factor = 4;
				render_menger_sponge(fractal);
			}
			else if (!ft_strncmp(fractal->name, "mandelbrot3d", 12))
			{
				// Use a lower resolution for preview during movement
				fractal->resolution_factor = 4;
				render_mandelbrot3d(fractal);
			}
			
			display_status(fractal);
			return (0);
		}
		else if (button == 5) // Mouse wheel down
		{
			// Zoom out (move camera backward)
			fractal->camera.position.z -= 0.3;
			
			if (!ft_strncmp(fractal->name, "menger", 6))
			{
				// Use a lower resolution for preview during movement
				fractal->resolution_factor = 4;
				render_menger_sponge(fractal);
			}
			else if (!ft_strncmp(fractal->name, "mandelbrot3d", 12))
			{
				// Use a lower resolution for preview during movement
				fractal->resolution_factor = 4;
				render_mandelbrot3d(fractal);
			}
			
			display_status(fractal);
			return (0);
		}
	}
	// For 2D fractals (existing code)
	else
	{
		// Handle zoom for 2D fractals
		if (button == 4) // Mouse wheel up - zoom in
		{
			// Set the zoom center to the mouse position
			t_bounds	bounds_x = (t_bounds){-2, +2, 0, WIDTH};
			t_bounds	bounds_y = (t_bounds){+2, -2, 0, HEIGHT};
			double 		mouse_x = (map(x, bounds_x) * fractal->zoom) + fractal->shift_x;
			double 		mouse_y = (map(y, bounds_y) * fractal->zoom) + fractal->shift_y;
			
			// Apply the zoom
			fractal->zoom *= 0.9;
			
			// Adjust the shift to keep the mouse position as the zoom center
			fractal->shift_x = mouse_x - (map(x, bounds_x) * fractal->zoom);
			fractal->shift_y = mouse_y - (map(y, bounds_y) * fractal->zoom);
			
			fractal_render(fractal);
			display_status(fractal);
			return (0);
		}
		else if (button == 5) // Mouse wheel down - zoom out
		{
			// Set the zoom center to the mouse position
			t_bounds	bounds_x = (t_bounds){-2, +2, 0, WIDTH};
			t_bounds	bounds_y = (t_bounds){+2, -2, 0, HEIGHT};
			double 		mouse_x = (map(x, bounds_x) * fractal->zoom) + fractal->shift_x;
			double 		mouse_y = (map(y, bounds_y) * fractal->zoom) + fractal->shift_y;
			
			// Apply the zoom
			fractal->zoom *= 1.1;
			
			// Adjust the shift to keep the mouse position as the zoom center
			fractal->shift_x = mouse_x - (map(x, bounds_x) * fractal->zoom);
			fractal->shift_y = mouse_y - (map(y, bounds_y) * fractal->zoom);
			
			fractal_render(fractal);
			display_status(fractal);
			return (0);
		}
		else if (button == 1) // Left click
		{
			if (!ft_strncmp(fractal->name, "mandelbrot", 10))
			{
				// In Mandelbrot, start dragging (panning)
				fractal->is_dragging = 1;
				fractal->prev_mouse_x = x;
				fractal->prev_mouse_y = y;
			}
			return (0);
		}
	}
	
	return (0);
}

int	mouse_release(int button, int x, int y, t_fractal *fractal)
{
	(void)button;
	(void)x;
	(void)y;
	
	if (fractal->is_dragging)
	{
		fractal->is_dragging = 0;
		
		// If we were dragging in 3D mode, render at full quality after release
		if (fractal->is_3d)
		{
			fractal->resolution_factor = 1;
			
			if (!ft_strncmp(fractal->name, "menger", 6))
				render_menger_sponge(fractal);
			else if (!ft_strncmp(fractal->name, "mandelbrot3d", 12))
				render_mandelbrot3d(fractal);
				
			display_status(fractal);
		}
	}
	
	return (0);
}

// Mouse motion handler for 3D fractal rotation
static void handle_3d_rotation(int x, int y, t_fractal *fractal)
{
	// Calculate movement deltas
	int dx = x - fractal->prev_mouse_x;
	int dy = y - fractal->prev_mouse_y;
	
	// Update rotation (scale down the movement for smoother control)
	// Use mouse for rotation, not position adjustment
	fractal->camera.rotation.y += dx * 0.01;  // Horizontal mouse movement rotates around Y axis
	fractal->camera.rotation.x += dy * 0.01;  // Vertical mouse movement rotates around X axis
	
	// Store new positions
	fractal->prev_mouse_x = x;
	fractal->prev_mouse_y = y;
	
	// Render at low resolution during dragging for better performance
	// Use 4 instead of 8 for better quality during dragging
	fractal->resolution_factor = 4;
	
	if (!ft_strncmp(fractal->name, "menger", 6))
		render_menger_sponge(fractal);
	else if (!ft_strncmp(fractal->name, "mandelbrot3d", 12))
		render_mandelbrot3d(fractal);
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
	double	scale;

	// Calculate the movement in screen coordinates
	dx = fractal->prev_mouse_x - x;
	dy = fractal->prev_mouse_y - y;
	
	// Scale the movement based on zoom level
	scale = fractal->zoom / 2.0;
	
	// Update shift values (move in the opposite direction of mouse movement)
	fractal->shift_x += dx * scale / (WIDTH / 4.0);
	fractal->shift_y += dy * scale / (HEIGHT / 4.0);
	
	// Update previous mouse positions
	fractal->prev_mouse_x = x;
	fractal->prev_mouse_y = y;
}

int	julia_track(int x, int y, t_fractal *fractal)
{
	if (fractal->is_dragging)
	{
		if (fractal->is_3d)
		{
			// Handle 3D rotation
			handle_3d_rotation(x, y, fractal);
		}
		else if (!ft_strncmp(fractal->name, "mandelbrot", 10))
		{
			// Handle 2D mandelbrot dragging
			handle_mandelbrot_drag(x, y, fractal);
			fractal_render(fractal);
		}
	}
	else if (fractal->mouse_control && !ft_strncmp(fractal->name, "julia", 5))
	{
		// Handle Julia set parameter control
		update_julia_coords(x, y, fractal);
		fractal_render(fractal);
	}
	
	return (0);
}
