/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   types.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 10:29:05 by abillote          #+#    #+#             */
/*   Updated: 2025/05/27 11:41:35 by abillote         ###   ########.fr       */
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
# include "../MLX42/include/MLX42/MLX42.h"

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
	t_material	material;
	t_object	*object;
	int			inside;
	int			triangle_idx;
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

//graphical settings
typedef struct graphic_settings
{
    bool    	enable_hard_shadows;
	bool		enable_reflections;
    bool    	enable_specular;
	bool		enable_refraction;
	int		resolution_factor; // For controlling render resolution
} t_graphic_settings;

typedef struct s_app
{
    mlx_t		*mlx;
    mlx_image_t	*img;

} t_app;

typedef struct s_scene
{
	t_app				app;
	t_graphic_settings	graphic_settings;
	t_mouse_state		mouse_state;

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
