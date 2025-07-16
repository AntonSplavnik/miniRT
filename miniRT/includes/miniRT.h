/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 10:35:54 by abillote          #+#    #+#             */
/*   Updated: 2025/06/20 12:06:43 by abillote         ###   ########.fr       */
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

// Color conversion functions (color_conversion.c)
int			valid_color_range(int c);
t_color		create_color(int r, int g, int b);
t_color_f	create_color_f(double r, double g, double b);
int			color_to_int(t_color color);
t_color		int_to_color(int color);

// Tone mapping functions (tone_mapping.c)
double		srgb_to_linear(double srgb);
double		linear_to_srgb(double linear);
t_color_f	color_to_linear(t_color color);
t_color_f	clamp_color_components(t_color_f color);
t_color_f	apply_saturation_boost(t_color_f color);

// HDR processing functions (hdr_processing.c)
t_color_f	apply_tone_mapping(t_color_f color);
t_color_f	apply_reinhard_tone_mapping(t_color_f color);
t_color_f	apply_gamma_correction(t_color_f color);
int			convert_color_f_to_int(t_color_f color);
int			color_f_to_display(t_color_f color);

// Lighting functions (lighting.c)
t_vec3		color_f_to_vec3(t_color_f color);
t_color_f	vec3_to_color_f(t_vec3 v);
int			add_colors(int color1, int color2);
int			blend_colors(int base_color, int blend_color,
				double base_weight, double blend_weight);

// Color calculations (color_calculations.c)
t_color_f	calculate_diffuse_lighting(t_color_f base_color,
				double diffuse_component, t_color_f light_color);
t_color_f	calculate_specular_lighting(double specular_intensity,
				t_color_f light_color);
t_color_f	add_color_components(t_color_f color1, t_color_f color2);
t_color_f	get_pixel_color_linear(t_hit_record *hit_record,
				double light_intensity, t_color_f light_color,
				double specular_intensity);

// Texture sampling functions (texture_sampling.c)
void		wrap_uv_coordinates(double *u, double *v);
void		clamp_texture_coords(int *x, int *y, t_texture *texture);
t_color		sample_texture(t_texture *texture, double u, double v);
t_color_f	sample_texture_linear(t_texture *texture, double u, double v);

// Surface color functions (surface_colors.c)
t_color		get_surface_color_with_texture(t_hit_record *hit_record);
t_color_f	get_surface_color_linear(t_hit_record *hit_record);

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
t_vec3		trace_ray(t_scene *scene, t_ray ray, int depth);
t_vec3		reinhard_tone_map(t_vec3 color, double exposure);
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

// Global exposure value for tone mapping
extern float g_exposure;

// Ray tracing helper functions
t_vec3		trace_ray(t_scene *scene, t_ray ray, int depth);
t_vec3		get_background_color(t_scene *scene);
t_vec3		handle_refractive_material(t_ray_context context, double cos_theta);
t_vec3		handle_reflective_material(t_ray_context context, double cos_theta, 
		double *total_contrib);
t_vec3		add_direct_lighting(t_scene *scene, t_hit_record hit_record, 
			double total_contribution, t_vec3 final_color);
t_vec3		process_lights(t_scene *scene, t_hit_record hit_record, 
			t_light *current_light, t_vec3 direct_color);
t_vec3		process_material_interaction(t_ray_context context, double cos_theta);

// Fresnel calculations
double		calculate_fresnel_reflectance(t_fresnel_params params);
double		calculate_fresnel_schlick(double cos_theta, double ior);

// Reflection operations
t_vec3		calculate_reflection(t_scene *scene, t_ray ray,
		t_hit_record hit_record, int depth);
t_vec3		add_reflection_contribution(t_ray_context context,
		t_material_contrib *contrib);

// Refraction operations
t_vec3		handle_total_internal_reflection(t_ray_context context,
		t_material_contrib *contrib);
t_vec3		process_refraction(t_ray_context context,
		t_material_contrib *contrib, t_vec3 refract_dir);
t_vec3		add_refraction_contribution(t_ray_context context,
		t_material_contrib *contrib, t_refraction_params refract_params);

// Material processing
t_vec3		process_reflection_refraction(t_ray_context context,
		t_material_contrib *contrib, t_fresnel_params fresnel_params);

// Lighting calculations
t_light_result	compute_light(t_scene *scene, t_hit_record hit_record, t_light *light);

// Ray generation
void		compute_ray_direction(t_scene *scene, t_ray *ray, double fov_scale, 
		double x, double y);

// Basic reflection/refraction mathematics
t_vec3		reflect_ray(t_vec3 incident, t_vec3 normal);
t_vec3		refract_ray(t_vec3 incident, t_vec3 normal, double eta_ratio, 
		bool *total_internal_reflection);

#endif
