/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 13:13:10 by asplavni          #+#    #+#             */
/*   Updated: 2025/07/16 14:16:42 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/miniRT.h"

static void	malloc_error(void)
{
	perror("Malloc malfunction");
	exit(EXIT_FAILURE);
}

void	init_data(t_scene *scene)
{
	//app
	scene->app.mlx = NULL;
	scene->app.img = NULL;

	// window
	scene->width = WIDTH;
	scene->height = HEIGHT;

	// Mouse controls
	scene->mouse_state.left_button_down = false;
	scene->mouse_state.right_button_down = false;
	scene->mouse_state.middle_button_down = false;
	scene->mouse_state.is_dragging = false;
	scene->mouse_state.x = 0;
	scene->mouse_state.y = 0;
	scene->mouse_state.prev_mouse_x = 0;
	scene->mouse_state.prev_mouse_y = 0;

	// Ambient light
	scene->ambient.ratio = 0.1; //default
	scene->ambient.color = create_color(255,255 ,255); //white by default
	scene->ambient.has_ambient = 0;

	// Background color
	scene->background_color = (0 << 16 | 0 << 8 | 0); // black

	// Lights
	scene->lights = NULL;
	scene->objects = NULL;

	// Graphic settings
	scene->graphic_settings.enable_hard_shadows = true;
	scene->graphic_settings.enable_reflections = true;
	scene->graphic_settings.enable_specular = true;
	scene->graphic_settings.enable_refraction = true;
	scene->graphic_settings.resolution_factor = 1;
	scene->graphic_settings.ssaa_samples = 1;

	// Bonus
	scene->sample = 1;
	scene->max_depth = 3;
	
	//camera
	scene->camera.movement_speed = 0.5;
}

// Set up event hooks
void	setup_hooks(t_scene *scene)
{
 	setup_mouse_hook(scene);
	setup_key_hooks(scene);
	setup_close_hook(scene);
}

void	init_ui(t_scene *scene)
{
	init_toggle_button(scene);
	init_ui_panel(scene);
}


void	init_mlx(t_scene *scene)
{
	// Initialize MLX42
	scene->app.mlx = mlx_init(WIDTH, HEIGHT, scene->name, true);
	if (!scene->app.mlx)
		malloc_error();

	// Create main image
	scene->app.img = mlx_new_image(scene->app.mlx, WIDTH, HEIGHT);
	if (!scene->app.img)
	{
		mlx_terminate(scene->app.mlx);
		malloc_error();
	}

	// Display the image in the window
	if (mlx_image_to_window(scene->app.mlx, scene->app.img, 0, 0) < 0)
	{
		mlx_delete_image(scene->app.mlx, scene->app.img);
		mlx_terminate(scene->app.mlx);
		malloc_error();
	}
}
