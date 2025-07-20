/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   surface_colors.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 21:23:35 by antonsplavn       #+#    #+#             */
/*   Updated: 2025/07/20 21:23:40 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/miniRT.h"

/* Get surface color with texture support */
t_color	get_surface_color_with_texture(t_hit_record *hit_record)
{
	t_material	*material;

	material = &hit_record->object->material;
	if (material->has_texture && material->texture)
	{
		return (sample_texture(material->texture,
				hit_record->uv.u, hit_record->uv.v));
	}
	if (material->has_checker)
	{
		return (get_checker_color(*material,
				hit_record->object, hit_record->point));
	}
	return (material->color);
}

/* Get surface color in linear space */
t_color_f	get_surface_color_linear(t_hit_record *hit_record)
{
	t_material	*material;
	t_color		srgb_color;

	material = &hit_record->object->material;
	if (material->has_texture && material->texture)
	{
		return (sample_texture_linear(material->texture,
				hit_record->uv.u, hit_record->uv.v));
	}
	if (material->has_checker)
	{
		srgb_color = get_checker_color(*material,
				hit_record->object, hit_record->point);
	}
	else
	{
		srgb_color = material->color;
	}
	return (color_to_linear(srgb_color));
}
