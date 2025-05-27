/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 13:13:10 by asplavni          #+#    #+#             */
/*   Updated: 2025/05/27 11:35:31 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/miniRT.h"

//Used
static void	malloc_error(void)
{
	perror("Malloc malfunction");
	exit(EXIT_FAILURE);
}

//Used
static void	data_init(t_scene *scene)
{
	//app
	scene->app.mlx = NULL;
	scene->app.img = NULL;

	// window
	scene->width = WIDTH;
	scene->height = HEIGHT;

	// mouse
	scene->mouse_state.left_button_down = false;
	scene->mouse_state.right_button_down = false;
	scene->mouse_state.middle_button_down = false;

	scene->mouse_state.is_dragging = false;

	scene->mouse_state.x = 0;
	scene->mouse_state.y = 0;

	scene->mouse_state.prev_mouse_x = 0;
	scene->mouse_state.prev_mouse_y = 0;

	// ambient
	scene->ambient.ratio = 0.1; //default
	scene->ambient.color = create_color(255,255 ,255); //white by default

	// background
	scene->background_color = (0 << 16 | 0 << 8 | 0); // black

	// lights
	scene->lights = NULL;
	scene->objects = NULL;

	// Initialize control window
    scene->graphic_settings.enable_hard_shadows = true;
    scene->graphic_settings.enable_reflections = true;
    scene->graphic_settings.enable_specular = true;
	scene->graphic_settings.enable_refraction = true;
	scene->graphic_settings.resolution_factor = 1;


	//Bonus
	scene->sample = 1;
	scene->max_depth = 3;


	//---to do : sort things from here---
	scene->escape_value = 4;
	scene->iterations_defintion = 100;
	scene->shift_x = 0.0;
	scene->shift_y = 0.0;
	scene->zoom = 1.0;

	scene->graphic_settings.resolution_factor = 1;  // Default resolution factor


	// Initialize camera defaults for 3D scenes
	scene->camera.fov = 60.0;
	scene->camera.aspect_ratio = (double)WIDTH / HEIGHT;
	scene->camera.near = 0.1;
	scene->camera.far = 100.0;
	scene->camera.position = (t_vec3){0.0, 0.0, 0.0};
	scene->camera.rotation = (t_vec3){0.0, 0.0, 0.0};
    scene->camera.movement_speed = 0.5;
    scene->camera.rotation_speed = 0.05;


	// Initialize Menger sponge defaults
	scene->menger.iterations = 0;
	scene->menger.size = 1.0;
	scene->menger.position = (t_vec3){0.0, 0.0, 0.0};
	scene->menger.rotation = (t_vec3){0.0, 0.0, 0.0};
	scene->menger.bvh_root = NULL;
}

static void	setup_hooks(t_scene *scene)
{
 	setup_mouse_hook(scene);
	setup_key_hooks(scene);
	setup_close_hook(scene);
}

void scene_init(t_scene *scene)
{
    // Initialize data
    data_init(scene);

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

    // Set up event hooks
    setup_hooks(scene);
}
