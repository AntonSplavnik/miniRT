/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_text_management.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 23:36:52 by antonsplavn       #+#    #+#             */
/*   Updated: 2025/07/14 00:20:20 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

/*
** Handle panel closing animation
*/
bool	update_panel_closing(t_panel *p)
{
	if (p->current_width > 0)
	{
		p->current_width -= p->animation_speed;
		if (p->current_width < 0)
			p->current_width = 0;
		return (true);
	}
	return (false);
}

/*
** Delete all panel text images
*/
void	delete_panel_text(t_scene *scene)
{
	t_panel	*p;
	int		i;

	p = &scene->ui.panel;
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
	p->text_count = 0;
}

/*
** Hide all panel text images
*/
void	hide_panel_text(t_panel *p)
{
	int	i;

	i = 0;
	while (i < p->text_count)
	{
		if (p->panel_text[i])
			p->panel_text[i]->enabled = false;
		i++;
	}
}

/*
** Show all panel text images
*/
void	show_panel_text(t_panel *p)
{
	int	i;

	i = 0;
	while (i < p->text_count)
	{
		if (p->panel_text[i])
			p->panel_text[i]->enabled = true;
		i++;
	}
}

/*
** Draw panel text including title and checkbox labels
*/
void	draw_panel_text(t_scene *scene)
{
	t_panel	*p;

	p = &scene->ui.panel;
	delete_panel_text(scene);
	draw_panel_title(scene, p);
	draw_checkbox_labels(scene, p);
	show_panel_text(p);
}
