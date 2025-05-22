/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fractol.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: parallels <parallels@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 13:10:14 by asplavni          #+#    #+#             */
/*   Updated: 2025/04/11 17:59:04 by parallels        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FRACTOL_H
# define FRACTOL_H

#ifdef __APPLE__
    # include <mlx.h>
    // macOS-specific headers
#else
    # include <X11/X.h>
    # include <X11/keysym.h>
    // Other Linux-specific headers
#endif

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <math.h>
# include <pthread.h>
# include <ctype.h>
# include "minilibx-linux/mlx.h"

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

// Forward declare the quaternion structure used in 3D Mandelbrot
typedef struct s_quaternion
{
    double w, x, y, z;
} t_quaternion;

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

typedef struct s_fractal
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
}				t_fractal;

typedef struct s_thread_data
{
	int			start_row;
	int			end_row;
	t_fractal	*fractal;
}	t_thread_data;

//events
int			close_handler(t_fractal *fractal);
int			key_handler(int keysym, t_fractal *fractal);
int			mouse_handler(int button, int x, int y, t_fractal *fractal);
int			mouse_release(int button, int x, int y, t_fractal *fractal);
int			julia_track(int x, int yj, t_fractal *fractal);
void        display_status(t_fractal *fractal);
void        display_progress(t_fractal *fractal, const char *status_text);

//handle_pixel
void		handle_pixel(int x, int y, t_fractal *fractal);

//init
void		fractal_init(t_fractal *fractal);

//math utils
double		map(double unscaled_num, t_bounds bounds);
int			is_valid_double(const char *str);
t_complex	sum_complex(t_complex z1, t_complex z2);
t_complex	square_complex(t_complex z);

//render_fractal_progressive
void		fractal_render_progressive(t_fractal *fractal);

//render
void		pixel_put(int x, int y, t_img *img, int color);
void		fractal_render(t_fractal *fractal);
void		fractal_render_multithreaded(t_fractal *fractal);
void		fractal_render_hybrid(t_fractal *fractal);
void		fractal_render_single_thread(t_fractal *fractal);
void		render_pixel_row(int y, t_fractal *fractal, t_complex z);
void		draw_image_to_window(t_fractal *fractal);
void		mandelbrot_vs_julia(t_complex *complex_z,
				t_complex *complex_c, t_fractal *fractal);

//string utils
int			ft_strncmp(char *s1, char *s2, int n);
double		string_to_double(char *str);
void		write_string_to_file_descriptor(char *str, int file_descriptor);

//thread_render
void		*thread_render(void *arg);

// 3D rendering functions
void		init_3d(t_fractal *fractal);
void		render_menger_sponge(t_fractal *fractal);
t_vec3		rotate_point(t_vec3 point, t_vec3 rotation);
void		project_point(t_vec3 point, t_fractal *fractal, int *x, int *y);
int			is_point_in_menger(t_vec3 point, int iterations);

// BVH functions
t_bvh_node	*build_menger_bvh(int max_iterations);
void		free_bvh(t_bvh_node *node);
int			ray_intersect_bvh(t_bvh_node *node, t_vec3 ray_origin, 
							t_vec3 ray_dir, double *t_min, double *t_max);
int			ray_intersect_aabb_scalar(t_aabb bounds, t_vec3 ray_origin, 
							t_vec3 ray_dir, double *t_min, double *t_max);
int			ray_intersect_aabb_simd(t_aabb bounds, t_vec3 origin, 
									t_vec3 dir, double *out_tmin, double *out_tmax);

// 3D Mandelbrot functions
void        init_mandelbrot3d(t_fractal *fractal);
void        render_mandelbrot3d(t_fractal *fractal);
void        render_mandelbrot3d_progressive(t_fractal *fractal);
t_vec3      vec3_add(t_vec3 a, t_vec3 b);
t_vec3      vec3_sub(t_vec3 a, t_vec3 b);
t_vec3      vec3_mul(t_vec3 a, double t);
double      vec3_dot(t_vec3 a, t_vec3 b);
double      vec3_length(t_vec3 v);
t_vec3      vec3_normalize(t_vec3 v);
t_quaternion quat_add(t_quaternion a, t_quaternion b);
t_quaternion quat_mul(t_quaternion a, t_quaternion b);
double      quat_length_sq(t_quaternion q);
void        generate_ray(int x, int y, t_fractal *fractal, t_vec3 *ray_origin, t_vec3 *ray_direction);
double      mandelbrot3d_DE(t_vec3 pos, t_fractal *fractal);
int         ray_march(t_vec3 origin, t_vec3 direction, t_fractal *fractal, double *hit_distance, t_vec3 *hit_point, t_vec3 *hit_normal);
int         get_mandelbrot3d_color(t_vec3 hit_point, t_vec3 hit_normal, t_vec3 ray_direction, t_fractal *fractal);
void        *render_mandelbrot3d_thread(void *arg);
#endif