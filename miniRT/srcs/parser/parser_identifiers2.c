/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_identifiers2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 18:30:00 by abillote          #+#    #+#             */
/*   Updated: 2025/05/16 10:54:35 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

int	is_cylinder(char *line)
{
	if (ft_strncmp(line, "cy", 2) == 0)
	{
		if (line[2] == ' ' || line[2] == '\t' || line[2] == '\0')
			return (1);
	}
	return (0);
}

int	is_cone(char *line)
{
	if (ft_strncmp(line, "co", 2) == 0)
	{
		if (line[2] == ' ' || line[2] == '\t' || line[2] == '\0')
			return (1);
	}
	return (0);
}

int	is_triangle(char *line)
{
	if (ft_strncmp(line, "tr", 2) == 0)
	{
		if (line[2] == ' ' || line[2] == '\t' || line[2] == '\0')
			return (1);
	}
	return (0);
}

int	is_cube(char *line)
{
	if (ft_strncmp(line, "cu", 2) == 0)
	{
		if (line[2] == ' ' || line[2] == '\t' || line[2] == '\0')
			return (1);
	}
	return (0);
}

int	is_mesh(char *line)
{
	if (ft_strncmp(line, "me", 2) == 0)
	{
		if (line[2] == ' ' || line[2] == '\t' || line[2] == '\0')
			return (1);
	}
	return (0);
}
