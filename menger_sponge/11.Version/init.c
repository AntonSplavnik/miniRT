/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 13:13:10 by asplavni          #+#    #+#             */
/*   Updated: 2025/04/14 19:49:16 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

static void	malloc_error(void)
{
	perror("Malloc malfunction");
	exit(EXIT_FAILURE);
}

static void	data_init(t_fractal *fractal)
{
	fractal->escape_value = 4;
	fractal->iterations_defintion = 100;
	fractal->shift_x = 0.0;
	fractal->shift_y = 0.0;
	fractal->zoom = 1.0;
	fractal->mouse_control = 1;
	fractal->is_dragging = 0;
	fractal->prev_mouse_x = 0;
	fractal->prev_mouse_y = 0;
	fractal->resolution_factor = 4;  // Default resolution factor

	// Initialize camera defaults for 3D fractals
	fractal->is_3d = 0;  // Default to 2D mode
	fractal->camera.fov = 60.0;
	fractal->camera.aspect_ratio = (double)WIDTH / HEIGHT;
	fractal->camera.near = 0.1;
	fractal->camera.far = 100.0;
	fractal->camera.position = (t_vec3){0.0, 0.0, -3.0};
	fractal->camera.rotation = (t_vec3){0.0, 0.0, 0.0};

	// Initialize Menger sponge defaults
	fractal->menger.iterations = 0;
	fractal->menger.size = 1.0;
	fractal->menger.position = (t_vec3){0.0, 0.0, 0.0};
	fractal->menger.rotation = (t_vec3){0.0, 0.0, 0.0};
	fractal->menger.bvh_root = NULL;
}

static void	events_init(t_fractal *fractal)
{
#ifdef __APPLE__
	mlx_hook(fractal->mlx_window, 2, 1L<<0, key_handler, fractal);
	mlx_hook(fractal->mlx_window, 4, 1L<<2, mouse_handler, fractal);
	mlx_hook(fractal->mlx_window, 5, 1L<<3, mouse_release, fractal);
	mlx_hook(fractal->mlx_window, 17, 0, close_handler, fractal);
	mlx_hook(fractal->mlx_window, 6, 1L<<6, julia_track, fractal);
#else
	mlx_hook(fractal->mlx_window, KeyPress,
		KeyPressMask, key_handler, fractal);
	mlx_hook(fractal->mlx_window, ButtonPress,
		ButtonPressMask, mouse_handler, fractal);
	mlx_hook(fractal->mlx_window, ButtonRelease,
		ButtonReleaseMask, mouse_release, fractal);
	mlx_hook(fractal->mlx_window, DestroyNotify,
		StructureNotifyMask, close_handler, fractal);
	mlx_hook(fractal->mlx_window, MotionNotify,
		PointerMotionMask, julia_track, fractal);
#endif
}

void	fractal_init(t_fractal *fractal)
{
	fractal->mlx_connection = mlx_init();
	if (NULL == fractal->mlx_connection)
		malloc_error();
	fractal->mlx_window = mlx_new_window(fractal->mlx_connection,
			WIDTH, HEIGHT, fractal->name);
	if (NULL == fractal->mlx_window)
	{
#ifndef __APPLE__
		mlx_destroy_display(fractal->mlx_connection);
#endif
		free(fractal->mlx_connection);
		malloc_error();
	}
	fractal->img.img_ptr = mlx_new_image(fractal->mlx_connection,
			WIDTH, HEIGHT);
	if (NULL == fractal->img.img_ptr)
	{
		mlx_destroy_window(fractal->mlx_connection, fractal->mlx_window);
#ifndef __APPLE__
		mlx_destroy_display(fractal->mlx_connection);
#endif
		free(fractal->mlx_connection);
		malloc_error();
	}
	fractal->img.pixels_ptr = mlx_get_data_addr(fractal->img.img_ptr,
			&fractal->img.bpp, &fractal->img.line_len, &fractal->img.endian);
	events_init(fractal);
	data_init(fractal);
}
