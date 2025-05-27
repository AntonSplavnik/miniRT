/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   close_handler.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 13:08:43 by asplavni          #+#    #+#             */
/*   Updated: 2025/05/23 15:35:00 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/miniRT.h"

void cleanup_scene(t_scene *scene)
{
	t_object *obj;
	t_object *next_obj;
	t_light *light;
	t_light *next_light;

	if (!scene)
		return;
	
	// Clean up UI panel resources
	cleanup_ui_panel(scene);
	
	obj = scene->objects;
	while (obj)
	{
		next_obj = obj->next;
		if (obj->data)
			free(obj->data);
		free(obj);
		obj = next_obj;
	}
	light = scene->lights;
	while (light)
	{
		next_light = light->next;
		free(light);
		light = next_light;
	}
}

