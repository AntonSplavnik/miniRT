/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_drawing.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 00:41:38 by antonsplavn       #+#    #+#             */
/*   Updated: 2025/07/14 00:46:11 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

/*
** Draw toggle button with hamburger menu icon
*/
void	draw_toggle_button(t_scene *scene)
{
	t_toggle_button	*tog;
	int				i;
	int				j;

	tog = &scene->ui.toggle;
	draw_button_background(tog);
	draw_button_border(tog);
	i = 0;
	while (i < 3)
	{
		j = 0;
		while (j < tog->size - 10)
		{
			mlx_put_pixel(tog->toggle_img, 5 + j, 8 + (i * 6), 0xFFFFFFFF);
			j++;
		}
		i++;
	}
}

/*
** Draw button background
*/
void	draw_button_background(t_toggle_button *tog)
{
	int	i;
	int	j;

	i = 0;
	while (i < tog->size)
	{
		j = 0;
		while (j < tog->size)
		{
			mlx_put_pixel(tog->toggle_img, i, j, 0x333333FF);
			j++;
		}
		i++;
	}
}

/*
** Draw button border
*/
void	draw_button_border(t_toggle_button *tog)
{
	int	i;

	i = 0;
	while (i < tog->size)
	{
		mlx_put_pixel(tog->toggle_img, i, 0, 0xFFFFFFFF);
		mlx_put_pixel(tog->toggle_img, i, tog->size - 1, 0xFFFFFFFF);
		mlx_put_pixel(tog->toggle_img, 0, i, 0xFFFFFFFF);
		mlx_put_pixel(tog->toggle_img, tog->size - 1, i, 0xFFFFFFFF);
		i++;
	}
}

/*
** Draw a checkbox with optional check mark
*/
void	draw_checkbox(t_panel *p, mlx_image_t *img, t_checkbox_pos *pos)
{
	draw_checkbox_background(p, img, pos->x, pos->y);
	draw_checkbox_border(p, img, pos->x, pos->y);
	if (pos->checked)
		draw_checkbox_check(p, img, pos->x, pos->y);
}
