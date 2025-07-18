/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ui_control_panel.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/14 00:50:00 by antonsplavn       #+#    #+#             */
/*   Updated: 2025/07/18 14:55:28 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

/*
** Helper function to get checkbox state pointer based on index
*/
bool	*get_checkbox_state(t_scene *scene, int idx)
{
	if (idx == 0)
		return (&scene->graphic_settings.enable_hard_shadows);
	if (idx == 1)
		return (&scene->graphic_settings.enable_reflections);
	if (idx == 2)
		return (&scene->graphic_settings.enable_specular);
	if (idx == 3)
		return (&scene->graphic_settings.enable_refraction);
	if (idx == 4)
		return (&scene->graphic_settings.enable_status_message);
	return (NULL);
}

/*
** Clear the panel image by setting all pixels to transparent
*/
void	clear_panel_image(t_panel *panel)
{
	uint32_t	x;
	uint32_t	y;

	if (!panel->panel_img)
		return ;
	y = 0;
	while (y < panel->panel_img->height)
	{
		x = 0;
		while (x < panel->panel_img->width)
		{
			mlx_put_pixel(panel->panel_img, x, y, 0x00000000);
			x++;
		}
		y++;
	}
}

/*
** Initialize toggle button with image and position
*/
void	init_toggle_button(t_scene *scene)
{
	t_toggle_button	*tog;

	tog = &scene->ui.toggle;
	tog->offset_x = TOGGLE_BTN_OFFSET_X;
	tog->offset_y = TOGGLE_BTN_OFFSET_Y;
	tog->size = TOGGLE_BTN_SIZE;
	tog->toggle_img = mlx_new_image(scene->app.mlx, tog->size, tog->size);
	if (!tog->toggle_img)
	{
		if (write(2, "Error: toggle btn img\n", 22))
			exit(1);
		exit(1);
	}
	mlx_image_to_window(scene->app.mlx, tog->toggle_img,
		tog->offset_x, tog->offset_y);
	tog->toggle_img->enabled = true;
}

/*
** Initialize UI panel with default values and create image
*/
void	init_ui_panel(t_scene *scene)
{
	t_panel	*p;

	p = &scene->ui.panel;
	p->visible = false;
	p->current_width = 0;
	p->target_width = PANEL_WIDTH;
	p->height = PANEL_HEIGHT;
	p->x = PANEL_X;
	p->y = PANEL_Y;
	p->animation_speed = 12;
	p->padding = PANEL_PADDING;
	p->header_height = PANEL_HEADER_HEIGHT;
	p->checkbox_size = CHECKBOX_SIZE;
	p->checkbox_spacing = CHECKBOX_SPACING;
	p->text_count = 0;
	p->status_text_img = NULL;
	init_panel_image(scene, p);
}
