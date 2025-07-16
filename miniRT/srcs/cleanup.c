/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 13:08:43 by asplavni          #+#    #+#             */
/*   Updated: 2025/07/13 23:35:36 by antonsplavn      ###   ########.fr       */
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
		if (obj->material.texture)
			free_texture_mlx(obj->material.texture);
		if (obj->material.bump_map)
			free_bump_map(obj->material.bump_map);
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
