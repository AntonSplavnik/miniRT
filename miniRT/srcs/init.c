/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 13:13:10 by asplavni          #+#    #+#             */
/*   Updated: 2025/06/24 12:28:58 by antonsplavn      ###   ########.fr       */
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
	scene->width = WIDTH;
	scene->height = HEIGHT;
	scene->background_color = BLACK;

	//Init objects
	scene->objects = NULL;
	scene->lights = NULL;

	scene->ambient.has_ambient = 0;
	scene->camera.has_camera = 0;

	//Init camera
	scene->camera.aspect_ratio = (double)WIDTH / HEIGHT;
	scene->camera.fov = FOV;
	scene->camera.near = NEAR_PLANE;
	scene->camera.far = FAR_PLANE;
	scene->camera.movement_speed = 1.0;
	scene->camera.rotation_speed = 0.05;

	//Init graphic settings
	scene->graphic_settings.enable_hard_shadows = true;
	scene->graphic_settings.enable_reflections = true;
	scene->graphic_settings.enable_specular = true;
	scene->graphic_settings.enable_refraction = false;
	scene->graphic_settings.enable_status_message = true;
	scene->graphic_settings.ssaa_samples = 1;  // No SSAA by default
	scene->graphic_settings.resolution_factor = 1;

	//Init mouse
	scene->mouse_state.is_dragging = false;
	scene->mouse_state.left_button_down = false;
	scene->mouse_state.right_button_down = false;
	scene->mouse_state.middle_button_down = false;

	scene->sample = 1;
	scene->max_depth = 3;

	scene->scene_bvh = NULL; // Initialize BVH pointer to NULL
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

// Add this function to initialize the scene BVH
void init_scene_bvh(t_scene *scene)
{
    // Build BVH for the entire scene
    if (scene->objects)
    {
        scene->scene_bvh = build_scene_bvh(scene);

        if (scene->scene_bvh)
            printf("Scene BVH initialized successfully\n");
        else
            printf("Failed to build scene BVH\n");
    }
    else
    {
        scene->scene_bvh = NULL;
        printf("No objects in scene, BVH not built\n");
    }
}
