/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_material_properties.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 13:46:37 by abillote          #+#    #+#             */
/*   Updated: 2025/07/18 15:17:44 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

int	get_property_color(t_scene *scene, const char *material_block, \
					const char *property, t_color *color)
{
	char	*property_pos;
	char	*end_pos;
	int		length;

	property_pos = ft_strstr(material_block, property);
	if (!property_pos)
		return (0);
	property_pos += ft_strlen(property);
	end_pos = ft_strchr(property_pos, ' ');
	if (!end_pos)
		end_pos = ft_strchr(property_pos, '}');
	if (!end_pos)
		parse_error(scene, "Invalid color format in material block");
	length = end_pos - property_pos;
	if (length <= 0)
		parse_error(scene, "Empty color value in material block");
	parse_checker_color(scene, color, property_pos, length);
	return (1);
}

static void	parse_reflectivity_transparency(char *material_block, \
											t_material *material)
{
	double	value;

	value = get_property_value(material_block, "reflectivity:");
	if (value >= 0.0)
		material->reflectivity = ft_clamp(value, 0.0, 1.0);
	value = get_property_value(material_block, "transparency:");
	if (value >= 0.0)
		material->transparency = ft_clamp(value, 0.0, 1.0);
	value = get_property_value(material_block, "refractive_index:");
	if (value >= 0.0)
		material->refractive_index = value;
	if (material->transparency > 0.0 && material->refractive_index > 0.0)
		material->has_refraction = true;
}

static void	parse_specular_shininess(char *material_block, t_material *material)
{
	double	value;

	value = get_property_value(material_block, "specular:");
	if (value >= 0.0)
		material->specular = ft_clamp(value, 0.0, 1.0);
	value = get_property_value(material_block, "shininess:");
	if (value >= 0.0)
		material->shininess = value;
}

static void	parse_checker_properties(t_scene *scene, char *material_block, \
									t_material *material)
{
	double	value;
	t_color	checker_color;

	value = get_property_value(material_block, "checker_size:");
	if (value > 0)
		material->checker_size = value;
	if (get_property_color(scene, material_block, "checker_color:", \
						&checker_color))
	{
		material->checker_color = checker_color;
		material->has_checker = 1;
	}
}

void	parse_material_properties(t_scene *scene, char *material_block, \
								t_material *material)
{
	parse_reflectivity_transparency(material_block, material);
	parse_specular_shininess(material_block, material);
	parse_checker_properties(scene, material_block, material);
	get_texture_bump_map(scene, material_block, material);
	free(material_block);
}
