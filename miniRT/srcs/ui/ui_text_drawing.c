/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_text_drawing.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 23:37:00 by antonsplavn       #+#    #+#             */
/*   Updated: 2025/07/13 23:38:28 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

/*
** Draw panel title
*/
void	draw_panel_title(t_scene *scene, t_panel *p)
{
	int	title_w;
	int	title_x;
	int	title_y;

	title_w = 8 * 8;
	title_x = (p->target_width - title_w) / 2;
	title_y = p->header_height / 2 - 8;
	p->panel_text[0] = mlx_put_string(scene->app.mlx, "Settings", \
		p->x + title_x, p->y + title_y);
	p->text_offset_x[0] = title_x;
	p->text_offset_y[0] = title_y;
	p->text_count = 1;
}

/*
** Draw checkbox labels
*/
void	draw_checkbox_labels(t_scene *scene, t_panel *p)
{
	int		i;
	int		label_x;
	int		label_y;
	int		y_base;
	char	*label;

	label_x = p->padding + p->checkbox_size + 12;
	y_base = p->header_height + p->padding;
	i = 0;
	while (i < NUM_CHECKBOXES)
	{
		label = get_checkbox_label(i);
		label_y = y_base + i * p->checkbox_spacing;
		p->panel_text[p->text_count] = mlx_put_string(scene->app.mlx, \
			label, p->x + label_x, p->y + label_y);
		p->text_offset_x[p->text_count] = label_x;
		p->text_offset_y[p->text_count] = label_y;
		p->text_count++;
		i++;
	}
}

/*
** Get checkbox label by index
*/
char	*get_checkbox_label(int idx)
{
	if (idx == 0)
		return ("Shadows");
	if (idx == 1)
		return ("Reflections");
	if (idx == 2)
		return ("Specular");
	if (idx == 3)
		return ("Refraction");
	if (idx == 4)
		return ("Status Message");
	return ("Unknown");
}

/*
** Main UI panel drawing function
*/
bool	draw_ui_panel(t_scene *scene)
{
	t_panel	*p;
	bool	anim;

	p = &scene->ui.panel;
	anim = update_panel_animation(p);
	clear_panel_image(p);
	if (p->current_width > 0)
	{
		draw_panel_background(p);
		draw_panel_content(scene);
	}
	handle_panel_text_display(scene, p);
	return (anim);
}

/*
** Handle panel text display based on animation state
*/
void	handle_panel_text_display(t_scene *scene, t_panel *p)
{
	if (p->current_width == p->target_width)
	{
		if (p->text_count == 0)
			draw_panel_text(scene);
		else
			show_panel_text(p);
	}
	else
	{
		hide_panel_text(p);
		if (p->current_width == 0 && p->text_count > 0)
			delete_panel_text(scene);
	}
}
