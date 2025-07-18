/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/13 22:17:00 by antonsplavn      #+#    #+#             */
/*   Updated: 2025/07/13 22:17:00 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

#ifdef __APPLE__
# define RETINA_SCALE 2.0
#else
# define RETINA_SCALE 1.0
#endif

/*
** Updates mouse button states based on button and action
*/
static void	update_button_state(t_scene *scene, mouse_key_t button,
	action_t action)
{
	if (button == MLX_MOUSE_BUTTON_LEFT)
		scene->mouse_state.left_button_down = (action == MLX_PRESS);
	else if (button == MLX_MOUSE_BUTTON_RIGHT)
		scene->mouse_state.right_button_down = (action == MLX_PRESS);
	else if (button == MLX_MOUSE_BUTTON_MIDDLE)
		scene->mouse_state.middle_button_down = (action == MLX_PRESS);
}

/*
** Handles mouse button press events
*/
static void	handle_button_press(t_scene *scene, mouse_key_t button)
{
	if (button == MLX_MOUSE_BUTTON_LEFT)
		handle_left_press(scene);
	if (button == MLX_MOUSE_BUTTON_RIGHT)
		handle_right_press(scene);
}

/*
** Handles mouse button release events
*/
static void	handle_button_release(t_scene *scene, mouse_key_t button)
{
	if (button == MLX_MOUSE_BUTTON_LEFT)
	{
		scene->mouse_state.is_dragging = false;
		scene->mouse_state.left_button_down = false;
		render_scene(scene);
		display_status(scene);
	}
	if (button == MLX_MOUSE_BUTTON_RIGHT)
	{
		scene->mouse_state.is_dragging = false;
		scene->mouse_state.right_button_down = false;
		scene->graphic_settings.resolution_factor = 1;
		scene->graphic_settings.ssaa_samples = scene->mouse_state.original_ssaa;
		render_scene(scene);
		display_status(scene);
	}
}

/*
** Main mouse button callback function
*/
void	mouse_button_callback(mouse_key_t button, action_t action,
	modifier_key_t mods, void *param)
{
	t_scene	*scene;

	(void)mods;
	scene = (t_scene *)param;
	update_button_state(scene, button, action);
	if (action == MLX_PRESS)
		handle_button_press(scene, button);
	else if (action == MLX_RELEASE)
		handle_button_release(scene, button);
}

/*
** Sets up mouse input hooks for the MLX window
*/
void	setup_mouse_hook(t_scene *scene)
{
	mlx_mouse_hook(scene->app.mlx, mouse_button_callback, scene);
	mlx_cursor_hook(scene->app.mlx, cursor_position_callback, scene);
}
