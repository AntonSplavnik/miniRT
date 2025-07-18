/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_handlers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 22:16:00 by antonsplavn      #+#    #+#             */
/*   Updated: 2025/07/13 22:16:00 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

/*
** Handles camera movement keys (W and S)
*/
void	handle_movement_keys(mlx_key_data_t keydata, t_scene *scene)
{
	if (keydata.key == MLX_KEY_W)
	{
		scene->camera.position = vec3_add(scene->camera.position, \
			vec3_scale(get_forward_vector(scene->camera.rotation), \
			scene->camera.movement_speed));
		render_scene(scene);
		display_status(scene);
	}
	else if (keydata.key == MLX_KEY_S)
	{
		scene->camera.position = vec3_subtract(scene->camera.position, \
			vec3_scale(get_forward_vector(scene->camera.rotation), \
			scene->camera.movement_speed));
		render_scene(scene);
		display_status(scene);
	}
}

/*
** Handles strafe movement keys (A and D)
*/
void	handle_strafe_keys(mlx_key_data_t keydata, t_scene *scene)
{
	if (keydata.key == MLX_KEY_A)
	{
		scene->camera.position = vec3_subtract(scene->camera.position, \
			vec3_scale(get_right_vector(scene->camera.rotation), \
			scene->camera.movement_speed));
		render_scene(scene);
		display_status(scene);
	}
	else if (keydata.key == MLX_KEY_D)
	{
		scene->camera.position = vec3_add(scene->camera.position, \
			vec3_scale(get_right_vector(scene->camera.rotation), \
			scene->camera.movement_speed));
		render_scene(scene);
		display_status(scene);
	}
}

/*
** Handles vertical movement keys (Q and E)
*/
void	handle_vertical_keys(mlx_key_data_t keydata, t_scene *scene)
{
	if (keydata.key == MLX_KEY_Q)
	{
		scene->camera.position = vec3_subtract(scene->camera.position, \
			vec3_scale(get_up_vector(scene->camera.rotation), \
			scene->camera.movement_speed));
		render_scene(scene);
		display_status(scene);
	}
	else if (keydata.key == MLX_KEY_E)
	{
		scene->camera.position = vec3_add(scene->camera.position, \
			vec3_scale(get_up_vector(scene->camera.rotation), \
			scene->camera.movement_speed));
		render_scene(scene);
		display_status(scene);
	}
}

/*
** Handles anti-aliasing sampling keys (1-4)
*/
void	handle_sampling_keys(mlx_key_data_t keydata, t_scene *scene)
{
	if (keydata.key == MLX_KEY_1)
	{
		scene->graphic_settings.ssaa_samples = 1;
		render_scene(scene);
	}
	else if (keydata.key == MLX_KEY_2)
	{
		scene->graphic_settings.ssaa_samples = 2;
		render_scene(scene);
	}
	else if (keydata.key == MLX_KEY_3)
	{
		scene->graphic_settings.ssaa_samples = 3;
		render_scene(scene);
	}
	else if (keydata.key == MLX_KEY_4)
	{
		scene->graphic_settings.ssaa_samples = 4;
		render_scene(scene);
	}
}
