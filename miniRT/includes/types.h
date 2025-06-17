/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   types.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 10:29:05 by abillote          #+#    #+#             */
/*   Updated: 2025/06/10 12:55:49 by antonsplavn      ###   ########.fr       */
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

#  define WIDTH 1920
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
#define MAX_PANEL_TEXTS     16
#define NUM_CHECKBOXES      5

#define TOGGLE_BTN_OFFSET_X 10
#define TOGGLE_BTN_OFFSET_Y 10
#define TOGGLE_BTN_SIZE     30

// 3D rendering constants
# define FOV			60.0
# define NEAR_PLANE		0.1
# define FAR_PLANE		100.0
# define MAX_RAY_DEPTH	5
# define MAX_BVH_DEPTH	8
# define MAX_BVH_NODES	1000


# define BLACK      			0x000000  // RGB(0, 0, 0)
# define WHITE       			0xFFFFFF  // RGB(255, 255, 255)
# define RED         			0xFF0000  // RGB(255, 0, 0)
# define GREEN       			0x00FF00  // RGB(0, 255, 0)
# define BLUE        			0x0000FF  // RGB(0, 0, 255)

# define MAGENTA_BURST   		0xFF00FF
# define LIME_SHOCK     		0xCCFF00
# define NEON_ORANGE     		0xFF6600
# define PSYCHEDELIC_PURPLE		0x660066
# define AQUA_DREAM      		0x33CCCC
# define HOT_PINK        		0xFF66B2
# define ELECTRIC_BLUE   		0x0066FF
# define LAVA_RED        		0xFF3300


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

typedef struct s_light_result {
	double diffuse;
	double specular_intensity;
	double light_distance;
	t_vec3 light_dir;
} t_light_result;

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
	double	specular;
	double	shininess;
	double	reflectivity;
	double	transparency; // 0.0 = opaque 1.0 = fully transparent
	double	refractive_index; //Air = 1.0 Water = 1.33 Glass = 1.5 Diamond = 2.4
	int		has_checker;
	double	checker_size;
	t_color	checker_color;
	int		has_texture;
	t_texture	*texture;
	int		has_bump_map;
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
}	t_triangle;

typedef struct s_mesh
{
	t_triangle	*triangles;
	int			triangle_count;
	t_vec3		position;
	t_vec3		rotation;
	t_vec3		scale;
	t_material	material;
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
	t_vec2		uv; // For texture mapping
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
	int text_offset_x[MAX_PANEL_TEXTS];
    int text_offset_y[MAX_PANEL_TEXTS];
    mlx_image_t *panel_img;
    mlx_image_t *panel_text[MAX_PANEL_TEXTS];
    int text_count;
    mlx_image_t *status_text_img;
} t_panel;

typedef struct s_toggle_button {
    int offset_x, offset_y;
    int size;
    mlx_image_t *toggle_img;
} t_toggle_button;

typedef struct s_ui {
    t_panel panel;
    t_toggle_button toggle;
} t_ui;

typedef struct s_scene
{
	t_app				app;
	t_graphic_settings	graphic_settings;
	t_mouse_state		mouse_state;

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
	int					max_depth; //Maximum recursion depth (for reflections)

	double				escape_value;
	int					iterations_defintion;
	double				shift_x;
	double				shift_y;
	double				zoom;



	t_menger			menger;
}				t_scene;

typedef struct s_thread_data
{
	int			start_row;
	int			end_row;
	t_scene		*scene;
}	t_thread_data;

#endif
