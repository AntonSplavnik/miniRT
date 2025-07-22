/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_light.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 18:30:00 by abillote          #+#    #+#             */
/*   Updated: 2025/07/18 15:55:47 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

t_light	*create_light(t_vec3 position, double intensity, t_color color)
{
	t_light	*light;

	light = malloc(sizeof(t_light));
	if (!light)
		return (NULL);
	light->color = color;
	light->intensity = intensity;
	light->position = position;
	light->is_area_light = 0;
	light->next = NULL;
	return (light);
}

t_light	*create_area_light(t_vec3 position, t_vec3 normal, double width, \
					double height)
{
	t_light	*light;
	t_vec3	temp;

	light = malloc(sizeof(t_light));
	if (!light)
		return (NULL);
	light->position = position;
	light->normal = vec3_normalize(normal);
	light->width = width;
	light->height = height;
	light->is_area_light = 1;
	temp = vec3_create(0, 1, 0);
	if (fabs(vec3_dot(light->normal, temp)) > 0.9)
		temp = vec3_create(1, 0, 0);
	light->u_axis = vec3_normalize(vec3_cross(light->normal, temp));
	light->v_axis = vec3_cross(light->normal, light->u_axis);
	light->next = NULL;
	return (light);
}

t_light	*create_area_light_with_properties(t_vec3 position, t_vec3 normal, \
								double width, double height, double intensity, \
								char **parts)
{
	t_light	*area_light;
	t_color	color;
	int		parse_color_result;

	parse_color_result = read_color(parts[6], &color);
	free_split(parts);
	if (!parse_color_result)
		parse_error(NULL, "Invalid format for area light color. Expected: r,g,b");
	area_light = create_area_light(position, normal, width, height);
	if (!area_light)
		parse_error(NULL, "Failed to create area light");
	area_light->intensity = intensity;
	area_light->color = color;
	return (area_light);
}

void	add_light(t_scene *scene, t_light *light)
{
	t_light	*current;

	if (!scene || !light)
		return ;
	if (!scene->lights)
	{
		scene->lights = light;
		return ;
	}
	current = scene->lights;
	while (current->next)
		current = current->next;
	current->next = light;
}

t_light	*parse_light_properties(t_scene *scene, char **parts)
{
	t_vec3	position;
	double	intensity;
	t_color	color;
	int		parse_color_result;
	t_light	*new_light;

	if (!read_vector(parts[1], &position))
	{
		free_split(parts);
		parse_error(scene, "Expected for light position: x,y,z");
	}
	intensity = ft_atof(parts[2]);
	if (!check_range(intensity, 0.0, 1.0))
	{
		free_split(parts);
		parse_error(scene, "Light ratio must be between 0.0 and 1.0");
	}
	parse_color_result = read_color(parts[3], &color);
	free_split(parts);
	if (!parse_color_result)
		parse_error(scene, "Invalid format for light color. Expected: r,g,b");
	new_light = create_light(position, intensity, color);
	if (!new_light)
		parse_error(scene, "Failed to create light");
	return (new_light);
}

t_light	*parse_area_light_properties(t_scene *scene, char **parts)
{
	t_vec3	position;
	t_vec3	normal;
	double	width;
	double	height;
	double	intensity;

	if (!read_vector(parts[1], &position))
	{
		free_split(parts);
		parse_error(scene, "Expected for area light position: x,y,z");
	}
	if (!read_vector(parts[2], &normal))
	{
		free_split(parts);
		parse_error(scene, "Expected for area light normal: x,y,z");
	}
	width = ft_atof(parts[3]);
	height = ft_atof(parts[4]);
	intensity = ft_atof(parts[5]);
	return (create_area_light_with_properties(position, normal, width, \
		height, intensity, parts));
}

int	parse_light(t_scene *scene, char *line)
{
	char	**parts;
	t_light	*new_light;

	if (scene->lights)
	{
		parse_error(scene, "Multiple lights not allowed.");
	}
	parts = ft_split_line(line, ' ');
	if (!parts)
		parse_error(scene, "Failed to split line");
	check_parts_count(scene, parts, 4, "light");
	new_light = parse_light_properties(scene, parts);
	add_light(scene, new_light);
	return (1);
}

int	parse_area_light(t_scene *scene, char *line)
{
	char	**parts;
	t_light	*new_area_light;

	parts = ft_split_line(line, ' ');
	if (!parts)
		parse_error(scene, "Failed to split line");
	check_parts_count(scene, parts, 7, "area light");
	new_area_light = parse_area_light_properties(scene, parts);
	add_light(scene, new_area_light);
	return (1);
}
