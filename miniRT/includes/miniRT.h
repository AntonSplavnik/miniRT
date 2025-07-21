/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 10:35:54 by abillote          #+#    #+#             */
/*   Updated: 2025/07/21 11:41:03 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# include "types.h"
# include "parser.h"
# include "libft.h"
# include "bvh.h"
# include "ft_printf.h"

// events
void			display_status(t_scene *scene);
void			format_float_string(double value, char *str);
int				int_to_string(int num, char *str);
void			append_coordinate_values(char *pos, \
				char *x_str, char *y_str, char *z_str);
mlx_image_t		*create_status_image(t_scene *scene, char *status);
void			apply_text_color(mlx_image_t *img);

// init
void			init_ui(t_scene *scene);
void			setup_hooks(t_scene *scene);
void			init_data(t_scene *scene);
void			init_mlx(t_scene *scene);
void			cleanup_scene(t_scene *scene);
void			init_scene_bvh(t_scene *scene);
void			init_app_and_window(t_scene *scene);
void			init_camera_settings(t_scene *scene);
void			init_graphics_and_mouse(t_scene *scene);

// cleanup functions
void			free_mesh(t_mesh *mesh);
void			cleanup_object(t_object *obj);
void			cleanup_objects(t_scene *scene);
void			cleanup_lights(t_scene *scene);

// main functions
void			init_and_parse_scene(t_scene *scene, char *filename);
void			init_graphics_and_ui(t_scene *scene);
void			start_raytracer(t_scene *scene, char *filename);
void			init_and_parse_scene_bonus(t_scene *scene, char *filename);
void			init_graphics_and_ui_bonus(t_scene *scene);
void			start_raytracer_bonus(t_scene *scene, char *filename);
void			print_usage_and_exit(void);

// render
void			pixel_put(int x, int y, mlx_image_t *img, int color);
void			draw_image_to_window(t_scene *scene);

// string utils
int				ft_strncmp(const char *s1, const char *s2, size_t n);
int				ft_strcmp(char *s1, char *s2);
char			*ft_strtrim_whitespace(char *s);
char			**ft_split_line(char *s, char c);
int				is_separator(char c, char delimiter);
int				ft_isspace(char c);
double			ft_atof(const char *str);
char			*ft_strchr(const char *s, int c);
char			*ft_strstr(const char *haystack, const char *needle);

// BVH functions
void			init_scene_bvh(t_scene *scene);
t_bvh_node		*build_menger_bvh(int max_iterations);
void			free_bvh(t_bvh_node *node);
int				ray_intersect_aabb_scalar(t_ray_aabb_params params);

// Scene BVH functions
t_aabb			calculate_object_aabb(t_object *object);
t_bvh_node		*build_scene_bvh(t_scene *scene);
t_bvh_node		*build_scene_bvh_recursive(t_bvh_params params);
int				scene_ray_intersect_bvh(t_scene_ray_params params);
t_bvh_node		*build_scene_bvh(t_scene *scene);
t_aabb			calculate_object_aabb(t_object *object);

// Vector utilities
t_vec3			vec3_create(double x, double y, double z);
t_vec3			vec3_negate(t_vec3 v);
t_vec3			vec3_add(t_vec3 u, t_vec3 v);
t_vec3			vec3_subtract(t_vec3 u, t_vec3 v);
t_vec3			vec3_multiply(t_vec3 u, t_vec3 v);
t_vec3			vec3_scale(t_vec3 v, double t);
t_vec3			vec3_divide(t_vec3 v, double t);
double			vec3_dot(t_vec3 u, t_vec3 v);
t_vec3			vec3_cross(t_vec3 u, t_vec3 v);
double			vec3_length_squared(t_vec3 v);
double			vec3_length(t_vec3 v);
t_vec3			vec3_normalize(t_vec3 v);
t_vec3			compute_barycentric(t_vec3 p, t_vec3 a, \
				t_vec3 b, t_vec3 c);
t_vec3			interpolate_vertex_normal(t_vec3 bary, \
				t_vec3 n0, t_vec3 n1, t_vec3 n2);

// Color conversion functions (color_conversion.c)
int				valid_color_range(int c);
t_color			create_color(int r, int g, int b);
t_color_f		create_color_f(double r, \
				double g, double b);
int				color_to_int(t_color color);
t_color			int_to_color(int color);

// Tone mapping functions (tone_mapping.c)
double			srgb_to_linear(double srgb);
double			linear_to_srgb(double linear);
t_color_f		color_to_linear(t_color color);
t_color_f		clamp_color_components(t_color_f color);
t_color_f		apply_saturation_boost(t_color_f color);

// HDR processing functions (hdr_processing.c)
t_color_f		apply_tone_mapping(t_color_f color);
t_color_f		apply_reinhard_tone_mapping(t_color_f color);
t_color_f		apply_gamma_correction(t_color_f color);
int				convert_color_f_to_int(t_color_f color);
int				color_f_to_display(t_color_f color);

// Lighting functions (lighting.c)
t_vec3			color_f_to_vec3(t_color_f color);
t_color_f		vec3_to_color_f(t_vec3 v);
int				add_colors(int color1, int color2);
int				blend_colors(int base_color, int blend_color, \
				double base_weight, double blend_weight);

// Color calculations (color_calculations.c)
t_color_f		calculate_diffuse_lighting(t_color_f base_color, \
				double diffuse_component, t_color_f light_color);
t_color_f		calculate_specular_lighting(double specular_intensity, \
				t_color_f light_color);
t_color_f		add_color_components(t_color_f color1, t_color_f color2);
t_color_f		get_pixel_color_linear(t_hit_record *hit_record, \
				double light_intensity, t_color_f light_color, \
				double specular_intensity);

// Texture sampling functions (texture_sampling.c)
// void		wrap_uv_coordinates(double *u, double *v);
void			clamp_texture_coords(int *x, int *y, t_texture *texture);
t_color			sample_texture(t_texture *texture, double u, double v);
t_color_f		sample_texture_linear(t_texture *texture, double u, double v);

// Surface color functions (surface_colors.c)
t_color			get_surface_color_with_texture(t_hit_record *hit_record);
t_color_f		get_surface_color_linear(t_hit_record *hit_record);

// material
t_material		create_material(t_color color);

// object creation
void			add_object(t_scene *scene, t_object *object);

// object intersection
int				ray_sphere_intersect(t_ray ray, t_sphere sphere, double *t);
int				find_closest_intersection(t_scene *scene, \
				t_ray ray, t_hit_record *hit_record, t_object **hit_object);
int				test_basic_shapes(t_object *current, \
				t_ray ray, double *t_temp);
int				test_cube_cone(t_object *current, t_ray ray, double *t_temp);
int				test_triangle_mesh_bary(t_object *current, \
				t_ray ray, double *t_temp, t_hit_record *hit_record);
void			update_closest_hit(double t_temp, t_intersection_data *data, \
				t_object *current);
void			interpolate_triangle_normal(t_hit_record *hit_record, \
				t_triangle *triangle);
void			interpolate_mesh_normal(t_hit_record *hit_record, \
				t_transformed_triangle *tri);
void			compute_inside_outside(t_hit_record *hit_record, t_ray ray);
void			compute_sphere_normal(t_hit_record *hit_record);
void			compute_cylinder_normal(t_hit_record *hit_record);
void			compute_plane_normal(t_hit_record *hit_record, t_ray ray);
void			compute_cube_normal(t_hit_record *hit_record);
void			compute_cone_normal(t_hit_record *hit_record);
void			compute_triangle_normal(t_hit_record *hit_record, t_ray ray);
void			compute_mesh_normal(t_hit_record *hit_record, t_ray ray);
void			compute_object_normal(t_hit_record *hit_record, t_ray ray);
void			compute_complex_normals(t_hit_record *hit_record, t_ray ray);
t_vec3			sphere_normal_at_point(t_vec3 point, t_sphere sphere);
t_vec3			cylinder_normal_at_point(t_vec3 point, t_cylinder cylinder);
int				ray_cylinder_intersect(t_ray ray, t_cylinder cylinder, \
					double *t);
void			check_cylinder_caps_intersection(t_ray ray, \
				t_cylinder cylinder, double *t_top, double *t_bottom);
double			ray_disc_intersect(t_ray ray, t_vec3 center, \
				t_vec3 normal, double radius);
int				ray_plane_intersect(t_ray ray, t_plane plane, double *t);
int				ray_cube_intersect(t_ray ray, t_cube cube, double *t);
double			min(double a, double b);
double			max(double a, double b);
t_slab			calculate_slab_data(t_ray ray, t_vec3 min_bound, \
				t_vec3 max_bound);
t_vec3			cube_normal_at_point(t_vec3 point, t_cube cube);
int				ray_triangle_intersect(t_ray ray, t_triangle triangle, \
					double *t);
int				ray_triangle_intersect_bary(t_ray ray, t_triangle triangle, \
				double *t, t_vec3 *bary);
t_vec3			triangle_normal(t_triangle triangle);
void			precompute_transformed_triangles(t_mesh *mesh);
int				ray_mesh_intersect(t_ray ray, t_mesh mesh, \
				double *t, int *triangle_idx);
int				ray_mesh_intersect_bary(t_mesh_intersect_params params);
int				mesh_linear_intersect(t_ray ray, t_mesh mesh, \
				double *t, int *triangle_idx);
int				mesh_linear_intersect_bary(t_ray ray, t_mesh mesh, \
				t_mesh_bary_params params);
int				mesh_bvh_intersect_bary(t_ray ray, t_mesh *mesh, \
				t_bvh_bary_params *bary_params);
int				ray_cone_intersect(t_ray ray, t_cone cone, double *t);
void			calculate_cone_equation_coeffs(t_ray ray, \
				t_cone cone, t_vec3 apex_to_origin, double *coeffs);
int				solve_cone_quadratic(t_ray ray, t_cone cone, \
				double *coeffs, double *t_body);
t_vec3			cone_normal_at_point(t_vec3 point, t_cone cone);

// lights
void			add_light(t_scene *scene, t_light *light);
t_light			*create_light(t_vec3 position, double intensity, t_color color);

// rendering
void			compute_ray_direction(t_scene *scene, t_ray *ray, \
				double fov_scale, t_vec2 coords);
t_vec3			trace_ray(t_scene *scene, t_ray ray, int depth);
t_vec3			reinhard_tone_map(t_vec3 color, double exposure);
t_vec3			reflect_ray(t_vec3 incident, t_vec3 normal);
t_vec3			refract_ray(t_vec3 incident, t_vec3 normal, \
				double eta_ratio, bool *total_internal_reflection);

/* bumpmap.c */
float			sample_bump_map(t_bump_map *bump_map, double u, double v);

/* tangent_vectors.c */
void			calculate_tangent_vectors(t_vec3 normal, t_vec3 *tangent, \
				t_vec3 *bitangent);
void			calculate_sphere_tangent_vectors(t_hit_record *hit_record, \
				t_vec3 *tangent, t_vec3 *bitangent);
void			calculate_plane_tangent_vectors(t_hit_record *hit_record, \
				t_vec3 *tangent, t_vec3 *bitangent);

/* cylinder_cube_tangents.c */
void			calculate_cylinder_tangent_vectors(t_hit_record *hit_record, \
				t_vec3 *tangent, t_vec3 *bitangent);
void			calculate_cube_tangent_vectors(t_hit_record *hit_record, \
				t_vec3 *tangent, t_vec3 *bitangent);

/* bump_normal.c */
t_vec3			calculate_bump_normal(t_hit_record *hit_record);

// uv_mapping
t_vec2			sphere_uv_mapping(t_vec3 point, t_sphere sphere);
t_vec2			plane_uv_mapping(t_vec3 point, t_plane plane);
t_vec2			cylinder_uv_mapping(t_vec3 point, t_cylinder cylinder);
t_vec2			cube_uv_mapping(t_vec3 point, t_cube cube);
t_vec2			calculate_uv_coordinates(t_vec3 point, t_object *object);

// compute light
t_light_result	compute_light(t_scene *scene, t_hit_record hit_record, \
				t_light *light);
void			compute_ray_intersection(t_ray ray, t_object *hit_object, \
				t_hit_record *hit_record);

//shadows
double			calculate_shadow_attenuation(t_scene *scene, \
				t_shadow_params params);
int				process_transparent_hit(t_shadow_data *data, \
				t_object *hit_object, double t);

// trace ray helpers
double			get_light_shadow_attenuation(t_scene *scene, \
				t_hit_record hit_record, t_light *light);
t_vec3			apply_light_contribution(t_light_params params);

// scenes
void			set_up_scene_triangle(t_scene *scene);
void			set_up_scene_plane(t_scene *scene);
void			set_up_scene_two_sphere(t_scene *scene);
void			set_up_scene_cylinder(t_scene *scene);
void			set_up_scene_mesh(t_scene *scene);
void			set_up_scene_cube(t_scene *scene);

// UI Control Panel Functions
bool			*get_checkbox_state(t_scene *scene, int idx);
void			clear_panel_image(t_panel *panel);
void			init_toggle_button(t_scene *scene);
void			init_ui_panel(t_scene *scene);
void			init_panel_image(t_scene *scene, t_panel *p);
void			cleanup_ui_panel(t_scene *scene);
void			cleanup_panel_images(t_scene *scene, t_panel *p);
void			cleanup_status_text(t_scene *scene, t_panel *p);

// Drawing Functions
void			draw_toggle_button(t_scene *scene);
void			draw_button_background(t_toggle_button *tog);
void			draw_button_border(t_toggle_button *tog);
void			draw_checkbox(t_panel *p, mlx_image_t *img, \
				t_checkbox_pos *pos);
void			draw_checkbox_background(t_panel *p, \
				mlx_image_t *img, int x, int y);
void			draw_checkbox_border(t_panel *p, mlx_image_t *img, \
					int x, int y);
void			draw_checkbox_check(t_panel *p, mlx_image_t *img, int x, int y);
void			draw_single_checkbox(t_scene *scene, int x_cb, int y_base, \
					int idx);
void			draw_panel_background(t_panel *p);
void			draw_panel_main_bg(t_panel *p);
void			draw_panel_header(t_panel *p);
void			draw_panel_borders(t_panel *p);
void			draw_panel_content(t_scene *scene);
bool			update_panel_animation(t_panel *p);
bool			update_panel_closing(t_panel *p);

// Text Management
void			delete_panel_text(t_scene *scene);
void			hide_panel_text(t_panel *p);
void			show_panel_text(t_panel *p);
void			draw_panel_text(t_scene *scene);
void			draw_panel_title(t_scene *scene, t_panel *p);
void			draw_checkbox_labels(t_scene *scene, t_panel *p);
char			*get_checkbox_label(int idx);
bool			draw_ui_panel(t_scene *scene);
void			handle_panel_text_display(t_scene *scene, t_panel *p);

// Interaction
bool			is_point_in_toggle_button(t_toggle_button *tog, int x, int y);
bool			is_point_in_checkbox(t_panel *p, int x, int y, int idx);
bool			ui_panel_mouse_click(t_scene *scene, int x, int y);
bool			handle_checkbox_clicks(t_scene *scene, \
					t_panel *p, int x, int y);
void			ui_animation_loop(void *param);
void			draw_ui(t_scene *scene);

// camera
t_vec3			rotate_point(t_vec3 point, t_vec3 rotation);
t_vec3			get_forward_vector(t_vec3 rotation);
t_vec3			get_right_vector(t_vec3 rotation);
t_vec3			get_up_vector(t_vec3 rotation);
void			update_camera_vectors(t_scene *scene);

// mouse controls
void			mouse_button_callback(mouse_key_t button, \
				action_t action, modifier_key_t mods, void *param);
void			cursor_position_callback(double xpos, double ypos, void *param);
void			handle_left_press(t_scene *scene);
void			handle_right_press(t_scene *scene);
void			handle_panel_drag(t_scene *scene, double xpos, double ypos);
void			handle_camera_rotation(t_scene *scene);
int				clamp(int val, int min, int max);
void			setup_mouse_hook(t_scene *scene);

// key controls
void			handle_movement_keys(mlx_key_data_t keydata, t_scene *scene);
void			handle_strafe_keys(mlx_key_data_t keydata, t_scene *scene);
void			handle_vertical_keys(mlx_key_data_t keydata, t_scene *scene);
void			handle_sampling_keys(mlx_key_data_t keydata, t_scene *scene);
void			setup_key_hooks(t_scene	*scene);

// close handler
void			setup_close_hook(t_scene *scene);

// cleanup
void			cleanup_scene(t_scene *scene);
void			close_callback(void *param);

/* checker.c function declarations */
t_vec2			spherical_map(t_vec3 point);
int				is_checker_point_2d(double u, double v, double checker_size);
t_vec2			get_plane_checker_uv(t_object *object, t_vec3 point);
t_vec2			get_sphere_checker_uv(t_object *object, t_vec3 point);
t_color			get_checker_color(t_material material, t_object *object, \
				t_vec3 point);

/* checker_utils.c function declarations */
t_vec2			project_to_plane_uv(t_plane *plane, t_vec3 point, \
				t_vec3 u_axis);
t_vec2			get_cylinder_checker_uv(t_object *object, t_vec3 point);
t_vec2			get_cube_checker_uv(t_object *object, t_vec3 point);
t_vec2			cylindrical_map(t_vec3 point, double radius, t_vec3 axis);
t_vec2			calculate_cylinder_checker_uv(t_vec3 point, double radius, \
				t_vec3 axis);

/* cubic_map.c function declarations */
t_vec2			cubic_map(t_cube *cube, t_vec3 local_point);
t_vec2			map_x_face(t_cube *cube, t_vec3 local_point);
t_vec2			map_y_face(t_cube *cube, t_vec3 local_point);
t_vec2			map_z_face(t_cube *cube, t_vec3 local_point);

// Ray tracing helper functions
t_vec3			trace_ray(t_scene *scene, t_ray ray, int depth);
t_vec3			get_background_color(t_scene *scene);
t_vec3			handle_refractive_material(t_ray_context context, \
				double cos_theta);
t_vec3			handle_reflective_material(t_ray_context context, \
				double cos_theta, double *total_contrib);
t_vec3			add_direct_lighting(t_scene *scene, t_hit_record hit_record, \
				double total_contribution, t_vec3 final_color);
t_vec3			process_lights(t_scene *scene, t_hit_record hit_record, \
				t_light *current_light, t_vec3 direct_color);
t_vec3			process_material_interaction(t_ray_context context, \
				double cos_theta);

// Fresnel calculations
double			calculate_fresnel_reflectance(t_fresnel_params params);
double			calculate_fresnel_schlick(double cos_theta, double ior);

// Reflection operations
t_vec3			calculate_reflection(t_scene *scene, t_ray ray, \
				t_hit_record hit_record, int depth);
t_vec3			add_reflection_contribution(t_ray_context context, \
				t_material_contrib *contrib);

// Refraction operations
t_vec3			handle_total_internal_reflection(t_ray_context context, \
				t_material_contrib *contrib);
t_vec3			process_refraction(t_ray_context context, \
				t_material_contrib *contrib, t_vec3 refract_dir);
t_vec3			add_refraction_contribution(t_ray_context context, \
				t_material_contrib *contrib, \
				t_refraction_params refract_params);

// Material processing
t_vec3			process_reflection_refraction(t_ray_context context, \
				t_material_contrib *contrib, t_fresnel_params fresnel_params);

// Lighting calculations
t_light_result	compute_light(t_scene *scene, t_hit_record hit_record, \
					t_light *light);

// Ray generation
void			compute_ray_direction(t_scene *scene, t_ray *ray, \
				double fov_scale, t_vec2 coords);

// Shared rendering functions
void			render_pixel(t_render_params *params);
void			process_sample_ray(t_render_params *params, int sx, int sy);
void			apply_final_color(t_render_params *params);
void			render_rows(t_render_params *params, t_thread_data *data);
void			setup_render_params(t_render_params *params, \
				t_thread_data *data);

// Main rendering function (single-threaded in regular, multi-threaded in bonus)
void			render_scene(t_scene *scene);
void			create_all_threads(t_scene *scene, int rows_per_thread, \
				pthread_t *threads, t_thread_data *thread_data);
void			oin_all_threads_norm(pthread_t *threads);
void			*render_thread(void *arg);

// Basic reflection/refraction mathematics
t_vec3			reflect_ray(t_vec3 incident, t_vec3 normal);
t_vec3			refract_ray(t_vec3 incident, t_vec3 normal, double eta_ratio, \
				bool *total_internal_reflection);

#endif
