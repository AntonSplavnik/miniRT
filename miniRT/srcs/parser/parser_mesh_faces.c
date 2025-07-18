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
								int *indices, int *t_idx, t_vec3 *normals, int *normal_indices)
{
	triangles[*t_idx].v0 = vertices[indices[0] - 1];
	triangles[*t_idx].v1 = vertices[indices[2] - 1];
	triangles[*t_idx].v2 = vertices[indices[3] - 1];
	
	// Assign correct normals based on normal indices from face definition (first triangle of quad)
	if (normals && normal_indices[0] > 0 && normal_indices[2] > 0 && normal_indices[3] > 0)
	{
		triangles[*t_idx].n0 = normals[normal_indices[0] - 1];
		triangles[*t_idx].n1 = normals[normal_indices[2] - 1];
		triangles[*t_idx].n2 = normals[normal_indices[3] - 1];
		triangles[*t_idx].has_vertex_normals = 1;
	}
	else
	{
		triangles[*t_idx].has_vertex_normals = 0;
	}
	
	calculate_triangle_normal(&triangles[*t_idx]);
	(*t_idx)++;
	
	triangles[*t_idx].v0 = vertices[indices[0] - 1];
	triangles[*t_idx].v1 = vertices[indices[1] - 1];
	triangles[*t_idx].v2 = vertices[indices[2] - 1];
	
	// Assign correct normals based on normal indices from face definition
	if (normals && normal_indices[0] > 0 && normal_indices[1] > 0 && normal_indices[2] > 0)
	{
		triangles[*t_idx].n0 = normals[normal_indices[0] - 1];
		triangles[*t_idx].n1 = normals[normal_indices[1] - 1];
		triangles[*t_idx].n2 = normals[normal_indices[2] - 1];
		triangles[*t_idx].has_vertex_normals = 1;
	}
	else
	{
		triangles[*t_idx].has_vertex_normals = 0;
	}
	
	calculate_triangle_normal(&triangles[*t_idx]);
	(*t_idx)++;
}

//Store triangle from face line
static void	parse_face_triangle(t_triangle *triangles, t_vec3 *vertices,
	int *indices, int *t_idx, t_vec3 *normals, int *normal_indices)
{
	triangles[*t_idx].v0 = vertices[indices[0] - 1];
	triangles[*t_idx].v1 = vertices[indices[1] - 1];
	triangles[*t_idx].v2 = vertices[indices[2] - 1];
	
	// Assign correct normals based on normal indices from face definition
	if (normals && normal_indices[0] > 0 && normal_indices[1] > 0 && normal_indices[2] > 0)
	{
		triangles[*t_idx].n0 = normals[normal_indices[0] - 1];
		triangles[*t_idx].n1 = normals[normal_indices[1] - 1];
		triangles[*t_idx].n2 = normals[normal_indices[2] - 1];
		triangles[*t_idx].has_vertex_normals = 1;
		
	}
	else
	{
		triangles[*t_idx].has_vertex_normals = 0;
	}
	
	calculate_triangle_normal(&triangles[*t_idx]);
	(*t_idx)++;
}

//Split face line (f v1 v2 v3 or f v1 v2 v3 v4)
//Store the vertices from the array into triangles structures
//If 4 vertices = quad, transform it into two triangles
void	process_face_line(char *line, t_triangle *triangles,
	t_vec3 *vertices, int *t_idx, t_vec3 *normals)
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
	int normal_indices[4] = {-1, -1, -1, -1};
	while (values[i] && i < 4)
	{
		// Parse vertex/texture/normal format (e.g., "1/1/1")
		char *vertex_part = values[i];
		char *normal_part = NULL;
		
		// Find the normal index after the second '/'
		char *first_slash = ft_strchr(vertex_part, '/');
		if (first_slash) {
			char *second_slash = ft_strchr(first_slash + 1, '/');
			if (second_slash) {
				normal_part = second_slash + 1;
				normal_indices[i] = ft_atoi(normal_part);
			}
		}
		
		
		indices[i] = ft_atoi(values[i]);
		i++;
	}
	if (i == 3)
		parse_face_triangle(triangles, vertices, indices, t_idx, normals, normal_indices);
	else if (i == 4)
		parse_face_quad(triangles, vertices, indices, t_idx, normals, normal_indices);
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

void	process_normal_line(char *line, t_vec3 *normals, int *n_idx)
{
	char	**values;

	values = ft_split_line(line + 3, ' ');
	if (!values || !values[0] || !values[1] || !values[2])
	{
		if (values)
			free_split(values);
		return ;
	}
	normals[*n_idx].x = ft_atof(values[0]);
	normals[*n_idx].y = ft_atof(values[1]);
	normals[*n_idx].z = ft_atof(values[2]);
	(*n_idx)++;
	free_split(values);
}
