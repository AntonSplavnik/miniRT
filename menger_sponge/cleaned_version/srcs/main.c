/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 13:16:31 by asplavni          #+#    #+#             */
/*   Updated: 2025/04/29 13:35:04 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "platform.h"
#include <string.h>

void cleanup_scene(t_scene *scene)
{
	t_object *obj;
	t_object *next_obj;
	t_light *light;
	t_light *next_light;

	if (!scene)
		return;
	obj = scene->objects;
	while (obj)
	{
		next_obj = obj->next;
		if (obj->data)
			free(obj->data);
		free(obj);
		obj = next_obj;
	}
	light = scene->lights;
	while (light)
	{
		next_light = light->next;
		free(light);
		light = next_light;
	}
}

void	start_raytracer(t_scene *scene, char *name)
{
	scene->name = name;
	scene_init(scene);
	init_3d(scene);

	render_menger_sponge(scene);
	display_status(scene);

	// Start the event loop
	mlx_loop(scene->mlx_connection);

	// Once mlx_loop exits, call close_handler to clean up resources
	close_handler(scene);

	// Exit program after cleanup is complete
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

	// Initialize the structure to zeros/NULL to avoid uninitialized memory
	memset(&scene, 0, sizeof(t_scene));

	if (ac == 2)
		start_raytracer(&scene, av[1]);
	else
		print_usage_and_exit();
	return (0);
}
