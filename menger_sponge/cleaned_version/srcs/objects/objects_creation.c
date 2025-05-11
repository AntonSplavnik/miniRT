/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   objects_creation.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 13:25:52 by abillote          #+#    #+#             */
/*   Updated: 2025/04/30 10:06:28 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "platform.h"

//Note: Due to args limitation (norminette), we need to modify the color later, after cylinder creation
t_object	*create_cylinder(t_vec3 center, t_vec3 axis, double diameter, double height)
{
	t_object	*object;
	t_cylinder	*cylinder;

	object = malloc(sizeof(t_object));
	if (!object)
		return (NULL);
	cylinder = malloc(sizeof(t_cylinder));
	if (!cylinder)
	{
		free(object);
		return (NULL);
	}
	cylinder->center = center;
	cylinder->axis = vec3_normalize(axis);
	cylinder->diameter = diameter;
	cylinder->radius = diameter / 2.0;
	cylinder->height = height;
	object->data = cylinder;
	object->material = create_material(create_color(255, 255, 255));
	object->type = CYLINDER;
	object->next = NULL;
	return (object);
}

t_object	*create_plane(t_vec3 point, t_vec3 normal, t_color color)
{
	t_object	*object;
	t_plane		*plane;

	object = malloc(sizeof(t_object));
	if (!object)
		return (NULL);
	plane = malloc(sizeof(t_plane));
	if (!plane)
	{
		free(object);
		return (NULL);
	}
	plane->normal = vec3_normalize(normal);
	plane->point = point;
	object->data = plane;
	object->material = create_material(color);
	object->type = PLANE;
	object->next = NULL;
	return (object);
}

t_object	*create_sphere(t_vec3 center, double diameter, t_color color)
{
	t_object	*object;
	t_sphere	*sphere;

	object = malloc(sizeof(t_object));
	if (!object)
		return (NULL);
	sphere = malloc(sizeof(t_sphere));
	if (!sphere)
	{
		free(object);
		return (NULL);
	}
	sphere->center = center;
	sphere->diameter = diameter;
	sphere->radius = diameter / 2.0;
	object->type = SPHERE;
	object->material = create_material(color);
	object->data = sphere;
	object->next = NULL;
	return (object);
}

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
