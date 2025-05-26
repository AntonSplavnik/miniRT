/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone_creation.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 13:25:52 by abillote          #+#    #+#             */
/*   Updated: 2025/05/20 09:53:18 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/miniRT.h"

t_object	*create_cone(t_vec3 apex, t_vec3 axis, double radius, double height)
{
	t_object	*object;
	t_cone		*cone;

	object = malloc(sizeof(t_object));
	if (!object)
		return (NULL);
	cone = malloc(sizeof(t_cone));
	if (!cone)
	{
		free(object);
		return (NULL);
	}
	cone->apex = apex;
	cone->axis = vec3_normalize(axis);
	cone->radius = radius;
	cone->height = height;
	cone->angle = atan2(cone->radius, cone->height);
	object->data = cone;
	object->material = create_material(create_color(255, 255, 255));
	object->type = CONE;
	object->next = NULL;
	return (object);
}
