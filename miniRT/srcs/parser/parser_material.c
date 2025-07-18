/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_material.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 12:20:00 by abillote          #+#    #+#             */
/*   Updated: 2025/07/18 15:17:44 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

t_material	create_material(t_color color)
{
	t_material	material;

	material.color = color;
	material.specular = 0.5;
	material.shininess = 64;
	material.reflectivity = 0.0;
	material.transparency = 0.0;
	material.refractive_index = 1.0;
	material.has_refraction = 0;
	material.has_checker = 0;
	material.checker_size = 0;
	material.checker_color = create_color(0, 0, 0);
	material.has_texture = 0;
	material.texture = NULL;
	material.has_bump_map = 0;
	material.bump_map = NULL;
	return (material);
}

/**
 * Extracts the material block (if any) from a line in the RT file
 * The material block is expected to be in the format:
 * {property1:value1 property2:value2 ...}
 *
 * @param line The full line from the RT file
 * @return A newly allocated string containing just the material block,
 * or NULL if none found
 */
char	*extract_material_block(char *line)
{
	char	*start;
	char	*end;
	char	*material_block;
	int		length;

	start = ft_strchr(line, '{');
	if (!start)
		return (NULL);
	end = ft_strchr(start, '}');
	if (!end)
		return (NULL);
	length = end - start + 1;
	material_block = (char *)malloc(length + 1);
	if (!material_block)
		return (NULL);
	ft_strlcpy(material_block, start, length + 1);
	return (material_block);
}

/**
 * Trims the material block from a line, leaving only the standard RT format
 *
 * @param line The line to trim
 */
void	trim_material_block(char *line)
{
	char	*start;

	start = ft_strchr(line, '{');
	if (start)
		*start = '\0';
}

void	parse_checker_color(t_scene *scene, t_color *color, \
							const char *color_pos, int length)
{
	char	*color_str;
	int		result;

	color_str = (char *)malloc(length + 1);
	if (!color_str)
		parse_error(scene, "Not able to allocate memory for checker color");
	ft_strlcpy(color_str, color_pos, length + 1);
	result = read_color(color_str, color);
	free(color_str);
	if (!result)
		parse_error(scene, "Expected color format in material block: r,g,b");
}
