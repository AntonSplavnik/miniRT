/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 10:35:54 by abillote          #+#    #+#             */
/*   Updated: 2025/05/27 11:23:50 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H
# define MINIRT_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <math.h>
# include <pthread.h>
# include <ctype.h>
# include <stdbool.h>
# include "platform_specifics.h"
# include "types.h"
# include "parser.h"

#  define WIDTH 1920
# define HEIGHT 1080

# define NUM_THREADS 8 // Number of threads for multithreaded rendering

// 3D rendering constants
# define FOV 60.0
# define NEAR_PLANE 0.1
# define FAR_PLANE 100.0
# define MAX_RAY_DEPTH 5
# define MAX_BVH_DEPTH 8
# define MAX_BVH_NODES 1000

# define BLACK       0x000000  // RGB(0, 0, 0)
# define WHITE       0xFFFFFF  // RGB(255, 255, 255)
# define RED         0xFF0000  // RGB(255, 0, 0)
# define GREEN       0x00FF00  // RGB(0, 255, 0)
# define BLUE        0x0000FF  // RGB(0, 0, 255)

# define MAGENTA_BURST   0xFF00FF
# define LIME_SHOCK      0xCCFF00
# define NEON_ORANGE     0xFF6600
# define PSYCHEDELIC_PURPLE 0x660066
# define AQUA_DREAM      0x33CCCC
# define HOT_PINK        0xFF66B2
# define ELECTRIC_BLUE   0x0066FF
# define LAVA_RED        0xFF3300

//events
int			close_handler(t_scene *scene);
int			key_handler(int keysym, t_scene *scene);
int 		control_mouse_handler(int button, int x, int y, t_scene *scene);
int			mouse_handler(int button, int x, int y, t_scene *scene);
int			mouse_release(int button, int x, int y, t_scene *scene);
void		display_status(t_scene *scene);
void		display_progress(t_scene *scene, const char *status_text);

//init
void		scene_init(t_scene *scene);
void		cleanup_scene(t_scene *scene);

//render
void		pixel_put(int x, int y, t_img *img, int color);
void		scene_render(t_scene *scene);
void		draw_image_to_window(t_scene *scene);

//string utils
int			ft_strncmp(const char *s1, const char *s2, size_t n);
void		write_string_to_file_descriptor(char *str, int file_descriptor);
int			ft_strcmp(char *s1, char *s2);
int			ft_strlen(char *str);
char		*ft_strtrim(char *s);
char		**ft_split(char *s, char c);
int			ft_atoi(const char *nptr);
int			ft_isspace(char c);
double		ft_atof(const char *str);
void		*ft_calloc(size_t nmemb, size_t size);
void		ft_bzero(void *s, size_t n);
void		*ft_memset(void *s, int c, size_t n);
size_t		ft_strlcpy(char *dst, const char *src, size_t dstsize);

// 3D rendering functions
void		init_3d(t_scene *scene);
void		render_menger_sponge(t_scene *scene);

// BVH functions
t_bvh_node	*build_menger_bvh(int max_iterations);
void		free_bvh(t_bvh_node *node);
int			ray_intersect_bvh(t_bvh_node *node, t_vec3 ray_origin,
							t_vec3 ray_dir, double *t_min, double *t_max);
int			ray_intersect_aabb_scalar(t_aabb bounds, t_vec3 ray_origin,
							t_vec3 ray_dir, double *t_min, double *t_max);
int			ray_intersect_aabb_simd(t_aabb bounds, t_vec3 origin,
									t_vec3 dir, double *out_tmin, double *out_tmax);

//Vector utilities
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

//colors
t_color		create_color(int r, int g, int b);
int			get_pixel_color(t_hit_record hit_record, double light_intensity,
                               t_color light_color,
                               double specular_intensity);
int			valid_color_range(int c);

//material
t_material	create_material(t_color color);

//object creation
void		add_object(t_scene *scene, t_object *object);
t_object	*create_sphere(t_vec3 center, double diameter, t_color color);
t_object	*create_cylinder(t_vec3 center, t_vec3 axis, double diameter, double height);
t_object	*create_plane(t_vec3 point, t_vec3 normal, t_color color);
t_object	*create_cone(t_vec3 tip, t_vec3 axis, double radius, double height);
t_object	*create_cube(t_vec3 center, double side_length, t_color color);
t_object	*create_triangle(t_vec3 v0, t_vec3 v1, t_vec3 v2, t_color color);
t_object	*create_mesh(t_triangle *triangles, int triangle_count, t_color color);
t_object	*create_cube_mesh(t_vec3 center, double size, t_color color);

//object intersection
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

//lights
void		add_light(t_scene *scene, t_light *light);
t_light		*create_light(t_vec3 position, double intensity, t_color color);

//rendering
void		compute_ray_direction(t_scene *scene, t_ray *ray, double fov_scale, int x, int y);
void		render_scene(t_scene *scene);
int			trace_ray(t_scene *scene, t_ray ray, int depth);
void		*render_thread(void *arg);
t_vec3		reflect_ray(t_vec3 incident, t_vec3 normal);

//compute light
t_light_result compute_light(t_scene *scene, t_hit_record hit_record, t_light *light);

void	compute_ray_intersection(t_ray ray, t_object *hit_object, double t, t_hit_record *hit_record);


//shadows
int			is_in_shadow(t_scene *scene, t_vec3 hit_point, t_vec3 light_dir, double light_distance, t_hit_record hit_record);

//scenes
void	set_up_scene_triangle(t_scene *scene);
void	set_up_scene_plane(t_scene *scene);
void	set_up_scene_two_sphere(t_scene *scene);
void	set_up_scene_cylinder(t_scene *scene);
void	set_up_scene_mesh(t_scene *scene);
void	set_up_scene_cube(t_scene *scene);

//user interface
void 		init_control_panel(t_scene *scene);
void		draw_control_panel(t_scene *scene);
void		destroy_control_panel(t_scene *scene);


//camera
t_vec3		rotate_point(t_vec3 point, t_vec3 rotation);
t_vec3		get_forward_vector(t_vec3 rotation);
t_vec3		get_right_vector(t_vec3 rotation);
t_vec3		get_up_vector(t_vec3 rotation);

//controls
int				motion_handler(int x, int y, t_scene *scene);
int				control_mouse_handler(int button, int x, int y, t_scene *scene);
int				mouse_release(int button, int x, int y, t_scene *scene);


//Checkerboard
int				is_checker_point(t_vec3 point, double checker_size);
int				is_checker_point_plane(t_plane plane, t_vec3 point, double checker_size);
t_color			get_checker_color(t_material material, t_object *object, t_vec3 point);
t_vec2			spherical_map(t_vec3 point);
int				is_checker_point_2d(double u, double v, double checker_size);
#endif
