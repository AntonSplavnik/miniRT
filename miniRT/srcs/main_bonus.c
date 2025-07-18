/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 00:00:00 by abillote          #+#    #+#             */
/*   Updated: 2025/07/18 15:55:39 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/miniRT.h"
#include "../libft/libft.h"

// Add the function prototype
void init_scene_bvh(t_scene *scene);

void	start_raytracer_bonus(t_scene *scene, char *filename)
{
	scene->name = filename;

    init_data(scene);

	if (parse_scene_file_bonus(filename, scene) != 0)
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
		start_raytracer_bonus(&scene, av[1]);
	else
		print_usage_and_exit();
	return (0);
}
