/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_mesh_faces.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/11 11:40:10 by abillote          #+#    #+#             */
/*   Updated: 2025/07/03 17:15:22 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

//Transform a face of triangles (quad) into two triangles
// First triangle (v0, v2, v3)
// Second triangle (v0, v1, v2)
static void	parse_face_quad(t_triangle *triangles, t_vec3 *vertices, \
								int *indices, int *t_idx)
{
	triangles[*t_idx].v0 = vertices[indices[0] - 1];
	triangles[*t_idx].v1 = vertices[indices[2] - 1];
	triangles[*t_idx].v2 = vertices[indices[3] - 1];
	calculate_triangle_normal(&triangles[*t_idx]);
	(*t_idx)++;
	triangles[*t_idx].v0 = vertices[indices[0] - 1];
	triangles[*t_idx].v1 = vertices[indices[1] - 1];
	triangles[*t_idx].v2 = vertices[indices[2] - 1];
	calculate_triangle_normal(&triangles[*t_idx]);
	(*t_idx)++;
}

//Store triangle from face line
static void	parse_face_triangle(t_triangle *triangles, t_vec3 *vertices,
	int *indices, int *t_idx)
{
	triangles[*t_idx].v0 = vertices[indices[0] - 1];
	triangles[*t_idx].v1 = vertices[indices[1] - 1];
	triangles[*t_idx].v2 = vertices[indices[2] - 1];
	calculate_triangle_normal(&triangles[*t_idx]);
	(*t_idx)++;
}

//Split face line (f v1 v2 v3 or f v1 v2 v3 v4)
//Store the vertices from the array into triangles structures
//If 4 vertices = quad, transform it into two triangles
void	process_face_line(char *line, t_triangle *triangles,
	t_vec3 *vertices, int *t_idx)
{
	char	**values;
	int		indices[4];
	int		i;

	values = ft_split_line(line + 2, ' ');
	if (!values || !values[0] || !values[1] || !values[2])
	{
		if (values)
			free_split(values);
		return ;
	}
	i = 0;
	while (values[i] && i < 4)
	{
		indices[i] = ft_atoi(values[i]);
		i++;
	}
	if (i == 3)
		parse_face_triangle(triangles, vertices, indices, t_idx);
	else if (i == 4)
		parse_face_quad(triangles, vertices, indices, t_idx);
	free_split(values);
}

//Split vertex line and store in the array of vertices at index v_idx
void	process_vertex_line(char *line, t_vec3 *vertices, int *v_idx)
{
	char	**values;

	values = ft_split_line(line + 2, ' ');
	if (!values || !values[0] || !values[1] || !values[2])
	{
		if (values)
			free_split(values);
		return ;
	}
	vertices[*v_idx].x = ft_atof(values[0]);
	vertices[*v_idx].y = ft_atof(values[1]);
	vertices[*v_idx].z = ft_atof(values[2]);
	(*v_idx)++;
	free_split(values);
}
