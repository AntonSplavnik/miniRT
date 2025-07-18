/* ************************************************************************** */
/*	                                                                        */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: Invalid date        by                   #+#    #+#             */
/*   Updated: 2025/07/18 12:37:10 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../includes/miniRT.h"

void free_mesh(t_mesh *mesh)
{
	if (!mesh) return;
	free(mesh->triangles);
	free(mesh->transformed_tris);
	free(mesh);
}

void cleanup_scene(t_scene *scene)
{
	t_object *obj;
	t_object *next_obj;
	t_light *light;
	t_light *next_light;

	if (!scene)
		return;
	cleanup_ui_panel(scene);
	obj = scene->objects;
	while (obj)
	{
		next_obj = obj->next;
		if (obj->type == MESH)
			free_mesh((t_mesh *)obj->data);
		else if (obj->data)
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
