/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   material.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 13:22:16 by abillote          #+#    #+#             */
/*   Updated: 2025/05/21 16:33:38 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"

//create material with default value
t_material	create_material(t_color color)
{
	t_material	material;

	material.color = color;
	material.specular = 0.2;
	material.shininess = 32;
	material.reflectivity = 0.0;
	material.refractive_index = 1.0;
	material.transparency = 0.0;
	material.has_checker = 0;
	material.checker_size = 0;
	material.checker_color = create_color(0, 0, 0);
	return (material);
}
