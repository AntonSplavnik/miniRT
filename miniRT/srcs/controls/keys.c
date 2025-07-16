/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 22:15:00 by antonsplavn      #+#    #+#             */
/*   Updated: 2025/07/13 22:16:00 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

/*
** Main key callback function that dispatches to specific handlers
*/
void	key_callback(mlx_key_data_t keydata, void *param)
{
	t_scene	*scene;

	scene = (t_scene *)param;
	if (keydata.action != MLX_PRESS)
		return ;
	if (keydata.key == MLX_KEY_ESCAPE)
	{
		mlx_close_window(scene->app.mlx);
		return ;
	}
	handle_movement_keys(keydata, scene);
	handle_strafe_keys(keydata, scene);
	handle_vertical_keys(keydata, scene);
	handle_sampling_keys(keydata, scene);
}

/*
** Sets up key input hooks for the MLX window
*/
void	setup_key_hooks(t_scene *scene)
{
	mlx_key_hook(scene->app.mlx, key_callback, scene);
}
