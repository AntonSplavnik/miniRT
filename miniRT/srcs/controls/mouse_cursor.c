/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_cursor.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 22:18:00 by antonsplavn      #+#    #+#             */
/*   Updated: 2025/07/13 22:18:00 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

/*
** Clamps a value between min and max
*/
int	clamp(int val, int min, int max)
{
	if (val < min)
		return (min);
	if (val > max)
		return (max);
	return (val);
}

/*
** Handles panel dragging when left mouse button is held
*/
void	handle_panel_drag(t_scene *scene, double xpos, double ypos)
{
	t_panel	*p;
	int		i;

	p = &scene->ui.panel;
	p->x = clamp((int)xpos - p->drag_offset_x, 0, \
		WIDTH - p->panel_img->width);
	p->y = clamp((int)ypos - p->drag_offset_y, 0, \
		HEIGHT - p->panel_img->height);
	p->panel_img->instances[0].x = p->x;
	p->panel_img->instances[0].y = p->y;
	i = 0;
	while (i < p->text_count)
	{
		if (p->panel_text[i])
		{
			p->panel_text[i]->instances[0].x = p->x + p->text_offset_x[i];
			p->panel_text[i]->instances[0].y = p->y + p->text_offset_y[i];
		}
		i++;
	}
}

/*
** Handles camera rotation when right mouse button is held
*/
void	handle_camera_rotation(t_scene *scene)
{
	static double	last_render_time = 0;
	double			current_time;
	double			dx;
	double			dy;

	dx = (double)scene->mouse_state.x - scene->mouse_state.prev_mouse_x;
	dy = (double)scene->mouse_state.y - scene->mouse_state.prev_mouse_y;
	if (dx != 0.0 || dy != 0.0)
	{
		scene->camera.rotation.y += dx * 0.005;
		scene->camera.rotation.x += dy * 0.005;
		update_camera_vectors(scene);
		scene->mouse_state.prev_mouse_x = scene->mouse_state.x;
		scene->mouse_state.prev_mouse_y = scene->mouse_state.y;
		current_time = mlx_get_time();
		if (current_time - last_render_time > 0.033)
		{
			render_scene(scene);
			display_status(scene);
			last_render_time = current_time;
		}
	}
}

/*
** Main cursor position callback function
*/
void	cursor_position_callback(double xpos, double ypos, void *param)
{
	t_scene	*scene;

	scene = (t_scene *)param;
	scene->mouse_state.x = (int32_t)xpos;
	scene->mouse_state.y = (int32_t)ypos;
	if (scene->mouse_state.left_button_down && scene->mouse_state.is_dragging)
		handle_panel_drag(scene, xpos, ypos);
	if (scene->mouse_state.right_button_down && scene->mouse_state.is_dragging)
		handle_camera_rotation(scene);
}
