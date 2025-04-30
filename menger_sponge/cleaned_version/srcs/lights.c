/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lights.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 13:27:34 by abillote          #+#    #+#             */
/*   Updated: 2025/04/30 10:16:09 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "platform.h"

//Note: need to create color struct beforehand
t_light	*create_light(t_vec3 position, double intensity, t_color color)
{
	t_light	*light;

	light = malloc(sizeof(light));
	if (!light)
		return (NULL);
	light->color = color;
	light->intensity = intensity;
	light->position = position;
	light->next = NULL;
	return (light);
}

void	add_light(t_scene *scene, t_light *light)
{
	t_light	*current;

	if (!scene || !light)
		return ;
	if (!scene->lights)
	{
		scene->lights = light;
		return ;
	}
	current = scene->lights;
	while (current->next)
		current = current->next;
	current->next = light;
}
