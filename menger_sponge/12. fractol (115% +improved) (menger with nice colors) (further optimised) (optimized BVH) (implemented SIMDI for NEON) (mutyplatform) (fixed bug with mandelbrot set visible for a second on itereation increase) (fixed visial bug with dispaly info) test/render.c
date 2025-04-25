/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: parallels <parallels@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 13:17:50 by asplavni          #+#    #+#             */
/*   Updated: 2025/04/13 19:36:34 by parallels        ###   ########.fr       */
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

void	mandelbrot_vs_julia(t_complex *complex_z,
			t_complex *complex_c, t_fractal *fractal)
{
	if (!ft_strncmp(fractal->name, "julia", 5))
	{
		complex_c->x = fractal->julia_x;
		complex_c->y = fractal->julia_y;
	}
	else
	{
		complex_c->x = complex_z->x;
		complex_c->y = complex_z->y;
	}
}

void	render_pixel_row(int y, t_fractal *fractal, t_complex z)
{
	t_bounds	bounds_x;
	t_bounds	bounds_y;
	double		mapped_y;
	int			x;
	t_complex	local_z;

	(void)z; // Explicitly mark z as unused to prevent warnings
	(void)local_z;
	bounds_x = (t_bounds){-2, +2, 0, WIDTH};
	bounds_y = (t_bounds){+2, -2, 0, HEIGHT};
	mapped_y = (map(y, bounds_y) * fractal->zoom) + fractal->shift_y;
	x = 0;
	while (x < WIDTH)
	{
		local_z.x = (map(x, bounds_x) * fractal->zoom) + fractal->shift_x;
		local_z.y = mapped_y;
		
		// We're calculating the coordinates but not passing them to handle_pixel
		// Just let handle_pixel do its own coordinate calculation since it does that anyway
		handle_pixel(x, y, fractal);
		
		x++;
	}
}

void	draw_image_to_window(t_fractal *fractal)
{
	mlx_put_image_to_window(fractal->mlx_connection, fractal->mlx_window,
		fractal->img.img_ptr, 0, 0);
}

void	fractal_render_multithreaded(t_fractal *fractal)
{
	pthread_t		threads[NUM_THREADS];
	t_thread_data	thread_data[NUM_THREADS];
	int				i;
	int				rows_per_thread;

	// Calculate how many rows each thread should process
	rows_per_thread = HEIGHT / NUM_THREADS;

	// Create and start the threads
	i = 0;
	while (i < NUM_THREADS)
	{
		thread_data[i].fractal = fractal;
		thread_data[i].start_row = i * rows_per_thread;
		thread_data[i].end_row = (i == NUM_THREADS - 1) ? HEIGHT : (i + 1) * rows_per_thread;
		
		if (pthread_create(&threads[i], NULL, thread_render, &thread_data[i]) != 0)
		{
			// If thread creation fails, fallback to single-threaded rendering
			i = 0;
			while (i < HEIGHT)
			{
				render_pixel_row(i, fractal, (t_complex){0, 0});
				i++;
			}
			draw_image_to_window(fractal);
			return;
		}
		i++;
	}

	// Wait for all threads to complete
	i = 0;
	while (i < NUM_THREADS)
	{
		pthread_join(threads[i], NULL);
		i++;
	}
	
	// Update the window with the rendered image
	draw_image_to_window(fractal);
}

void	fractal_render_hybrid(t_fractal *fractal)
{
	int	step;
	int	x;
	int	y;

	// First, do a quick low-resolution preview
	step = 8; // Start with very low resolution
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
	
	// Show the preview immediately
	draw_image_to_window(fractal);
	
	// Then render at full resolution with multithreading
	fractal_render_multithreaded(fractal);
}

void	fractal_render_single_thread(t_fractal *fractal)
{
	int	i;

	// Render each row in a single thread
	i = 0;
	while (i < HEIGHT)
	{
		render_pixel_row(i, fractal, (t_complex){0, 0});
		i++;
	}
	
	// Update the window with the rendered image
	draw_image_to_window(fractal);
}

void	fractal_render(t_fractal *fractal)
{
	// Display progress at start of rendering
	display_progress(fractal, "Rendering...");
	
	// Check if the fractal is 3D 
	if (fractal->is_3d)
	{
		if (!ft_strncmp(fractal->name, "menger", 6))
		{
			// Use the dedicated Menger sponge renderer
			render_menger_sponge(fractal);
		}
		else if (!ft_strncmp(fractal->name, "mandelbrot3d", 12))
		{
			// Use the dedicated 3D Mandelbrot renderer
			render_mandelbrot3d(fractal);
		}
		return; // Exit early to prevent any 2D rendering
	}
	
	// 2D fractal rendering
	// Use single-threaded rendering for 2D fractals
	fractal_render_single_thread(fractal);
	
	// Draw the image to window
	draw_image_to_window(fractal);
	
	// Update status AFTER drawing the image
	display_status(fractal);
	
	// For 2D fractals, clear any bottom message
	if (!fractal->is_3d) {
		display_progress(fractal, ""); // Empty message clears previous message
	}
}

// Function to display progress visually on the window
void display_progress(t_fractal *fractal, const char *status_text)
{
	// Only proceed if MLX is properly initialized
	if (!fractal || !fractal->mlx_connection || !fractal->mlx_window)
		return;
		
	// Special case: empty message means clear any previous message
	if (status_text[0] == '\0') {
		mlx_string_put(fractal->mlx_connection, fractal->mlx_window, 
					10, HEIGHT - 15, 0x000000, "                                   ");
		return;
	}
	
	// For Menger sponge, keep displaying messages
	if (fractal->is_3d && !ft_strncmp(fractal->name, "menger", 6))
	{
		// Display the text with a bright color (yellow text)
		mlx_string_put(fractal->mlx_connection, fractal->mlx_window, 
					10, HEIGHT - 15, 0xFFFF00, (char *)status_text);
	}
	else if (ft_strncmp((char *)status_text, "Rendering...", 12) == 0)
	{
		// For 2D fractals, only show "Rendering..." message
		// Display the text with a bright color (yellow text)
		mlx_string_put(fractal->mlx_connection, fractal->mlx_window, 
					10, HEIGHT - 15, 0xFFFF00, (char *)status_text);
	}
	// For 2D fractals, don't show "Rendering complete" message
	
	// Force immediate processing of the window update
	mlx_do_sync(fractal->mlx_connection);
}
