/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 16:30:17 by abillote          #+#    #+#             */
/*   Updated: 2025/07/18 16:30:23 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/miniRT.h"

/* Free mesh data structure */
void	free_mesh(t_mesh *mesh)
{
	if (!mesh)
		return ;
	free(mesh->triangles);
	free(mesh->transformed_tris);
	free(mesh);
}

/* Free object data and materials */
void	cleanup_object(t_object *obj)
{
	if (obj->type == MESH)
		free_mesh((t_mesh *)obj->data);
	else if (obj->data)
		free(obj->data);
	if (obj->material.texture)
		free_texture_mlx(obj->material.texture);
	if (obj->material.bump_map)
		free_bump_map(obj->material.bump_map);
	free(obj);
}

/* Free all objects in the scene */
void	cleanup_objects(t_scene *scene)
{
	t_object	*obj;
	t_object	*next_obj;

	obj = scene->objects;
	while (obj)
	{
		next_obj = obj->next;
		cleanup_object(obj);
		obj = next_obj;
	}
}

/* Free all lights in the scene */
void	cleanup_lights(t_scene *scene)
{
	t_light	*light;
	t_light	*next_light;

	light = scene->lights;
	while (light)
	{
		next_light = light->next;
		free(light);
		light = next_light;
	}
}

/* Clean up entire scene */
void	cleanup_scene(t_scene *scene)
{
	if (!scene)
		return ;
	cleanup_ui_panel(scene);
	cleanup_objects(scene);
	cleanup_lights(scene);
}
