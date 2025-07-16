/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_panel_helpers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 23:47:23 by antonsplavn       #+#    #+#             */
/*   Updated: 2025/07/14 00:47:05 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

/*
** Calculate checkbox Y position
*/
int	calculate_checkbox_y(t_panel *p, int y_base, int idx)
{
	return (y_base + idx * p->checkbox_spacing);
}

/*
** Draw single checkbox at specified position
*/
void	draw_single_checkbox(t_scene *scene, int x_cb, int y_base, int idx)
{
	t_panel			*p;
	t_checkbox_pos	pos;
	bool			*state;

	p = &scene->ui.panel;
	pos.x = x_cb;
	pos.y = calculate_checkbox_y(p, y_base, idx);
	state = get_checkbox_state(scene, idx);
	pos.checked = *state;
	draw_checkbox(p, p->panel_img, &pos);
}
