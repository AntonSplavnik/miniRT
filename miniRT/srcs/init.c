/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 13:13:10 by asplavni          #+#    #+#             */
/*   Updated: 2025/07/23 21:12:02 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/miniRT.h"

/* Initialize basic app and window settings */
void	init_app_and_window(t_scene *scene)
{
	scene->app.mlx = NULL;
	scene->app.img = NULL;
	scene->width = WIDTH;
	scene->height = HEIGHT;
	scene->background_color = BLACK;
	scene->objects = NULL;
	scene->lights = NULL;
	scene->ambient.has_ambient = 0;
	scene->camera.has_camera = 0;
}

/* Initialize camera settings */
void	init_camera_settings(t_scene *scene)
{
	scene->camera.aspect_ratio = (double)WIDTH / HEIGHT;
	scene->camera.fov = FOV;
	scene->camera.near = NEAR_PLANE;
	scene->camera.far = FAR_PLANE;
	scene->camera.movement_speed = CAMERA_MOVEMENT_SPEED;
	scene->camera.rotation_speed = CAMERA_ROTATION_SPEED;
}


/* Initialize graphics and mouse settings */
void	init_graphics(t_scene *scene)
{
	scene->graphic_settings.enable_hard_shadows = 1;
	scene->graphic_settings.enable_reflections = 1;
	scene->graphic_settings.enable_specular = 1;
	scene->graphic_settings.enable_refraction = 1;
	scene->graphic_settings.enable_status_message = 0;
	scene->graphic_settings.enable_global_illumination = 1;
	scene->graphic_settings.ssaa_samples = 1;
	scene->graphic_settings.resolution_factor = 1;
}
void	init_mouse(t_scene *scene)
{
	scene->mouse_state.is_dragging = 0;
	scene->mouse_state.left_button_down = 0;
	scene->mouse_state.right_button_down = 0;
	scene->mouse_state.middle_button_down = 0;
	scene->mouse_state.x = 0;
	scene->mouse_state.y = 0;
	scene->mouse_state.prev_mouse_x = 0;
	scene->mouse_state.prev_mouse_y = 0;
}

void	init_global_ilumination(t_scene *scene)
{
	scene->gi.max_depth = 3; // quality vs performanc
	scene->gi.samples_per_bounce = 18; // noise vs speed
}

void	init_light_settings(t_scene *scene)
{
	scene->ambient.ratio = 0.05;
	scene->ambient.color = create_color(255, 255, 255);
	scene->sample = 1;
	scene->scene_bvh = NULL;
}
void	init_light(t_scene *scene)
{
	init_global_ilumination(scene);
	init_light_settings(scene);
}

/* Initialize ambient lighting and other scene data */
void	init_data(t_scene *scene)
{
	init_app_and_window(scene);
	init_camera_settings(scene);
	init_graphics(scene);
	init_light(scene);
	init_mouse(scene);
}
