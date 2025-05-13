/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   types.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/13 10:29:05 by abillote          #+#    #+#             */
/*   Updated: 2025/05/13 10:42:14 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TYPES_H
# define TYPES_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <math.h>
# include <pthread.h>
# include <ctype.h>

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

typedef struct	s_material
{
	t_color	color;
	double	specular;
	double	shininess;
	double	reflectivity;
	int		checkerboard;
	t_color	checker_color;
}	t_material;

typedef struct s_ambient
{
	double	ratio;
	t_color	color;
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
	t_vec3	rotation;
	double	fov;
	t_vec3	forwards;
	t_vec3	right;
	t_vec3	up;
	double	aspect_ratio;
	double	near;
	double	far;
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
}	t_mesh;

typedef struct s_cone
{
	t_vec3  tip;
	t_vec3  axis;
	double  height;
	double  radius;
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
	char		*name;
	void		*mlx_connection;
	void		*mlx_window;
	t_img		img;
	int			width;
	int			height;
	t_ambient	ambient;
	t_camera	camera;
	t_light		*lights;
	t_object	*objects;
	int 		sample;
	int			max_depth;
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
	int			resolution_factor;
}				t_scene;

typedef struct s_thread_data
{
	int			start_row;
	int			end_row;
	t_scene	*scene;
}	t_thread_data;

#endif
