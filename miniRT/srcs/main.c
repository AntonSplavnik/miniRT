/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 13:16:31 by asplavni          #+#    #+#             */
/*   Updated: 2025/07/18 13:32:03 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/miniRT.h"
#include "../libft/libft.h"

// Add the function prototype
void init_scene_bvh(t_scene *scene);

void	start_raytracer(t_scene *scene, char *filename)
{
	scene->name = filename;

    init_data(scene);

	if (parse_scene_file(filename, scene) != 0)
	{
		write_string_to_file_descriptor("Error\nParsing scene file failed.\n", 2);
		exit(1);
	}

	// Initialize BVH after all objects are loaded
	init_scene_bvh(scene);

	// Initialize MLX before setting up hooks
	init_mlx(scene);
	setup_hooks(scene);
	// Set up UI and hooks
	init_ui(scene);
	mlx_loop_hook(scene->app.mlx, ui_animation_loop, scene);		// Register the UI animation loop
	render_scene(scene);
	mlx_loop(scene->app.mlx);
	close_callback(scene);
	exit(EXIT_SUCCESS);
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

