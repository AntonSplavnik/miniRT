/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cube_creation.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 13:15:00 by abillote          #+#    #+#             */
/*   Updated: 2025/05/10 13:20:00 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

/**
 * Creates a new cube object
 * 
 * @param center Center position of the cube
 * @param side_length Length of each side of the cube
 * @param color Color of the cube
 * @return Pointer to the created object or NULL if allocation fails
 */
t_object	*create_cube(t_vec3 center, double side_length, t_color color)
{
	t_object	*object;
	t_cube		*cube;

	object = malloc(sizeof(t_object));
	if (!object)
		return (NULL);
	cube = malloc(sizeof(t_cube));
	if (!cube)
	{
		free(object);
		return (NULL);
	}
	cube->center = center;
	cube->side_length = side_length;
	object->data = cube;
	object->material = create_material(color);
	object->type = CUBE;
	object->next = NULL;
	return (object);
} 