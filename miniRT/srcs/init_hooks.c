/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_hooks.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 00:00:00 by abillote          #+#    #+#             */
/*   Updated: 2025/07/18 16:20:23 by abillote         ###   ########.fr       */
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
	ssize_t	result;

	if (scene->objects)
	{
		scene->scene_bvh = build_scene_bvh(scene);
		if (!scene->scene_bvh)
		{
			result = write(2, "Warning: Failed to build scene BVH\n", 36);
			(void)result;
		}
	}
	else
		scene->scene_bvh = NULL;
}
