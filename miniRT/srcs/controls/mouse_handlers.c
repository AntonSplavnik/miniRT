/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_handlers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 22:19:00 by antonsplavn      #+#    #+#             */
/*   Updated: 2025/07/13 22:19:00 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

/*
** Handles left mouse button press for UI interaction and panel dragging
*/
void	handle_left_press(t_scene *scene)
{
	scene->mouse_state.left_button_down = true;
	mlx_get_mouse_pos(scene->app.mlx, &scene->mouse_state.x,
		&scene->mouse_state.y);
	if (ui_panel_mouse_click(scene, scene->mouse_state.x, \
		scene->mouse_state.y))
	{
		render_scene(scene);
		return ;
	}
	if (scene->mouse_state.x >= scene->ui.panel.x
		&& scene->mouse_state.x <= scene->ui.panel.x
		+ scene->ui.panel.current_width
		&& scene->mouse_state.y >= scene->ui.panel.y
		&& scene->mouse_state.y <= scene->ui.panel.y
		+ scene->ui.panel.header_height)
	{
		scene->mouse_state.is_dragging = true;
		scene->ui.panel.drag_offset_x = scene->mouse_state.x
			- scene->ui.panel.x;
		scene->ui.panel.drag_offset_y = scene->mouse_state.y
			- scene->ui.panel.y;
	}
	else
		scene->mouse_state.is_dragging = false;
}

/*
** Handles right mouse button press for camera rotation
*/
void	handle_right_press(t_scene *scene)
{
	scene->mouse_state.is_dragging = true;
	scene->mouse_state.right_button_down = true;
	scene->graphic_settings.resolution_factor = 4;
	scene->mouse_state.original_ssaa = scene->graphic_settings.ssaa_samples;
	scene->graphic_settings.ssaa_samples = 1;
	mlx_get_mouse_pos(scene->app.mlx, &scene->mouse_state.x,
		&scene->mouse_state.y);
	scene->mouse_state.prev_mouse_x = scene->mouse_state.x;
	scene->mouse_state.prev_mouse_y = scene->mouse_state.y;
}
