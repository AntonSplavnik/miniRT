/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 18:30:00 by abillote          #+#    #+#             */
/*   Updated: 2025/05/23 11:49:45 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "platform.h"

void	free_split(char **str)
{
	int	i;

	i = 0;
	while (str[i])
		free(str[i++]);
	free(str);
}

void	parse_error(t_scene *scene, char *message)
{
	cleanup_scene(scene);
	write_string_to_file_descriptor("Error\n", STDERR_FILENO);
	write_string_to_file_descriptor(message, STDERR_FILENO);
	write_string_to_file_descriptor("\n", STDERR_FILENO);
	exit(EXIT_FAILURE);
}

int	check_range(double value, double min, double max)
{
	if (value < min || value > max)
		return (0);
	return (1);
}

int	count_parts(char **parts)
{
	int	count;

	count = 0;
	while (parts[count])
		count++;
	return (count);
}

void	check_parts_count(t_scene *scene, char **parts, int expected_count, char *element_name)
{
	int	parts_count;

	parts_count = count_parts(parts);
	if (parts_count != expected_count)
	{
		free_split(parts);
		if (ft_strcmp(element_name, "camera") == 0)
			parse_error(scene, "Invalid format for camera. Expected: C x,y,z nx,ny,nz fov");
		else if (ft_strcmp(element_name, "ambient") == 0)
			parse_error(scene, "Invalid format for ambient lighting. Expected: A ratio r,g,b");
		else if (ft_strcmp(element_name, "light") == 0)
			parse_error(scene, "Invalid format for light. Expected: L x,y,z ratio r,g,b");
		else if (ft_strcmp(element_name, "sphere") == 0)
			parse_error(scene, "Invalid format for sphere. Expected: sp x,y,z r r,g,b");
		else if (ft_strcmp(element_name, "plane") == 0)
			parse_error(scene, "Invalid format for plane. Expected: pl x,y,z nx,ny,nz r,g,b");
		else if (ft_strcmp(element_name, "cylinder") == 0)
			parse_error(scene, "Invalid format for cylinder. Expected: cy x,y,z nx,ny,nz d h r,g,b");
		else if (ft_strcmp(element_name, "cone") == 0)
			parse_error(scene, "Invalid format for cone. Expected: co x,y,z nx,ny,nz radius height r,g,b");
		else if (ft_strcmp(element_name, "triangle") == 0)
			parse_error(scene, "Invalid format for triangle. Expected: tr x1,y1,z1 x2,y2,z2 x3,y3,z3 r,g,b");
		else if (ft_strcmp(element_name, "cube") == 0)
			parse_error(scene, "Invalid format for cube. Expected: cu x,y,z side_length r,g,b");
		else if (ft_strcmp(element_name, "mesh") == 0)
			parse_error(scene, "Invalid format for mesh. Expected: me filename x,y,z nx,ny,nz s r,g,b");
	}
}

