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

/*
** Applies color tinting to the status text image
*/
static void	apply_text_color(mlx_image_t *img)
{
	uint32_t	i;
	uint8_t		*pixel;
	float		alpha;

	i = 0;
	while (i < img->width * img->height)
	{
		pixel = &img->pixels[i * 4];
		if (pixel[3] > 0)
		{
			alpha = pixel[3] / 255.0f;
			pixel[0] = (uint8_t)(255 * alpha + pixel[0] * (1 - alpha));
			pixel[1] = (uint8_t)(160 * alpha + pixel[1] * (1 - alpha));
			pixel[2] = (uint8_t)(30 * alpha + pixel[2] * (1 - alpha));
		}
		i++;
	}
}

/*
** Creates and formats the status string with scene name
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
	while ("Camera Position"[i])
	{
		*pos++ = "Camera Position"[i++];
	}
	*pos = '\0';
}

/*
** Creates and displays status text image with formatting
*/
static mlx_image_t	*create_status_image(t_scene *scene, char *status)
{
	mlx_image_t	*img;

	if (!scene->app.mlx)
		return (NULL);
	img = mlx_put_string(scene->app.mlx, status, 50, 15);
	if (img)
	{
		img->enabled = true;
		apply_text_color(img);
	}
	return (img);
}

/*
** Helper function to print status messages
*/
void	display_status(t_scene *scene)
{
	char				status[100];
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
