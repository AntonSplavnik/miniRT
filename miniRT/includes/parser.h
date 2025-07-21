/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 16:09:12 by abillote          #+#    #+#             */
/*   Updated: 2025/07/21 11:21:33 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

# include "types.h"

# define MAX_LINE_LEN 8192

// Utility functions (parser_utils.c)
size_t		ft_strlcpy(char *dst, const char *src, size_t dstsize);
void		free_split(char **split);
void		parse_error(t_scene *scene, char *message);
int			check_range(double value, double min, double max);
int			count_parts(char **parts);

// Error checking (parser_error.c)
void		check_parts_count(t_scene *scene, char **parts, \
				int expected, char *element);

// Identifier functions (parser_identifiers.c)
int			is_ambient(char *line);
int			is_camera(char *line);
int			is_light(char *line);
int			is_sphere(char *line);
int			is_plane(char *line);
int			is_cylinder(char *line);
int			is_cone(char *line);
int			is_triangle(char *line);
int			is_cube(char *line);
int			is_mesh(char *line);

// Vector and color parsing (parser_vectors.c)
int			read_vector(char *str, t_vec3 *vector);
int			read_color(char *str, t_color *color);
int			check_vector_normalization(t_vec3 vector);
int			parse_rgb(char **parts, int *r, int *g, int *b);

// Object parsing functions (parser_objects.c)
int			parse_ambient(t_scene *scene, char *line);
int			parse_camera(t_scene *scene, char *line);
int			parse_light(t_scene *scene, char *line);
int			parse_sphere(t_scene *scene, char *line);
int			parse_plane(t_scene *scene, char *line);
int			parse_cylinder(t_scene *scene, char *line);
int			parse_cone(t_scene *scene, char *line);
int			parse_triangle(t_scene *scene, char *line);
int			parse_cube(t_scene *scene, char *line);
int			parse_mesh(t_scene *scene, char *line);

// Light parsing helper functions
t_light		*parse_light_properties(t_scene *scene, char **parts);
t_light		*create_light(t_vec3 position, double intensity, t_color color);
void		add_light(t_scene *scene, t_light *light);

// Bonus parsing functions
int			parse_light_bonus(t_scene *scene, char *line);
int			parse_line_bonus(t_scene *scene, char *line);
int			parse_scene_file_bonus(char *filename, t_scene *scene);

// Mesh parsing functions (parser_mesh.c)
void		parse_vertex_line(char *line, t_vec3 *vertex);
void		count_obj_elements(int fd, int *vertex_count, int *face_count);
t_mesh		*load_obj_file(const char *filename);
t_object	*create_mesh_object(t_mesh *mesh, t_color color);
t_mesh		*create_mesh(t_triangle *triangles, int triangle_count);

// Mesh processing (parser_mesh_process.c)
void		parse_obj_content(int fd, t_vec3 *vertices, \
					t_triangle *triangles, int *triangle_count);
void		calculate_triangle_normal(t_triangle *triangle);
void		parse_face_line(char *line, t_vec3 *vertices, \
					t_triangle *triangles, int *triangle_index);
void		process_vertex_line(char *line, t_vec3 *vertices, int *v_idx);
void		process_normal_line(char *line, t_vec3 *normals, int *n_idx);
void		process_face_line(char *line, t_process_data *process_data);

// Face processing functions (parser_mesh_face_processing.c)
void		parse_face_quad(t_triangle *triangles, t_vec3 *vertices, \
					t_face_data *face_data);
void		set_triangle_normals(t_triangle *triangle, t_vec3 *normals, \
					int *normal_indices);
void		parse_face_triangle(t_triangle *triangles, t_vec3 *vertices, \
					t_face_data *face_data);
void		parse_vertex_normal_indices(char *values[], \
				int *normal_indices, int i);
void		parse_face_indices(char **values, int *indices, \
				int *normal_indices);

// Triangle helper functions (parser_mesh_triangle_helpers.c)
void		set_triangle_vertices_quad_1(t_triangle *triangle, \
				t_vec3 *vertices, int *indices);
void		set_triangle_normals_quad_1(t_triangle *triangle, \
					t_vec3 *normals, int *normal_indices);
void		set_triangle_vertices_quad_2(t_triangle *triangle, \
					t_vec3 *vertices, int *indices);
void		set_triangle_normals_quad_2(t_triangle *triangle, t_vec3 *normals, \
					int *normal_indices);
void		set_triangle_vertices(t_triangle *triangle, t_vec3 *vertices, \
					int *indices);

// Mesh loader helper functions (parser_mesh_loader_helpers.c)
void		count_elements(int fd, int *vertex_count, int *face_count, \
					int *normal_count);
int			allocate_memory(t_vec3 **vertices, t_triangle **triangles, \
					t_vec3 **normals, t_counts *counts);
void		parse_file_content(int fd, t_mesh_data *mesh_data);
void		cleanup_memory(t_vec3 *vertices, t_triangle *triangles, \
					t_vec3 *normals);
int			open_file_for_parsing(const char *filename, t_vec3 *vertices, \
					t_triangle *triangles, t_vec3 *normals);

// File handling functions (parser_file.c)
int			is_valid_filename(char *filename);
int			open_scene_file(t_scene *scene, char *filename);
int			read_scene_file(int fd, t_scene *scene);
int			parse_scene_file(char *filename, t_scene *scene);

// Line processing functions (parser_line.c)
int			parse_line(t_scene *scene, char *line);
int			parse_parameters(t_scene *scene, char *line);

// Material parsing (parser_material.c)
char		*extract_material_block(char *line);
void		trim_material_block(char *line);
double		get_property_value(const char *material_block, \
				const char *property);
void		parse_material_properties(t_scene *scene, char *material_block, \
					t_material *material);
double		ft_clamp(double value, double min, double max);
void		parse_checker_color(t_scene *scene, t_color *color, \
				const char *color_pos, int length);

// Material helper functions (parser_material_helpers.c)
double		get_property_value(const char *material_block,
				const char *property);
void		*get_property_filename(t_scene *scene, const char *material_block, \
					const char *property, char **filename);
void		get_texture_bump_map(t_scene *scene, char *material_block, \
					t_material *material);

// Texture handling (parser_texture.c)
t_texture	*create_texture(t_scene *scene, const char *filename);
void		free_texture_mlx(t_texture *texture);

// Bump map handling (parser_bumpmap.c)
t_bump_map	*create_bump_map(t_scene *scene, const char *filename);
void		load_bump_map_file(t_scene *scene, t_bump_map **bump_map, \
					const char *filename);
void		allocate_elevation(t_scene *scene, t_bump_map **bump_map, int fd);
void		free_bump_map(t_bump_map *bump_map);

// Bump map PPM header parsing (parser_bumpmap_ppm_header.c)
void		parse_ppm_header(t_scene *scene, t_bump_map **bump_map, int fd);
void		read_width_and_height(t_scene *scene, char *line, \
					t_bump_map **bump_map, int fd);
void		validate_max_color_value(t_scene *scene, char *line, \
									t_bump_map **bump_map, int fd);
int			allocate_elevation_memory(t_bump_map *bump_map, \
										int fd, int total_pixels);

// Bump map file reading (parser_bumpmap_file_reader.c)
char		*read_remaining_file(int fd, size_t *size);

// Bump map RGB processing (parser_bumpmap_rgb_process.c)
float		calculate_rgb_normalized_value(char **current_pos);
int			process_rgb_data(t_bump_map **bump_map, char *data_buffer, \
						size_t buffer_size, int total_pixels);

#endif
