/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   material.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 13:22:16 by abillote          #+#    #+#             */
/*   Updated: 2025/06/04 16:06:24 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/miniRT.h"

//create material with default value
t_material	create_material(t_color color)
{
	t_material	material;

	material.color = color;
	material.specular = 0.5;
	material.shininess = 64;
	material.reflectivity = 0.0;
	material.has_checker = 0;
	material.checker_size = 0;
	material.checker_color = create_color(0, 0, 0);
	material.has_texture = 0;
	material.texture = NULL;
	material.has_bump_map = 0;
	material.bump_map = NULL;
	return (material);
}
