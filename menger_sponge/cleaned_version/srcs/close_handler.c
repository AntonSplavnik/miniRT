/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   close_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 13:08:43 by asplavni          #+#    #+#             */
/*   Updated: 2025/05/23 15:35:00 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include <stdio.h>

int	close_handler(t_scene *scene)
{
	static int	freed = 0;

	if (freed)
		return (0);
	freed = 1;
	cleanup_scene(scene);
	if (scene->img.img_ptr && scene->mlx_connection)
	{
		mlx_destroy_image(scene->mlx_connection, scene->img.img_ptr);
		scene->img.img_ptr = NULL;
	}
	if (scene->mlx_window && scene->mlx_connection)
	{
		mlx_destroy_window(scene->mlx_connection, scene->mlx_window);
		scene->mlx_window = NULL;
	}
	if (scene->mlx_connection)
	{
#ifdef __linux__
		mlx_destroy_display(scene->mlx_connection);
#endif
		free(scene->mlx_connection);
		scene->mlx_connection = NULL;
	}
	exit(EXIT_SUCCESS);
	return (0);
}

