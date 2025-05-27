/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   triangle_creation.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 14:50:00 by abillote          #+#    #+#             */
/*   Updated: 2025/05/10 15:00:00 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/miniRT.h"

/**
 * Creates a new triangle object
 * 
 * @param v0 First vertex
 * @param v1 Second vertex
 * @param v2 Third vertex
 * @param color Color of the triangle
 * @return Pointer to the created object or NULL if allocation fails
 */
t_object	*create_triangle(t_vec3 v0, t_vec3 v1, t_vec3 v2, t_color color)
{
	t_object	*object;
	t_triangle	*triangle;

	object = malloc(sizeof(t_object));
	if (!object)
		return (NULL);
	triangle = malloc(sizeof(t_triangle));
	if (!triangle)
	{
		free(object);
		return (NULL);
	}
	triangle->v0 = v0;
	triangle->v1 = v1;
	triangle->v2 = v2;
	
	// Precompute normal for efficiency
	t_vec3 edge1 = vec3_subtract(v1, v0);
	t_vec3 edge2 = vec3_subtract(v2, v0);
	triangle->normal = vec3_normalize(vec3_cross(edge1, edge2));
	
	object->data = triangle;
	object->material = create_material(color);
	object->type = TRIANGLE;
	object->next = NULL;
	return (object);
} 