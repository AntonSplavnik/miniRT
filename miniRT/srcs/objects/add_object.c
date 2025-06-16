/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_object.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 13:25:52 by abillote          #+#    #+#             */
/*   Updated: 2025/05/22 10:56:10 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

//Add an object to the scene's object list
void	add_object(t_scene *scene, t_object *object)
{
	t_object	*current;

	if (!scene || !object)
		return ;
	if (!scene->objects)
	{
		scene->objects = object;
		return ;
	}
	current = scene->objects;
	while (current->next)
		current = current->next;
	current->next = object;
}
