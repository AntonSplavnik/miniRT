/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   types.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 10:29:05 by abillote          #+#    #+#             */
/*   Updated: 2025/07/21 00:51:30 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPES_H
# define TYPES_H

# include <ctype.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <stdint.h>
# include <unistd.h>
# include <math.h>
# include <string.h>
# include <stdbool.h>
# include <pthread.h>
# include <fcntl.h>
# include "../MLX42/include/MLX42/MLX42.h"

# define WIDTH 1920
# define HEIGHT 1080

# define NUM_THREADS 8 // Number of threads for multithreaded rendering

// UI constants
#define PANEL_X             10
#define PANEL_Y             50
#define PANEL_WIDTH         260
#define PANEL_HEIGHT        400
#define PANEL_HEADER_HEIGHT 40
#define PANEL_PADDING       16
#define CHECKBOX_SIZE       16
#define CHECKBOX_SPACING    38
#define MAX_PANEL_TEXT      20
#define NUM_CHECKBOXES      5

#define TOGGLE_BTN_OFFSET_X 10
#define TOGGLE_BTN_OFFSET_Y 10
#define TOGGLE_BTN_SIZE		30

// 3D rendering constants
# define FOV					60.0
# define CAMERA_MOVEMENT_SPEED	0.5
# define CAMERA_ROTATION_SPEED	0.05
# define NEAR_PLANE				0.1
# define FAR_PLANE				100.0
# define MAX_RAY_DEPTH			8


# define BLACK					0x000000  // RGB(0, 0, 0)
# define WHITE					0xFFFFFF  // RGB(255, 255, 255)
# define RED					0xFF0000  // RGB(255, 0, 0)
# define GREEN					0x00FF00  // RGB(0, 255, 0)
# define BLUE					0x0000FF  // RGB(0, 0, 255)

# define MAGENTA_BURST			0xFF00FF
# define LIME_SHOCK				0xCCFF00
# define NEON_ORANGE			0xFF6600
# define PSYCHEDELIC_PURPLE		0x660066
# define AQUA_DREAM				0x33CCCC
# define HOT_PINK				0xFF66B2
# define ELECTRIC_BLUE			0x0066FF
# define LAVA_RED				0xFF3300

typedef struct s_vec2
{
	double	u;
	double	v;
}				t_vec2;

typedef struct s_vec3
{
	double	x;
	double	y;
	double	z;
}				t_vec3;


typedef struct s_counts
{
	int	vertex_count;
	int	face_count;
	int	normal_count;
}	t_counts;

// Add new floating-point color type
//Linear RGB values, typically [0.0, 1.0] but can exceed for HDR
typedef struct s_color_f
{
	double r;
	double g;
	double b;
} t_color_f;

// Update light result to use floating-point
//Used to check cube intersection
typedef struct s_slab
{
	double	slab_x_dir;
	double	slab_x_origin;
	double	slab_x_min;
	double	slab_x_max;
	double	slab_y_dir;
	double	slab_y_origin;
	double	slab_y_min;
	double	slab_y_max;
	double	slab_z_dir;
	double	slab_z_origin;
	double	slab_z_min;
	double	slab_z_max;
}	t_slab;

typedef struct s_light_result
{
    double diffuse;
    double specular_intensity;
    double light_distance;
    t_vec3 light_dir;
} t_light_result;

typedef struct s_fresnel_params
{
	double	cos_i;
	double	eta_i;
	double	eta_t;
}	t_fresnel_params;

typedef struct s_material_contrib
{
	double	total_contrib;
	double	fresnel;
	double	transmission;
}	t_material_contrib;

typedef struct s_refraction_params
{
	t_vec3	refract_dir;
	bool	total_internal_reflection;
}	t_refraction_params;


typedef struct	s_color
{
	int	r;
	int g;
	int b;
}	t_color;

typedef struct s_texture
{
	char	*filename;
	int		width;
	int		height;
	unsigned char	*data; // Raw pixel data (RGB or RGBA)
	void	*mlx_texture; // Pointer to the MLX42 texture object
}	t_texture;


/* Structure to hold pixel coordinates for bump mapping */
typedef struct s_pixel_coords
{
	double	px;
	double	py;
	int		x0;
	int		y0;
	int		x1;
}	t_pixel_coords;

/* Structure to hold bump map height samples */
typedef struct s_bump_heights
{
	float	du;
	float	dv;
	float	center;
	float	u;
	float	v;
}	t_bump_heights;

/* Structure to hold calculated gradients for bump mapping */
typedef struct s_bump_gradients
{
	float	gradient_u;
	float	gradient_v;
}	t_bump_gradients;

typedef struct s_bump_map
{
	char	*filename;
	int		width;
	int		height;
	float	*elevation; // height values (between 0.0 and 1.0)
}	t_bump_map;

typedef struct	s_material
{
	t_color	color;
	double		specular;			// 0 - 1 intencity of specular highlight
	double		shininess;			// 0 - 50 plastic ; 50 - 200+ metals; (more - smaller the area)
	double		reflectivity;		// 0.0 - 1.0
	double		transparency; 		// 0.0 = opaque 1.0 = fully transparent
	double		refractive_index;	// Air = 1.0 Water = 1.33 Glass = 1.5 Diamond = 2.4
	bool		has_refraction;
	int			has_checker;
	double		checker_size;
	t_color		checker_color;
	int			has_texture;
	t_texture	*texture;
	int			has_bump_map;
	t_bump_map	*bump_map;
}	t_material;

typedef struct s_ambient
{
	double	ratio;
	t_color	color;
	int		has_ambient;
}	t_ambient;

typedef struct	s_light
{
	t_vec3	position;
	double	intensity;
	t_color	color;
	struct s_light *next;
}	t_light;

typedef struct s_aabb
{
	t_vec3	min;
	t_vec3	max;
}				t_aabb;

/* BVH structures (needed here because they're embedded in other structs) */
typedef struct s_bvh_node
{
	t_aabb				bounds;
	struct s_bvh_node	*left;
	struct s_bvh_node	*right;
	int					is_leaf;
	int					iteration;
	void				*object_ref;  // Added for scene BVH to reference objects
}				t_bvh_node;

typedef struct s_mesh_bvh
{
	t_aabb		*nodes;
	int			*tri_indices;
	int			*node_children;
	int			node_count;
	int			max_nodes;
}	t_mesh_bvh;

typedef struct s_camera
{
	t_vec3	position;
	t_vec3	direction;
	t_vec3	rotation;
	double	fov;
	t_vec3	forwards;
	t_vec3	right;
	t_vec3	up;
	double	aspect_ratio;
	double	near;
	double	far;
	int		has_camera;

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
	CONE,
	HYPERBOLOID,
	PARABOLOID,
}	t_object_type;

typedef struct	s_object
{
	void			*data;
	t_object_type	type;
	t_material		material;
	struct s_object	*next;
}	t_object;

typedef struct	s_sphere
{
	t_vec3	center;
	double	diameter;
	double	radius;
}	t_sphere;

typedef struct s_plane
{
	t_vec3	point;
	t_vec3	normal;
}	t_plane;

typedef struct s_cylinder
{
	t_vec3	center;
	t_vec3	axis;
	double	diameter;
	double	radius;
	double height;
}	t_cylinder;

typedef struct s_cube
{
	t_vec3	center;
	double	side_length;
}	t_cube;

typedef struct s_triangle
{
	t_vec3	v0;
	t_vec3	v1;
	t_vec3	v2;
	t_vec3	normal;
	t_vec3	n0;
	t_vec3	n1;
	t_vec3	n2;
	int		has_vertex_normals;
}	t_triangle;

typedef struct s_triangle_data
{
	t_vec3	edge1;
	t_vec3	edge2;
	t_vec3	h;
	t_vec3	q;
	double	a;
	double	f;
	double	u;
	double	v;
}	t_triangle_data;


typedef struct s_process_data
{
	t_triangle	*triangles;
	t_vec3		*vertices;
	int			*t_idx;
	t_vec3		*normals;
}	t_process_data;

typedef struct s_mesh_data
{
	t_vec3		*vertices;
	t_triangle	*triangles;
	t_vec3		*normals;
	int			*t_idx;
}	t_mesh_data;

typedef struct s_face_data
{
	int		*indices;
	int		*t_idx;
	t_vec3	*normals;
	int		*normal_indices;
}	t_face_data;

typedef struct s_transformed_triangle
{
	t_vec3 v0, v1, v2;		// Vertices (already transformed)
	t_vec3 normal;			// Precomputed normal
	t_vec3 n0, n1, n2;		// Vertex normals (already transformed)
	int has_vertex_normals;
} t_transformed_triangle;

typedef struct s_mesh
{
	t_triangle				*triangles;
	t_transformed_triangle	*transformed_tris; // Precomputed triangles
	t_mesh_bvh				bvh;  // Flat array BVH

	int						triangle_count;
	t_vec3					position;
	t_vec3					rotation;
	t_vec3					scale;
	t_material				material;
}	t_mesh;


typedef struct s_cone
{
	t_vec3	apex;
	t_vec3	axis;
	double	height;
	double	radius;
	double	angle;
} t_cone;

typedef struct	s_ray
{
	t_vec3	origin;
	t_vec3	direction;
}	t_ray;

typedef struct s_mesh_intersect_data
{
	t_ray	ray;
	double	closest_t;
	int		hit_something;
	int		*triangle_idx;
}	t_mesh_intersect_data;

typedef struct s_mesh_bary_data
{
	t_ray	ray;
	double	closest_t;
	int		hit_something;
	int		*triangle_idx;
	t_vec3	closest_bary;
}	t_mesh_bary_data;

typedef struct s_mesh_bary_params
{
	double	*t;
	int		*triangle_idx;
	t_vec3	*bary;
}	t_mesh_bary_params;

typedef struct s_mesh_intersect_params
{
	t_ray	ray;
	t_mesh	mesh;
	double	*t;
	int		*triangle_idx;
	t_vec3	*bary;
}	t_mesh_intersect_params;

typedef struct	s_hit_record
{
	double		t;
	t_vec3		point;
	t_vec3		normal;
	t_vec3		original_normal;
	t_material	material;
	t_object	*object;
	int			inside;
	int			triangle_idx;
	t_vec3		barycentric;
	t_vec2		uv; // For texture mapping
}	t_hit_record;

/**
 * Structure to hold intersection parameters
 */
typedef struct s_intersection_params
{
	double	closest_t;
	int		triangle_idx;
}	t_intersection_params;

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

typedef struct s_mouse_state
{
	bool	is_dragging;
	bool	left_button_down;
	bool	right_button_down;
	bool	middle_button_down;

	int32_t	prev_mouse_x;
	int32_t	prev_mouse_y;

	int32_t	x;
	int32_t	y;

	int		original_ssaa;

} t_mouse_state;

typedef struct graphic_settings
{
    bool    	enable_hard_shadows;
	bool		enable_reflections;
    bool    	enable_specular;
	bool		enable_refraction;
	bool		enable_status_message;

	int			ssaa_samples;

	int			resolution_factor;
} t_graphic_settings;

typedef struct s_app
{
    mlx_t		*mlx;
    mlx_image_t	*img;

} t_app;

typedef struct s_checkbox_pos
{
	int		x;
	int		y;
	bool	checked;
}	t_checkbox_pos;

typedef struct s_panel
{
    bool visible;
    int current_width;
    int target_width;
    int height;
    int x;
	int	y;
	int drag_offset_x;
	int drag_offset_y;
    int animation_speed;
    int padding;
    int header_height;
    int checkbox_size;
    int checkbox_spacing;
	int text_offset_x[MAX_PANEL_TEXT];
    int text_offset_y[MAX_PANEL_TEXT];
    mlx_image_t *panel_img;
    mlx_image_t *panel_text[MAX_PANEL_TEXT];
    int text_count;
    mlx_image_t *status_text_img;
} t_panel;

typedef struct s_toggle_button
{
	int offset_x, offset_y;
	int size;
	mlx_image_t *toggle_img;
} t_toggle_button;

typedef struct s_ui
{
	t_panel panel;
	t_toggle_button toggle;
} t_ui;

typedef struct s_scene
{
	t_app				app;
	t_graphic_settings	graphic_settings;
	t_mouse_state		mouse_state;
	t_bvh_node			*scene_bvh; // Root of scene-wide BVH
	int					object_count;

	t_ui				ui; //UI panel and toggle button

	char				*name; //input file name

	int					width; // Window width
	int					height; //Window height

	int					background_color;

	t_ambient			ambient;
	t_camera			camera;
	t_light				*lights; //Linked list of lights
	t_object			*objects; //Linked list of objects

	//for bonuses
	int 				sample; //for anti-aliasing

	double				escape_value;
	int					iterations_defintion;
	double				shift_x;
	double				shift_y;
	double				zoom;

	t_menger			menger;
}				t_scene;

typedef struct s_ray_context
{
	t_scene			*scene;
	t_ray			ray;
	t_hit_record	hit_record;
	int				depth;
}	t_ray_context;

typedef struct s_light_params
{
	t_scene			*scene;
	t_hit_record	hit_record;
	t_light			*light;
	double			shadow_attenuation;
	t_vec3			direct_color;
}	t_light_params;

/* Structure to hold rendering parameters for performance optimization */
typedef struct s_render_params
{
	t_scene		*scene;
	double		fov_scale;
	int			samples;
	t_vec2		xy_coords;
	t_vec3		final_color;
}	t_render_params;

typedef struct s_thread_data
{
	int			start_row;
	int			end_row;
	t_scene		*scene;
}	t_thread_data;

#endif
