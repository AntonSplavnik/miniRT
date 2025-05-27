/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 13:16:31 by asplavni          #+#    #+#             */
/*   Updated: 2025/05/23 15:33:30 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/miniRT.h"

/**
 * UI animation loop function - updates the UI panel animation
 * This is registered as a loop hook with MLX
 */
void ui_animation_loop(void *param)
{
	t_scene *scene = (t_scene *)param;
	static double last_update_time = 0;
	double current_time;
	
	// Get current time
	current_time = mlx_get_time();
	
	// Limit UI updates to 60 FPS for smooth animation
	if (current_time - last_update_time > 0.016) { // ~60 FPS
		// Update and redraw the UI panel if animation is active
		if (draw_ui_panel(scene)) {
			// Animation is still active, no need to re-render the entire scene
		}
		last_update_time = current_time;
	}
}

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
	
	// Initialize UI panel
	init_ui_panel(scene);
	
	// Register the UI animation loop
	mlx_loop_hook(scene->app.mlx, ui_animation_loop, scene);
	
	render_scene(scene);
	display_status(scene);
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
