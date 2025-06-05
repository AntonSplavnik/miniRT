/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 13:16:31 by asplavni          #+#    #+#             */
/*   Updated: 2025/06/05 11:58:58 by abillote         ###   ########.fr       */
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
		ft_putendl_fd("Error: Failed to parse scene file\n", STDERR_FILENO);
		exit(EXIT_FAILURE);
	}
	render_scene(scene);
	display_status(scene);
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
