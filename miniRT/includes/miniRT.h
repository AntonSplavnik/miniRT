/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 10:35:54 by abillote          #+#    #+#             */
/*   Updated: 2025/06/10 13:01:10 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

#include "types.h"
#include "parser.h"
#include "../libft/libft.h"


// events
int			close_handler(t_scene *scene);
int			key_handler(int keysym, t_scene *scene);
int 		control_mouse_handler(int button, int x, int y, t_scene *scene);
int			mouse_handler(int button, int x, int y, t_scene *scene);
int			mouse_release(int button, int x, int y, t_scene *scene);
void		display_status(t_scene *scene);
void		display_progress(t_scene *scene, const char *status_text);

// init
void		init_ui(t_scene *scene);
void		setup_hooks(t_scene *scene);
void		init_data(t_scene *scene);
void		init_mlx(t_scene *scene);
void		cleanup_scene(t_scene *scene);

// render
void		pixel_put(int x, int y, mlx_image_t *img, int color);
void		draw_image_to_window(t_scene *scene);

// string utils
int			ft_strncmp(const char *s1, const char *s2, size_t n);
void		write_string_to_file_descriptor(char *str, int file_descriptor);
int			ft_strcmp(char *s1, char *s2);
char		*ft_strtrim_whitespace(char *s);
char		**ft_split_line(char *s, char c);
int			is_separator(char c, char delimiter);
int			ft_isspace(char c);
double		ft_atof(const char *str);
char		*ft_strchr(const char *s, int c);
char		*ft_strstr(const char *haystack, const char *needle);

// menger sponge
// void		render_menger_sponge(t_scene *scene);

// BVH functions
t_bvh_node	*build_menger_bvh(int max_iterations);
void		free_bvh(t_bvh_node *node);
int			ray_intersect_bvh(t_bvh_node *node, t_vec3 ray_origin,
							t_vec3 ray_dir, double *t_min, double *t_max);
int			ray_intersect_aabb_scalar(t_aabb bounds, t_vec3 ray_origin,
							t_vec3 ray_dir, double *t_min, double *t_max);
int			ray_intersect_aabb_simd(t_aabb bounds, t_vec3 origin,
									t_vec3 dir, double *out_tmin, double *out_tmax);

// Vector utilities
t_vec3		vec3_create(double x, double y, double z);
t_vec3		vec3_negate(t_vec3 v);
t_vec3		vec3_add(t_vec3 u, t_vec3 v);
t_vec3		vec3_subtract(t_vec3 u, t_vec3 v);
t_vec3		vec3_multiply(t_vec3 u, t_vec3 v);
t_vec3		vec3_scale(t_vec3 v, double t);
t_vec3		vec3_divide(t_vec3 v, double t);
double		vec3_dot(t_vec3 u, t_vec3 v);
t_vec3		vec3_cross(t_vec3 u, t_vec3 v);
double		vec3_length_squared(t_vec3 v);
double		vec3_length(t_vec3 v);
t_vec3		vec3_normalize(t_vec3 v);

// colors
t_color		create_color(int r, int g, int b);
int			get_pixel_color(t_hit_record *hit_record, double light_intensity,
                               t_color light_color,
                               double specular_intensity);
int			valid_color_range(int c);
t_color 	get_surface_color_with_texture(t_hit_record *hit_record);
t_color		int_to_color(int color);
int			color_to_int(t_color color);
int			blend_colors(int base_color, int blend_color, double base_weight, double blend_weight);
int			add_colors(int color1, int color2);

// material
t_material	create_material(t_color color);

// object creation
void		add_object(t_scene *scene, t_object *object);

// object intersection
int			ray_sphere_intersect(t_ray ray, t_sphere sphere, double *t);
int			find_closest_intersection(t_scene *scene, t_ray ray, double *t, t_object **hit_object, t_hit_record *hit_record);
t_vec3		sphere_normal_at_point(t_vec3 point, t_sphere sphere);
t_vec3		cylinder_normal_at_point(t_vec3 point, t_cylinder cylinder);
int			ray_cylinder_intersect(t_ray ray, t_cylinder cylinder, double *t);
int			ray_plane_intersect(t_ray ray, t_plane plane, double *t);
int			ray_cube_intersect(t_ray ray, t_cube cube, double *t);
t_vec3		cube_normal_at_point(t_vec3 point, t_cube cube);
int			ray_triangle_intersect(t_ray ray, t_triangle triangle, double *t);
t_vec3		triangle_normal(t_triangle triangle);
int			ray_mesh_intersect(t_ray ray, t_mesh mesh, double *t, int *triangle_idx);
int			ray_disc_intersect(t_ray ray, t_vec3 center, t_vec3 normal,
								double radius, double *t);
int			ray_cone_intersect(t_ray ray, t_cone cone, double *t);
t_vec3		cone_normal_at_point(t_vec3 point, t_cone cone);
int			is_point_in_disc(t_vec3 point, t_vec3 center, t_vec3 normal, double radius);

// lights
void		add_light(t_scene *scene, t_light *light);
t_light		*create_light(t_vec3 position, double intensity, t_color color);

// rendering
void		compute_ray_direction(t_scene *scene, t_ray *ray, double fov_scale, double x, double y);
void		render_scene(t_scene *scene);
// void	render_scene(void *param);
int			trace_ray(t_scene *scene, t_ray ray, int depth);
void		*render_thread(void *arg);
t_vec3		reflect_ray(t_vec3 incident, t_vec3 normal);
t_vec3		refract_ray(t_vec3 incident, t_vec3 normal, double eta_ratio, bool *total_internal_reflection);
t_vec2 		calculate_uv_coordinates(t_vec3 point, t_object *object);
t_vec3 		calculate_bump_normal(t_hit_record *hit_record);


// compute light
t_light_result	compute_light(t_scene *scene, t_hit_record hit_record, t_light *light);

void	compute_ray_intersection(t_ray ray, t_object *hit_object, double t, t_hit_record *hit_record);


//shadows
int			is_in_shadow(t_scene *scene, t_vec3 hit_point, t_vec3 light_dir, double light_distance, t_hit_record hit_record);

// scenes
void	set_up_scene_triangle(t_scene *scene);
void	set_up_scene_plane(t_scene *scene);
void	set_up_scene_two_sphere(t_scene *scene);
void	set_up_scene_cylinder(t_scene *scene);
void	set_up_scene_mesh(t_scene *scene);
void	set_up_scene_cube(t_scene *scene);


// UI Panel functions
void	init_toggle_button(t_scene *scne);
void	init_ui_panel(t_scene *scene);
bool	draw_ui_panel(t_scene *scene);
void	cleanup_ui_panel(t_scene *scene);
bool	ui_panel_mouse_click(t_scene *scene, int x, int y);
void	ui_animation_loop(void *param);
void	draw_ui(t_scene *scene);

// camera
t_vec3		rotate_point(t_vec3 point, t_vec3 rotation);
t_vec3		get_forward_vector(t_vec3 rotation);
t_vec3		get_right_vector(t_vec3 rotation);
t_vec3		get_up_vector(t_vec3 rotation);

// mouse controls
void		mouse_button_callback(mouse_key_t button, action_t action, modifier_key_t mods, void* param);
void		cursor_position_callback(double xpos, double ypos, void* param);
void		setup_mouse_hook(t_scene *scene);

// key controls
void		setup_key_hooks(t_scene	*scene);

// close handler
void		setup_close_hook(t_scene *scene);

// cleanup
void		cleanup_scene(t_scene *scene);
void		close_callback(void *param);


// Checkerboard
int				is_checker_point(t_vec3 point, double checker_size);
int				is_checker_point_plane(t_plane plane, t_vec3 point, double checker_size);
t_color			get_checker_color(t_material material, t_object *object, t_vec3 point);
t_vec2			spherical_map(t_vec3 point);
t_vec2			cylindrical_map(t_vec3 point, double radius, t_vec3 axis);
int				is_on_cylinder_cap(t_vec3 point, t_cylinder *cylinder);
t_vec2			circular_map(t_vec3 point, t_cylinder *cylinder);
t_vec2			cubic_map(t_cube *cube, t_vec3 point);
t_vec2			planar_map(t_plane *plane, t_vec3 point);
int				is_checker_point_2d(double u, double v, double checker_size);
#endif
