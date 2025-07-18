/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   close_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 13:08:43 by asplavni          #+#    #+#             */
/*   Updated: 2025/07/18 16:28:06 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/miniRT.h"

void	mlx_cleanup(t_scene *scene)
{
	if (scene->app.mlx)
	{
		if (scene->app.img)
		{
			mlx_delete_image(scene->app.mlx, scene->app.img);
		}
		mlx_terminate(scene->app.mlx);
	}
}

void	close_callback(void *param)
{
	t_scene	*scene;

	scene = (t_scene *)param;
	cleanup_scene(scene);
	mlx_cleanup(scene);
	exit(EXIT_SUCCESS);
}

void	setup_close_hook(t_scene *scene)
{
	mlx_close_hook(scene->app.mlx, close_callback, scene);
}
