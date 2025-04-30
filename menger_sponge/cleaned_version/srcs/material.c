/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   material.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 13:22:16 by abillote          #+#    #+#             */
/*   Updated: 2025/04/29 13:23:23 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "platform.h"

//create material with default value - to improve with bonuses
t_material	create_material(t_color color)
{
	t_material	material;

	material.color = color;
	material.specular = 0.0;
	material.shininess = 0.0;
	material.reflectivity = 0.0;
	material.checkerboard = 0;
	material.checker_color = create_color(0, 0, 0);
	return (material);
}
