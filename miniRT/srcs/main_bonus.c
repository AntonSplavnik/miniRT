/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 00:00:00 by abillote          #+#    #+#             */
/*   Updated: 2025/07/18 16:25:42 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/miniRT.h"
#include "../libft/libft.h"

/* Initialize scene data and parse file for bonus */
void	init_and_parse_scene_bonus(t_scene *scene, char *filename)
{
	scene->name = filename;
	init_data(scene);
	if (parse_scene_file_bonus(filename, scene) != 0)
	{
		ft_putstr_fd("Error\nParsing scene file failed.\n", 2);
		exit(1);
	}
	init_scene_bvh(scene);
}

/* Initialize graphics and UI components for bonus */
void	init_graphics_and_ui_bonus(t_scene *scene)
{
	init_mlx(scene);
	setup_hooks(scene);
	init_ui(scene);
	mlx_loop_hook(scene->app.mlx, ui_animation_loop, scene);
}

/* Start the raytracer with given scene and filename for bonus */
void	start_raytracer_bonus(t_scene *scene, char *filename)
{
	init_and_parse_scene_bonus(scene, filename);
	init_graphics_and_ui_bonus(scene);
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

/* Main function for bonus */
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
