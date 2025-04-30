/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 13:13:10 by asplavni          #+#    #+#             */
/*   Updated: 2025/04/29 13:06:16 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "platform.h"

//Used
static void	malloc_error(void)
{
	perror("Malloc malfunction");
	exit(EXIT_FAILURE);
}

//Used
static void	data_init(t_scene *scene)
{

	scene->mlx_connection = NULL;
	scene->mlx_window = NULL;
	scene->width = WIDTH;
	scene->height = HEIGHT;
	scene->ambient.ratio = 0.1; //default
	scene->ambient.color = create_color(255,255 ,255); //white by default
	scene->lights = NULL;
	scene->objects = NULL;

	//Bonus
	scene->sample = 1;
	scene->max_depth = 3;

	//---to do : sort things from here---
	scene->escape_value = 4;
	scene->iterations_defintion = 100;
	scene->shift_x = 0.0;
	scene->shift_y = 0.0;
	scene->zoom = 1.0;
	scene->mouse_control = 1;
	scene->is_dragging = 0;
	scene->prev_mouse_x = 0;
	scene->prev_mouse_y = 0;
	scene->resolution_factor = 4;  // Default resolution factor

	// Initialize camera defaults for 3D scenes
	scene->is_3d = 0;  // Default to 2D mode - needs to be cleaned out
	scene->camera.fov = 60.0;
	scene->camera.aspect_ratio = (double)WIDTH / HEIGHT;
	scene->camera.near = 0.1;
	scene->camera.far = 100.0;
	scene->camera.position = (t_vec3){0.0, 0.0, -3.0};
	scene->camera.rotation = (t_vec3){0.0, 0.0, 0.0};

	// Initialize Menger sponge defaults
	scene->menger.iterations = 0;
	scene->menger.size = 1.0;
	scene->menger.position = (t_vec3){0.0, 0.0, 0.0};
	scene->menger.rotation = (t_vec3){0.0, 0.0, 0.0};
	scene->menger.bvh_root = NULL;
}

//Used
static void	events_init(t_scene *scene)
{
#ifdef __APPLE__
	mlx_hook(scene->mlx_window, 2, 1L<<0, key_handler, scene);
	mlx_hook(scene->mlx_window, 4, 1L<<2, mouse_handler, scene);
	mlx_hook(scene->mlx_window, 5, 1L<<3, mouse_release, scene);
	mlx_hook(scene->mlx_window, 17, 0, close_handler, scene);
#else
	mlx_hook(scene->mlx_window, KeyPress,
		KeyPressMask, key_handler, scene);
	mlx_hook(scene->mlx_window, ButtonPress,
		ButtonPressMask, mouse_handler, scene);
	mlx_hook(scene->mlx_window, ButtonRelease,
		ButtonReleaseMask, mouse_release, scene);
	mlx_hook(scene->mlx_window, DestroyNotify,
		StructureNotifyMask, close_handler, scene);
#endif
}

void scene_mlx_init(t_scene *scene)
{
	scene->mlx_connection = mlx_init();
	if (NULL == scene->mlx_connection)
		malloc_error();
	scene->mlx_window = mlx_new_window(scene->mlx_connection,
			WIDTH, HEIGHT, scene->name);
	if (NULL == scene->mlx_window)
	{
#ifdef __linux__
		mlx_destroy_display(scene->mlx_connection);
#endif
		free(scene->mlx_connection);
		malloc_error();
	}
	scene->img.img_ptr = mlx_new_image(scene->mlx_connection,
			WIDTH, HEIGHT);
	if (NULL == scene->img.img_ptr)
	{
		mlx_destroy_window(scene->mlx_connection, scene->mlx_window);
#ifdef __linux__
		mlx_destroy_display(scene->mlx_connection);
#endif
		free(scene->mlx_connection);
		malloc_error();
	}
	scene->img.pixels_ptr = mlx_get_data_addr(scene->img.img_ptr,
			&scene->img.bpp, &scene->img.line_len, &scene->img.endian);
}

//Used
void	scene_init(t_scene *scene)
{

	data_init(scene);
	scene_mlx_init(scene);
	events_init(scene);
}
