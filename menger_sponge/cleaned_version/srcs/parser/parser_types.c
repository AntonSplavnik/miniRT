/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_types.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 18:42:18 by abillote          #+#    #+#             */
/*   Updated: 2025/05/13 18:47:14 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "platform.h"

int	parse_rgb(char **parts, int *r, int *g, int *b)
{
	*r = ft_atoi(parts[0]);
	*g = ft_atoi(parts[1]);
	*b = ft_atoi(parts[2]);
	if (!check_range(*r, 0, 255) || !check_range(*g, 0, 255) || !check_range(*b, 0, 255))
		return (0);
	return (1);
}

int	check_vector_normalization(t_vec3 vector)
{
	if (vector.x < -1.0 || vector.x > 1.0 || vector.y < -1.0 || vector.y > 1.0 || vector.z < -1.0 || vector.z > 1.0)
		return (0);
	return (1);
}

int	read_vector(char *str, t_vec3 *vector)
{
	char	**parts;
	double	x;
	double	y;
	double	z;
	int		parts_count;

	parts = ft_split(str, ',');
	if (!parts)
		return (0);
	parts_count = count_parts(parts);
	if (parts_count != 3)
	{
		free_split(parts);
		return (0);
	}
	x = ft_atof(parts[0]);
	y = ft_atof(parts[1]);
	z = ft_atof(parts[2]);
	free_split(parts);
	*vector = vec3_create(x, y, z);
	return (1);
}

int	read_color(char *str, t_color *color)
{
	char	**parts;
	int		parts_count;
	int		r;
	int		b;
	int		g;
	int		parse_result;

	parts = ft_split(str, ',');
	if (!parts)
		return (0);
	parts_count = 0;
	while (parts[parts_count])
		parts_count++;
	if (parts_count != 3)
	{
		free_split(parts);
		return (0);
	}
	parse_result = parse_rgb(parts, &r, &g, &b);
	free_split(parts);
	if (!parse_result)
		return (0);
	*color = create_color(r, g, b);
	return (1);
}
