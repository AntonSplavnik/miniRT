/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   status_formatting.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 00:00:00 by antonsplavn      #+#    #+#             */
/*   Updated: 2025/07/18 00:00:00 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

/*
** Converts integer to string
*/
int	int_to_string(int num, char *str)
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
** Formats a float value to string with 2 decimal places
*/
void	format_float_string(double value, char *str)
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
** Appends coordinate values in parentheses with separators
*/
void	append_coordinate_values(char *pos, char *x_str, \
									char *y_str, char *z_str)
{
	int	i;

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
	*pos++ = ' ';
	*pos++ = '|';
	*pos++ = ' ';
	*pos = '\0';
}

/*
** Creates and displays status text image with formatting
*/
mlx_image_t	*create_status_image(t_scene *scene, char *status)
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
** Applies color tinting to the status text image
*/
void	apply_text_color(mlx_image_t *img)
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
