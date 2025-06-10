/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_mesh_loader.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/06 16:30:19 by abillote          #+#    #+#             */
/*   Updated: 2025/06/10 15:48:15 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

static void	parse_face_quad(t_triangle *triangles, t_vec3 *vertices,
	int *indices, int *t_idx)
{
	triangles[*t_idx].v0 = vertices[indices[0] - 1];
	triangles[*t_idx].v1 = vertices[indices[2] - 1];
	triangles[*t_idx].v2 = vertices[indices[3] - 1];
	calculate_triangle_normal(&triangles[*t_idx]);
	(*t_idx)++;
}

static void	parse_face_triangle(t_triangle *triangles, t_vec3 *vertices,
	int *indices, int *t_idx)
{
	triangles[*t_idx].v0 = vertices[indices[0] - 1];
	triangles[*t_idx].v1 = vertices[indices[1] - 1];
	triangles[*t_idx].v2 = vertices[indices[2] - 1];
	calculate_triangle_normal(&triangles[*t_idx]);
	(*t_idx)++;
}

void	process_face_line(char *line, t_triangle *triangles,
	t_vec3 *vertices, int *t_idx)
{
	int	indices[4];
	int	count;

	count = sscanf(line, "f %d %d %d %d", &indices[0], &indices[1],
		&indices[2], &indices[3]);
	parse_face_triangle(triangles, vertices, indices, t_idx);
	if (count == 4)
		parse_face_quad(triangles, vertices, indices, t_idx);
}

void	process_vertex_line(char *line, t_vec3 *vertices, int *v_idx)
{
	sscanf(line, "v %lf %lf %lf", &vertices[*v_idx].x,
		&vertices[*v_idx].y, &vertices[*v_idx].z);
	(*v_idx)++;
}

int	check_mesh_error(t_mesh *mesh, char **parts, t_scene *scene)
{
	if (!mesh)
	{
		free_split(parts);
		parse_error(scene, "Failed to load OBJ file");
		return (0);
	}
	return (1);
}
