/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   miniRT.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 10:35:54 by abillote          #+#    #+#             */
/*   Updated: 2025/04/29 10:49:58 by abillote         ###   ########.fr       */
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
# include "platform_specifics.h"


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
	t_vec3	rotation; //Used for rotation of the camera
	t_vec3	orientation; //simple camera orientation, from parsing
	double	fov; //range 0-180

	//These three vectors together define a local coordinate system for the camera
	t_vec3	forwards; //forward vector
	t_vec3	right; //right vector
	t_vec3	up; //up vector

	double	aspect_ratio; //Initialized but not used yet
	double	near; //Initialized but not used yet
	double	far; //Initialized but not used yet
}				t_camera;

typedef enum	e_object_type
{
	SPHERE,
	PLANE,
	CYLINDER,
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

typedef struct s_scene
{
	char		*name; //input file name
	void		*mlx_connection; //MLX pointer
	void		*mlx_window; //MLW window pointer
	t_img		img; //Image struct

	int			width; // Window width
	int			height; //Window height

	t_ambient	ambiant;
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

	t_menger	menger;
	int			is_3d;
	int			resolution_factor;  // For controlling render resolution
}				t_scene;

typedef struct s_thread_data
{
	int			start_row;
	int			end_row;
	t_scene	*scene;
}	t_thread_data;

//events
int			close_handler(t_scene *scene);
int			key_handler(int keysym, t_scene *scene);
int			mouse_handler(int button, int x, int y, t_scene *scene);
int			mouse_release(int button, int x, int y, t_scene *scene);
void		display_status(t_scene *scene);
void		display_progress(t_scene *scene, const char *status_text);

//init
void		scene_init(t_scene *scene);

//render
void		pixel_put(int x, int y, t_img *img, int color);
void		scene_render(t_scene *scene);
void		draw_image_to_window(t_scene *scene);

//string utils
int			ft_strncmp(char *s1, char *s2, int n);
void		write_string_to_file_descriptor(char *str, int file_descriptor);

// 3D rendering functions
void		init_3d(t_scene *scene);
void		render_menger_sponge(t_scene *scene);
t_vec3		rotate_point(t_vec3 point, t_vec3 rotation);

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

#endif
