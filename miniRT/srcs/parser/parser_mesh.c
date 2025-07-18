/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_mesh.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 10:19:44 by abillote          #+#    #+#             */
/*   Updated: 2025/07/18 14:36:54 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

t_mesh	*create_mesh(t_triangle *triangles, int triangle_count)
{
	t_mesh	*mesh;

	mesh = malloc(sizeof(t_mesh));
	if (!mesh)
		return (NULL);
	mesh->triangles = triangles;
	mesh->triangle_count = triangle_count;
	mesh->position = (t_vec3){0, 0, 0};
	mesh->rotation = (t_vec3){0, 0, 0};
	mesh->scale = (t_vec3){1, 1, 1};
	return (mesh);
}

t_object	*create_mesh_object(t_mesh *mesh, t_color color)
{
	t_object	*mesh_obj;

	mesh_obj = malloc(sizeof(t_object));
	if (!mesh_obj)
		return (NULL);
	mesh_obj->type = MESH;
	mesh_obj->data = mesh;
	mesh_obj->material = create_material(color);
	mesh_obj->next = NULL;
	return (mesh_obj);
}

void	parse_mesh_vectors(t_scene *scene, char **parts, \
							t_vec3 *position, t_vec3 *rotation)
{
	if (!read_vector(parts[2], position))
	{
		free_split(parts);
		parse_error(scene, "Expected format for mesh position: x,y,z");
	}
	if (!read_vector(parts[3], rotation))
	{
		free_split(parts);
		parse_error(scene, "Expected format for mesh rotation: nx,ny,nz");
	}
}

void	parse_mesh_scale_color(t_scene *scene, char **parts, \
								t_vec3 *scale, t_color *color)
{
	double	scale_value;

	scale_value = ft_atof(parts[4]);
	if (scale_value <= 0)
	{
		free_split(parts);
		parse_error(scene, "Expected format for mesh scale: positive number");
	}
	*scale = (t_vec3){scale_value, scale_value, scale_value};
	if (!read_color(parts[5], color))
	{
		free_split(parts);
		parse_error(scene, "Invalid format for mesh color. Expected: r,g,b");
	}
}

int	parse_mesh(t_scene *scene, char *line)
{
	char		**parts;
	t_color		color;
	t_mesh		*mesh;
	t_object	*mesh_obj;

	parts = ft_split_line(line, ' ');
	if (!parts)
		return (0);
	check_parts_count(scene, parts, 6, "mesh");
	mesh = load_obj_file(parts[1]);
	if (!mesh)
	{
		free_split(parts);
		parse_error(scene, "Failed to load OBJ file");
	}
	parse_mesh_vectors(scene, parts, &mesh->position, &mesh->rotation);
	parse_mesh_scale_color(scene, parts, &mesh->scale, &color);
	precompute_transformed_triangles(mesh);
	mesh_obj = create_mesh_object(mesh, color);
	add_object(scene, mesh_obj);
	free_split(parts);
	return (1);
}
