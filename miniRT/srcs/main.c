/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 13:16:31 by asplavni          #+#    #+#             */
/*   Updated: 2025/06/24 19:17:16 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/miniRT.h"
#include "../libft/libft.h"

// Add the function prototype
void init_scene_bvh(t_scene *scene);

void	start_raytracer(t_scene *scene, char *filename)
{
	scene->name = filename;

	// Initialize miniRT
	init_data(scene);
	init_mlx(scene);

	// Parse scene file

	if (parse_scene_file(filename, scene) != 0)
	{
		write_string_to_file_descriptor("Error\nParsing scene file failed.\n", 2);
		exit(1);
	}

	// Initialize BVH after all objects are loaded
	init_scene_bvh(scene);

	// Set up UI and hooks
	init_ui(scene);
	setup_hooks(scene);

	// Initialize the mouse hook for camera control
	// mlx_loop_hook(scene->app.mlx, &render_scene, scene);

	render_scene(scene);

	// Start main loop
	mlx_loop(scene->app.mlx);

	// Clean up
	cleanup_scene(scene);
}

void	print_usage_and_exit(void)
{
	ft_putendl_fd("Please enter a valid arg\n", STDERR_FILENO);
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

