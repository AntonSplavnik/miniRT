/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 16:09:12 by abillote          #+#    #+#             */
/*   Updated: 2025/06/05 12:35:15 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
# define PARSER_H

#include "types.h"

# define MAX_LINE_LEN 8192

// Utility functions
size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize);
void	free_split(char **split);
void	parse_error(t_scene *scene, char *message);
int		check_range(double value, double min, double max);
int		count_parts(char **parts);
void	check_parts_count(t_scene *scene, char **parts, int expected, char *element);

// Identifier functions
int		is_ambient(char *line);
int		is_camera(char *line);
int		is_light(char *line);
int		is_sphere(char *line);
int		is_plane(char *line);
int		is_cylinder(char *line);
int		is_cone(char *line);
int		is_triangle(char *line);
int		is_cube(char *line);
int		is_mesh(char *line);

// Vector and color parsing
int		read_vector(char *str, t_vec3 *vector);
int		read_color(char *str, t_color *color);
int		check_vector_normalization(t_vec3 vector);
int		parse_rgb(char **parts, int *r, int *g, int *b);

// Object parsing functions
int		parse_ambient(t_scene *scene, char *line);
int		parse_camera(t_scene *scene, char *line);
int		parse_light(t_scene *scene, char *line);
int		parse_sphere(t_scene *scene, char *line);
int		parse_plane(t_scene *scene, char *line);
int		parse_cylinder(t_scene *scene, char *line);
int		parse_cone(t_scene *scene, char *line);
int		parse_triangle(t_scene *scene, char *line);
int		parse_cube(t_scene *scene, char *line);
int		parse_mesh(t_scene *scene, char *line);

// File handling functions
int		is_valid_filename(char *filename);
int		open_scene_file(t_scene *scene, char *filename);
int		read_scene_file(int fd, t_scene *scene);
int		parse_scene_file(char *filename, t_scene *scene);

// Line processing functions
int		parse_line(t_scene *scene, char *line);
int		parse_parameters(t_scene *scene, char *line);

//material
/* Function declarations for material parser */
char    *extract_material_block(char *line);
void    trim_material_block(char *line);
double  get_property_value(const char *material_block, const char *property);
void    parse_material_properties(t_scene *scene, const char *material_block, t_material *material);
double  ft_clamp(double value, double min, double max);

//texture
/* Function declarations for texture parser */
t_texture *create_texture(t_scene *scene, const char *filename);
void    free_texture_mlx(t_texture *texture);

//bump map
/* Function declarations for bump map parser */
t_bump_map *create_bump_map(t_scene *scene, const char *filename);
void    free_bump_map(t_bump_map *bump_map);

/* Function declarations for string utilities */
char    *ft_strstr(const char *haystack, const char *needle);
size_t  ft_strlcat(char *dst, const char *src, size_t size);

#endif
