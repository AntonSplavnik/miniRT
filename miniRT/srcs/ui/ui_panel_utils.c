/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_panel_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 23:38:46 by antonsplavn       #+#    #+#             */
/*   Updated: 2025/07/18 14:55:41 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

/*
** Initialize panel image and add to window
*/
void	init_panel_image(t_scene *scene, t_panel *p)
{
	int	i;

	p->panel_img = mlx_new_image(scene->app.mlx, p->target_width, p->height);
	if (!p->panel_img)
	{
		if (write(2, "Error: panel img\n", 17))
			exit(1);
		exit(1);
	}
	mlx_image_to_window(scene->app.mlx, p->panel_img, p->x, p->y);
	p->panel_img->enabled = true;
	i = 0;
	while (i < MAX_PANEL_TEXT)
	{
		p->panel_text[i] = NULL;
		i++;
	}
}

/*
** Cleanup UI panel and toggle button images
*/
void	cleanup_ui_panel(t_scene *scene)
{
	t_panel			*p;
	t_toggle_button	*tog;

	p = &scene->ui.panel;
	tog = &scene->ui.toggle;
	if (tog->toggle_img)
	{
		mlx_delete_image(scene->app.mlx, tog->toggle_img);
		tog->toggle_img = NULL;
	}
	cleanup_panel_images(scene, p);
	p->visible = false;
	p->current_width = 0;
}

/*
** Clean up all panel text images
*/
void	cleanup_panel_images(t_scene *scene, t_panel *p)
{
	int	i;

	if (p->panel_img)
	{
		mlx_delete_image(scene->app.mlx, p->panel_img);
		p->panel_img = NULL;
	}
	i = 0;
	while (i < p->text_count)
	{
		if (p->panel_text[i])
		{
			mlx_delete_image(scene->app.mlx, p->panel_text[i]);
			p->panel_text[i] = NULL;
		}
		i++;
	}
	cleanup_status_text(scene, p);
}

/*
** Clean up status text image
*/
void	cleanup_status_text(t_scene *scene, t_panel *p)
{
	p->text_count = 0;
	if (p->status_text_img)
	{
		mlx_delete_image(scene->app.mlx, p->status_text_img);
		p->status_text_img = NULL;
	}
}
