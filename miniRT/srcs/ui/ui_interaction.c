/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_interaction.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 00:41:31 by antonsplavn       #+#    #+#             */
/*   Updated: 2025/07/14 00:41:32 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

/*
** Check if point is inside toggle button
*/
bool	is_point_in_toggle_button(t_toggle_button *tog, int x, int y)
{
	if (x >= tog->offset_x && x <= tog->offset_x + tog->size)
		if (y >= tog->offset_y && y <= tog->offset_y + tog->size)
			return (true);
	return (false);
}

/*
** Check if point is inside checkbox
*/
bool	is_point_in_checkbox(t_panel *p, int x, int y, int idx)
{
	int	cb_x;
	int	cb_y;
	int	s;

	cb_x = p->x + p->padding;
	cb_y = p->y + p->header_height + p->padding + idx * p->checkbox_spacing;
	s = p->checkbox_size;
	if (x >= cb_x && x <= cb_x + s)
		if (y >= cb_y && y <= cb_y + s)
			return (true);
	return (false);
}

/*
** Handle mouse click on UI panel
*/
bool	ui_panel_mouse_click(t_scene *scene, int x, int y)
{
	t_panel			*p;
	t_toggle_button	*tog;

	p = &scene->ui.panel;
	tog = &scene->ui.toggle;
	if (is_point_in_toggle_button(tog, x, y))
	{
		p->visible = !p->visible;
		return (true);
	}
	if (p->current_width < p->target_width / 2)
		return (false);
	return (handle_checkbox_clicks(scene, p, x, y));
}

/*
** Handle checkbox clicks
*/
bool	handle_checkbox_clicks(t_scene *scene, t_panel *p, int x, int y)
{
	int		i;
	bool	*cb;

	i = 0;
	while (i < NUM_CHECKBOXES)
	{
		if (is_point_in_checkbox(p, x, y, i))
		{
			cb = get_checkbox_state(scene, i);
			if (cb)
				*cb = !(*cb);
			return (true);
		}
		i++;
	}
	return (false);
}

/*
** UI animation loop callback
*/
void	ui_animation_loop(void *param)
{
	t_scene			*scene;
	static double	last = 0.0;
	double			now;

	scene = (t_scene *)param;
	now = mlx_get_time();
	if (now - last > 0.016)
	{
		draw_ui_panel(scene);
		last = now;
	}
}
