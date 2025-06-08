/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 13:16:31 by asplavni          #+#    #+#             */
/*   Updated: 2025/06/07 21:31:43 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/miniRT.h"


void	start_raytracer(t_scene *scene, char *filename)
{
	scene->name = filename;

	scene_init(scene);
	
	if (!parse_scene_file(filename, scene))
	{
		cleanup_scene(scene);
		write_string_to_file_descriptor("Error: Failed to parse scene file\n", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}

	mlx_loop_hook(scene->app.mlx, ui_animation_loop, scene); 	// Register the UI animation loop

	render_scene(scene);

	mlx_loop(scene->app.mlx);

	close_callback(scene);

	exit(EXIT_SUCCESS);
}

void	print_usage_and_exit(void)
{
	write_string_to_file_descriptor("Please enter a valid arg\n", STDERR_FILENO);
	exit(EXIT_FAILURE);
}


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

