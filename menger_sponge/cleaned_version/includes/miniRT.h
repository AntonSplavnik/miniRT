/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 10:35:54 by abillote          #+#    #+#             */
/*   Updated: 2025/05/20 09:57:53 by abillote         ###   ########.fr       */
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

# define WIDTH	1280
# define HEIGHT	1024
# define NUM_THREADS 8  // Number of threads for multithreaded rendering

// 3D rendering constants
# define FOV 60.0
# define NEAR_PLANE 0.1
# define FAR_PLANE 100.0
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
/*
typedef struct s_vec3
{
	double	x;
	double	y;
	double	z;
}				t_vec3;

typedef struct	s_color
{
	int	r;
	int g;
	int b;
}	t_color;

//material properties for objects (bonus)
//for bonuses: nothing mentionned so we can add the material properties in the rt files
typedef struct	s_material
{
	t_color	color;
	double	specular; //specular reflection coeff
	double	shininess; //shininess exponent for specular reflection
	double	reflectivity; //reflectivity coeff
	int		checkerboard; //flag for checkerboard pattern
	t_color	checker_color; //Secondary color for checkerboard
}	t_material;


//Ambient lighting
typedef struct s_ambient
{
	double	ratio; //Ambient lighting ratio (0.0 - 1.0)
	t_color	color;
}	t_ambient;

typedef struct	s_light
{
	t_vec3	position;
	double	intensity; //light brightness ratio (0.0 - 1.0)
	t_color	color; //light color (for bonus)
	struct s_light *next; //pointer to next light (bonuses)
}	t_light;

typedef struct s_aabb
{
	t_vec3	min;
	t_vec3	max;
}				t_aabb;

typedef struct s_bvh_node
{
	t_aabb				bounds;
	struct s_bvh_node	*left;
	struct s_bvh_node	*right;
	int					is_leaf;
	int					iteration;
}				t_bvh_node;

typedef struct s_camera
{
	t_vec3	position;
	t_vec3	rotation; //simple camera orientation, from parsing
	double	fov; //range 0-180

	//These three vectors together define a local coordinate system for the camera
	t_vec3	forwards; //forward vector
	t_vec3	right; //right vector
	t_vec3	up; //up vector

	double	aspect_ratio; //Initialized but not used yet
	double	near; //Initialized but not used yet
	double	far; //Initialized but not used yet

	double  movement_speed; //Speed for camera movement
	double	rotation_speed;	//Speed for camera rotation
}				t_camera;

typedef enum	e_object_type
{
	SPHERE,
	PLANE,
	CYLINDER,
	CUBE,
	TRIANGLE,
	MESH,
	CONE, //For bonus
	HYPERBOLOID, //For bonus
	PARABOLOID, //For bonus
}	t_object_type;

//Object structure, linked list
typedef struct	s_object
{
	void			*data; //pointer to store any object struct
	t_object_type	type;
	t_material		material;
	struct s_object	*next;
}	t_object;

typedef struct	s_sphere
{
	t_vec3	center; //from parsing
	double	diameter; //from parsing
	double	radius; //calculated from diameter
}	t_sphere;

typedef struct s_plane
{
	t_vec3	point; //from parsing
	t_vec3	normal; //from parsing
}	t_plane;

typedef struct s_cylinder
{
	t_vec3	center; //parsing
	t_vec3	axis; //parsing
	double	diameter; //parsing
	double	radius; //calculated from diameter
	double height; //parsing
}	t_cylinder;

typedef struct s_cube
{
	t_vec3	center; //center point of the cube
	double	side_length; //length of each side of the cube
}	t_cube;

typedef struct s_triangle
{
	t_vec3	v0; // First vertex
	t_vec3	v1; // Second vertex
	t_vec3	v2; // Third vertex
	t_vec3	normal; // Precomputed normal
}	t_triangle;

typedef struct s_mesh
{
	t_triangle	*triangles; // Array of triangles
	int			triangle_count; // Number of triangles
	t_vec3		position; // Center position of the mesh
	t_vec3		rotation; // Rotation of the mesh
	t_vec3		scale; // Scale of the mesh
}	t_mesh;

typedef struct	s_ray
{
	t_vec3	origin;
	t_vec3	direction; //normalized
}	t_ray;

typedef struct	s_hit_record
{
	double		t; //Ray parameter at the intersection
	t_vec3		point; //Point of intersection
	t_vec3		normal; //Surface normal at intersection
	t_material	material; //material of the intersected object
	t_object	*object; //Pointer to the intersected object
	int			inside; //Flag indicating if the ray is inside the object
}	t_hit_record;

typedef struct s_img
{
	void	*img_ptr;
	char	*pixels_ptr;
	int		bpp;
	int		endian;
	int		line_len;
}				t_img;

typedef struct s_menger
{
	int			iterations;
	double		size;
	t_vec3		position;
	t_vec3		rotation;
	t_bvh_node	*bvh_root;
}				t_menger;

typedef struct s_bounds
{
	double	new_min;
	double	new_max;
	double	old_min;
	double	old_max;
}	t_bounds;

// In the t_app struct or t_scene struct, add:
typedef struct s_control_panel {
    t_img   img;          // Using your existing t_img structure
    bool    initialized;  // Flag to track initialization status
} t_control_panel;

typedef struct s_app {
    void    *mlx;
    void    *win;
	void    *control_window;
    bool    enable_hard_shadows;
	bool    enable_reflections;
    bool    enable_specular;
	double	resolution_factor; // For controlling render resolution
	t_control_panel control_panel;  //store the control panel state
} t_app;


typedef struct s_scene
{
	char		*name; //input file name
	void		*mlx_connection; //MLX pointer
	void		*mlx_window; //MLW window pointer
	t_img		img; //Image struct
	t_app		app;

	int			width; // Window width
	int			height; //Window height

	int			background_color;

	t_ambient	ambient;
	t_camera	camera;
	t_light		*lights; //Linked list of lights
	t_object	*objects; //Linked list of objects

	//for bonuses
	int 		sample; //for anti-aliasing
	int			max_depth; //Maximum recursion depth (for reflections)

	double		escape_value;
	int			iterations_defintion;
	double		shift_x;
	double		shift_y;
	double		zoom;
	double		julia_x;
	double		julia_y;
	int			mouse_control;
	int			is_dragging;
	int			prev_mouse_x;
	int			prev_mouse_y;

	t_menger	menger;	//menger delete or repurpose later
	int			is_3d;	//menger delete or repurpose later
	int			resolution_factor;  // For controlling render resolution menger (delete later)
}				t_scene;

typedef struct s_thread_data
{
	int			start_row;
	int			end_row;
	t_scene	*scene;
}	t_thread_data;
 */



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
int			get_final_color(t_scene *scene, double light_intensity);
int			get_object_color(t_object *object, double light_intensity, t_color light_color);

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
int			find_closest_intersection(t_scene *scene, t_ray ray, double *t, t_object **hit_object);
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
void		render_complex_scene(t_scene *scene);

//shadows
int			is_in_shadow(t_scene *scene, t_vec3 hit_point, t_vec3 light_dir, double light_distance);

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
t_vec3 camera_to_world(t_scene *scene, t_vec3 camera_dir);
void update_camera_matrix(t_scene *scene);

//controls
int				motion_handler(int x, int y, t_scene *scene);
int				control_mouse_handler(int button, int x, int y, t_scene *scene);
int				mouse_release(int button, int x, int y, t_scene *scene);

#endif
