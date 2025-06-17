/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_identifiers.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 18:30:00 by abillote          #+#    #+#             */
/*   Updated: 2025/05/13 18:30:14 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

int	is_ambient(char *line)
{
	if (line[0] == 'A')
	{
		if (line[1] == ' ' || line[1] == '\t' || line[1] == '\0')
			return (1);
	}
	return (0);
}

int	is_camera(char *line)
{
	if (line[0] == 'C')
	{
		if (line[1] == ' ' || line[1] == '\t' || line[1] == '\0')
			return (1);
	}
	return (0);
}

int	is_light(char *line)
{
	if (line[0] == 'L')
	{
		if (line[1] == ' ' || line[1] == '\t' || line[1] == '\0')
			return (1);
	}
	return (0);
}

int	is_sphere(char *line)
{
	if (ft_strncmp(line, "sp", 2) == 0)
	{
		if (line[2] == ' ' || line[2] == '\t' || line[2] == '\0')
			return (1);
	}
	return (0);
}

int	is_plane(char *line)
{
	if (ft_strncmp(line, "pl", 2) == 0)
	{
		if (line[2] == ' ' || line[2] == '\t' || line[2] == '\0')
			return (1);
	}
	return (0);
}
