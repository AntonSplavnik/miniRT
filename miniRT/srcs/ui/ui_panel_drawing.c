/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_panel_drawing.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 23:38:55 by antonsplavn       #+#    #+#             */
/*   Updated: 2025/07/14 00:40:55 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

/*
** Draw panel header background
*/
void	draw_panel_header(t_panel *p)
{
	int	x;
	int	y;

	x = 0;
	while (x < p->current_width)
	{
		y = 0;
		while (y < p->header_height)
		{
			mlx_put_pixel(p->panel_img, x, y, 0x222222CC);
			y++;
		}
		x++;
	}
}

/*
** Draw panel borders
*/
void	draw_panel_borders(t_panel *p)
{
	int	x;
	int	y;

	x = 0;
	while (x < p->current_width)
	{
		mlx_put_pixel(p->panel_img, x, 0, 0xFFFFFFFF);
		mlx_put_pixel(p->panel_img, x, p->height - 1, 0xFFFFFFFF);
		x++;
	}
	y = 0;
	while (y < p->height)
	{
		if (p->current_width > 0)
			mlx_put_pixel(p->panel_img, 0, y, 0xFFFFFFFF);
		if (p->current_width > 1)
			mlx_put_pixel(p->panel_img, p->current_width - 1, y, 0xFFFFFFFF);
		y++;
	}
}

/*
** Draw panel content including checkboxes
*/
void	draw_panel_content(t_scene *scene)
{
	t_panel	*p;
	int		i;
	int		x_cb;
	int		y_base;

	p = &scene->ui.panel;
	x_cb = p->padding;
	y_base = p->header_height + p->padding;
	i = 0;
	while (i < NUM_CHECKBOXES)
	{
		draw_single_checkbox(scene, x_cb, y_base, i);
		i++;
	}
}

bool	update_panel_animation(t_panel *p)
{
	if (p->visible)
	{
		if (p->current_width < p->target_width)
		{
			p->current_width += p->animation_speed;
			if (p->current_width > p->target_width)
				p->current_width = p->target_width;
			return (true);
		}
	}
	else
		return (update_panel_closing(p));
	return (false);
}
