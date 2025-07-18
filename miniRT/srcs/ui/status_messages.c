/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   status_messages.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 22:20:00 by antonsplavn      #+#    #+#             */
/*   Updated: 2025/07/13 22:20:00 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

static void	format_camera_coordinates(t_scene *scene, char *pos);
static void	format_camera_direction(t_scene *scene, char *pos);

/*
** Creates and formats the status string with scene name and camera coordinates
*/
static void	format_status_string(t_scene *scene, char *status)
{
	char	*pos;
	int		i;

	pos = status;
	i = 0;
	if (scene->name)
	{
		while (scene->name[i] && i < 50)
		{
			*pos++ = scene->name[i++];
		}
		*pos++ = ' ';
		*pos++ = '|';
		*pos++ = ' ';
	}
	i = 0;
	while ("Camera Pos: "[i])
	{
		*pos++ = "Camera Pos: "[i++];
	}
	format_camera_coordinates(scene, pos);
}

/*
** Formats camera coordinates into the string
*/
static void	format_camera_coordinates(t_scene *scene, char *pos)
{
	char	x_str[16];
	char	y_str[16];
	char	z_str[16];
	int		i;

	format_float_string(scene->camera.position.x, x_str);
	format_float_string(scene->camera.position.y, y_str);
	format_float_string(scene->camera.position.z, z_str);
	append_coordinate_values(pos, x_str, y_str, z_str);
	while (*pos)
		pos++;
	i = 0;
	while ("Dir: "[i])
	{
		*pos++ = "Dir: "[i++];
	}
	format_camera_direction(scene, pos);
}

/*
** Formats camera direction into the string
*/
static void	format_camera_direction(t_scene *scene, char *pos)
{
	char	x_str[16];
	char	y_str[16];
	char	z_str[16];
	int		i;

	format_float_string(scene->camera.forwards.x, x_str);
	format_float_string(scene->camera.forwards.y, y_str);
	format_float_string(scene->camera.forwards.z, z_str);
	*pos++ = '(';
	i = 0;
	while (x_str[i])
		*pos++ = x_str[i++];
	*pos++ = ',';
	*pos++ = ' ';
	i = 0;
	while (y_str[i])
		*pos++ = y_str[i++];
	*pos++ = ',';
	*pos++ = ' ';
	i = 0;
	while (z_str[i])
		*pos++ = z_str[i++];
	*pos++ = ')';
	*pos = '\0';
}

/*
** Helper function to print status messages
*/
void	display_status(t_scene *scene)
{
	char				status[150];
	static mlx_image_t	*status_img = NULL;

	if (!scene || !scene->app.mlx)
		return ;
	if (status_img != NULL)
	{
		mlx_delete_image(scene->app.mlx, status_img);
		status_img = NULL;
	}
	if (scene->graphic_settings.enable_status_message)
	{
		format_status_string(scene, status);
		status_img = create_status_image(scene, status);
	}
}
