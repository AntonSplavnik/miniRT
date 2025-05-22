/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_resolution.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 13:53:08 by abillote          #+#    #+#             */
/*   Updated: 2025/05/22 14:29:35 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "platform.h"

int	parse_resolution(t_scene *scene, char *line)
{
	int		width;
	int		height;
	char	**parts;
	int		parts_count;

	parts = ft_split(line, ' ');
	if (!parts)
		return (0);
	parts_count = count_parts(parts);
	if (parts_count != 3)
	{
		free_split(parts);
		parse_error(scene, "Invalid format for resolution. Expected: R width height");
	}
	width = ft_atoi(parts[1]);
	height = ft_atoi(parts[2]);
	free_split(parts);
	if(width > MAX_RESOLUTION || height > MAX_RESOLUTION)
	{
		parse_error(scene, "Resolution exceeds maximum allowed value");
	}
	scene->width = width;
	scene->height = height;
	scene->img.width = width;
	scene->img.height = height;
	return (1);
}
