/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol_mac.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 13:10:14 by asplavni          #+#    #+#             */
/*   Updated: 2025/04/26 08:47:17 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRACTOL_MAC_H
# define FRACTOL_MAC_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <math.h>
# include <pthread.h>
# include <ctype.h>
# include "minilibx_mms_20191025_beta/mlx.h"

// Define macOS key codes (different from X11 keysyms)
# define KEY_ESC 53
# define KEY_UP 126
# define KEY_DOWN 125
# define KEY_LEFT 123
# define KEY_RIGHT 124
# define KEY_PLUS 24
# define KEY_MINUS 27
# define KEY_EQUAL 24
# define KEY_R 15
# define KEY_M 46
# define KEY_1 18
# define KEY_2 19
# define KEY_3 20
# define KEY_4 21
# define KEY_5 23
# define KEY_W 13
# define KEY_A 0
# define KEY_S 1
# define KEY_D 2
# define KEY_BRACKET_LEFT 33
# define KEY_BRACKET_RIGHT 30
# define KEY_Q 12
# define KEY_E 14

// Mouse button codes
# define MOUSE_LEFT_BUTTON 1
# define MOUSE_RIGHT_BUTTON 2
# define MOUSE_SCROLL_UP 4
# define MOUSE_SCROLL_DOWN 5

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

// If you're using these keys in your code, they may also need to be defined
# define KEY_B 11
# define KEY_C 8
# define KEY_F 3
# define KEY_G 5
# define KEY_H 4
# define KEY_I 34
# define KEY_J 38
# define KEY_K 40
# define KEY_L 37
# define KEY_N 45
# define KEY_O 31
# define KEY_P 35
# define KEY_T 17
# define KEY_U 32
# define KEY_V 9
# define KEY_X 7
# define KEY_Y 16
# define KEY_Z 6

# define KEY_bracketleft KEY_BRACKET_LEFT
# define KEY_bracketright KEY_BRACKET_RIGHT
# define KEY_r KEY_R

typedef struct s_vec3
{
	double	x;
	double	y;
	double	z;
}				t_vec3;

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
	double	aspect_ratio;
	double	near;
	double	far;
}				t_camera;

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

typedef struct s_complex
{
	double	x;
	double	y;
}				t_complex;

typedef struct s_img
{
	void	*img_ptr;
	char	*pixels_ptr;
	int		bpp;
	int		endian;
	int		line_len;
}				t_img;

typedef struct s_scene
{
	char		*name;
	void		*mlx_connection;
	void		*mlx_window;
	t_img		img;

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

	// 3D specific fields
	t_camera	camera;
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
void        display_status(t_scene *scene);
void        display_progress(t_scene *scene, const char *status_text);

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
#endif
