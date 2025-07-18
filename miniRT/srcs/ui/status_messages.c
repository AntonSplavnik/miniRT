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
static void	format_float_string(double value, char *str);
static int	int_to_string(int num, char *str);

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
	while ("Camera Position: "[i])
	{
		*pos++ = "Camera Position: "[i++];
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
** Formats a float value to string with 2 decimal places
*/
static void	format_float_string(double value, char *str)
{
	int		whole_part;
	int		decimal_part;
	int		i;
	int		is_negative;

	is_negative = (value < 0);
	if (is_negative)
		value = -value;
	whole_part = (int)value;
	decimal_part = (int)((value - whole_part) * 100);
	i = 0;
	if (is_negative)
		str[i++] = '-';
	if (whole_part == 0)
		str[i++] = '0';
	else
		i += int_to_string(whole_part, &str[i]);
	str[i++] = '.';
	if (decimal_part < 10)
		str[i++] = '0';
	i += int_to_string(decimal_part, &str[i]);
	str[i] = '\0';
}

/*
** Converts integer to string
*/
static int	int_to_string(int num, char *str)
{
	int	len;
	int	temp;
	int	original_len;

	if (num == 0)
	{
		str[0] = '0';
		return (1);
	}
	len = 0;
	temp = num;
	while (temp > 0)
	{
		temp /= 10;
		len++;
	}
	original_len = len;
	str[len] = '\0';
	while (num > 0)
	{
		str[--len] = '0' + (num % 10);
		num /= 10;
	}
	return (original_len);
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
