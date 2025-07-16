/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_checkbox_drawing.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 00:50:12 by antonsplavn       #+#    #+#             */
/*   Updated: 2025/07/14 00:50:13 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

/*
** Draw checkbox background
*/
void	draw_checkbox_background(t_panel *p, mlx_image_t *img, int x, int y)
{
	int	i;
	int	j;

	i = 1;
	while (i < p->checkbox_size - 1)
	{
		j = 1;
		while (j < p->checkbox_size - 1)
		{
			mlx_put_pixel(img, x + i, y + j, 0x222222FF);
			j++;
		}
		i++;
	}
}

/*
** Draw checkbox border
*/
void	draw_checkbox_border(t_panel *p, mlx_image_t *img, int x, int y)
{
	int	i;

	i = 0;
	while (i < p->checkbox_size)
	{
		mlx_put_pixel(img, x + i, y, 0xFFFFFFFF);
		mlx_put_pixel(img, x + i, y + p->checkbox_size - 1, 0xFFFFFFFF);
		mlx_put_pixel(img, x, y + i, 0xFFFFFFFF);
		mlx_put_pixel(img, x + p->checkbox_size - 1, y + i, 0xFFFFFFFF);
		i++;
	}
}

/*
** Draw checkbox check mark
*/
void	draw_checkbox_check(t_panel *p, mlx_image_t *img, int x, int y)
{
	int	j;

	j = 0;
	while (j < p->checkbox_size - 4)
	{
		mlx_put_pixel(img, x + 2 + j, y + 2 + j, 0x00FF00FF);
		mlx_put_pixel(img, x + p->checkbox_size - 3 - j, y + 2 + j,
			0x00FF00FF);
		j++;
	}
}

/*
** Draw panel background with header and borders
*/
void	draw_panel_background(t_panel *p)
{
	if (p->current_width <= 0)
		return ;
	draw_panel_main_bg(p);
	draw_panel_header(p);
	draw_panel_borders(p);
}

/*
** Draw main panel background
*/
void	draw_panel_main_bg(t_panel *p)
{
	int	x;
	int	y;

	x = 0;
	while (x < p->current_width)
	{
		y = 0;
		while (y < p->height)
		{
			mlx_put_pixel(p->panel_img, x, y, 0x33333399);
			y++;
		}
		x++;
	}
}
