/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 13:16:31 by asplavni          #+#    #+#             */
/*   Updated: 2025/07/20 23:19:16 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/miniRT.h"

/* Initialize scene data and parse file */
void	init_and_parse_scene(t_scene *scene, char *filename)
{
	scene->name = filename;
	init_data(scene);
	if (parse_scene_file(filename, scene) != 0)
	{
		ft_putstr_fd("Error\nParsing scene file failed.\n", 2);
		exit(1);
	}
	init_scene_bvh(scene);
}

/* Initialize graphics and UI components */
void	init_graphics_and_ui(t_scene *scene)
{
	init_mlx(scene);
	setup_hooks(scene);
	init_ui(scene);
	mlx_loop_hook(scene->app.mlx, ui_animation_loop, scene);
}

/* Start the raytracer with given scene and filename */
void	start_raytracer(t_scene *scene, char *filename)
{
	init_and_parse_scene(scene, filename);
	init_graphics_and_ui(scene);
	render_scene(scene);
	mlx_loop(scene->app.mlx);
	close_callback(scene);
	exit(EXIT_SUCCESS);
}

/* Print usage message and exit */
void	print_usage_and_exit(void)
{
	ft_putendl_fd("Please enter a valid arg\n", STDERR_FILENO);
	exit(EXIT_FAILURE);
}

/* Main function */
int	main(int ac, char **av)
{
	t_scene	scene;

	memset(&scene, 0, sizeof(t_scene));
	if (ac == 2)
		start_raytracer(&scene, av[1]);
	else
		print_usage_and_exit();
	return (0);
}
