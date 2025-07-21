/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_hooks.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 00:00:00 by abillote          #+#    #+#             */
/*   Updated: 2025/07/21 05:29:00 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/miniRT.h"

/* Set up event hooks */
void	setup_hooks(t_scene *scene)
{
	setup_mouse_hook(scene);
	setup_key_hooks(scene);
	setup_close_hook(scene);
}

/* Initialize user interface */
void	init_ui(t_scene *scene)
{
	init_toggle_button(scene);
	init_ui_panel(scene);
}

/* Handle malloc errors */
static void	malloc_error(void)
{
	ssize_t	result;

	result = write(2, "Error\nMalloc malfunction\n", 25);
	(void)result;
	exit(EXIT_FAILURE);
}

/* Initialize MLX42 and create window */
void	init_mlx(t_scene *scene)
{
	scene->app.mlx = mlx_init(WIDTH, HEIGHT, scene->name, 1);
	if (!scene->app.mlx)
		malloc_error();
	scene->app.img = mlx_new_image(scene->app.mlx, WIDTH, HEIGHT);
	if (!scene->app.img)
	{
		mlx_terminate(scene->app.mlx);
		malloc_error();
	}
	if (mlx_image_to_window(scene->app.mlx, scene->app.img, 0, 0) < 0)
	{
		mlx_delete_image(scene->app.mlx, scene->app.img);
		mlx_terminate(scene->app.mlx);
		malloc_error();
	}
}

/* Initialize scene BVH for optimization */
void	init_scene_bvh(t_scene *scene)
{
	count_scene_objects(scene);
	if (scene->objects && scene->object_count > 30)
	{
		scene->scene_bvh = build_scene_bvh(scene);
		if (scene->scene_bvh)
			ft_printf("Scene BVH initialized successfully (%d objects)\n", \
				scene->object_count);
		else
			ft_printf("Failed to build scene BVH\n");
	}
	else
	{
		scene->scene_bvh = NULL;
		if (scene->objects)
			ft_printf("Scene has %d objects, using linear search\n", \
				scene->object_count);
		else
			ft_printf("No objects in scene, BVH not built\n");
	}
}
