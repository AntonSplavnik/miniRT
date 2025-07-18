/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bvh.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 00:00:00 by antonsplavn      #+#    #+#             */
/*   Updated: 2025/06/23 00:00:00 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BVH_H
# define BVH_H

# include "types.h"
# include "miniRT.h"

# define MAX_STACK_SIZE 64
# define LEAF_TRI_THRESHOLD 4
# define MAX_BVH_DEPTH	8
# define MAX_BVH_NODES	1000

typedef struct s_sort_params
{
	t_object	**objects;
	t_aabb		*bounds;
	int			start;
	int			end;
	int			axis;
}	t_sort_params;

typedef struct s_bvh_params
{
	t_object	**objects;
	t_aabb		*bounds;
	int			start;
	int			end;
	int			depth;
}	t_bvh_params;

typedef struct s_ray_aabb_params
{
	t_aabb		bounds;
	t_vec3		ray_origin;
	t_vec3		ray_dir;
	double		*t_min;
	double		*t_max;
}	t_ray_aabb_params;

typedef struct s_leaf_intersection_params
{
	t_object		*obj;
	t_ray			ray;
	double			*closest_t;
	t_object		**hit_object;
	t_hit_record	*hit_record;
}	t_leaf_intersection_params;

typedef struct s_scene_ray_params
{
	t_scene			*scene;
	t_ray			ray;
	double			*t;
	t_object		**hit_object;
	t_hit_record	*hit_record;
}	t_scene_ray_params;

typedef struct s_node_children_params
{
	t_bvh_node	*node;
	t_object	**objects;
	t_aabb		*bounds;
	int			start;
	int			end;
	int			depth;
}	t_node_children_params;

typedef struct s_process_node_params
{
	t_bvh_node	*node;
	t_ray		ray;
	double		closest_t;
	t_bvh_node	**stack;
	int			*stack_size;
}	t_process_node_params;

typedef struct s_bvh_traverse_params
{
	t_scene_ray_params	ray_params;
	t_bvh_node			**stack;
	int					*stack_size;
	double				*closest_t;
}	t_bvh_traverse_params;

/* calculate_object_aabb functions */
t_aabb		calculate_object_aabb(t_object *object);
t_aabb		calculate_sphere_aabb(t_sphere *sphere);
t_aabb		calculate_plane_aabb(t_plane *plane);
t_aabb		calculate_cylinder_aabb(t_cylinder *cylinder);
t_aabb		calculate_cylinder_aabb_helper(t_vec3 top, t_vec3 bottom, double radius);
t_aabb		calculate_cube_aabb(t_cube *cube);
t_aabb		calculate_cone_aabb(t_cone *cone);
t_aabb		calculate_cone_aabb_helper(t_vec3 apex, t_vec3 base_center,
				double radius);
t_aabb		calculate_triangle_aabb(t_triangle *triangle);
t_aabb		calculate_mesh_aabb(t_mesh *mesh);
void		adjust_mesh_aabb(t_mesh *mesh, t_aabb *bounds);
void		handle_negative_scale(t_aabb *bounds, t_vec3 scale);
void		update_mesh_min_bounds(t_triangle *tri, t_aabb *bounds);
void		update_mesh_max_bounds(t_triangle *tri, t_aabb *bounds);
void		pad_triangle_aabb_x(t_aabb *bounds, double epsilon);
void		pad_triangle_aabb_y(t_aabb *bounds, double epsilon);
void		pad_triangle_aabb_z(t_aabb *bounds, double epsilon);
void		pad_triangle_aabb(t_aabb *bounds);

/* build_scene_bvh functions */
t_bvh_node	*build_scene_bvh(t_scene *scene);
t_bvh_node	*build_scene_bvh_recursive(t_bvh_params params);
void		compute_total_bounds(t_aabb *total_bounds, t_aabb *bounds,
				int start, int end);
int			find_split_axis(t_aabb total_bounds);
void		sort_objects_by_axis(t_sort_params params);
void		swap_objects(t_object **objects, t_aabb *bounds, int i, int j);

/* build_scene_bvh_helpers functions */
t_bvh_node	*create_bvh_node(void);
t_bvh_node	*create_leaf_node(t_object **objects, t_aabb *bounds, int start);
void		build_node_children(t_node_children_params params);

/* build_scene_bvh_utils functions */
int			count_scene_objects(t_scene *scene);
void		fill_objects_and_bounds(t_scene *scene, t_object **objects,
				t_aabb *bounds, int object_count);

/* scene_ray_intersect_bvh functions */
int			scene_ray_intersect_bvh(t_scene_ray_params params);
int			test_leaf_node_intersection(t_leaf_intersection_params params);
void		process_bvh_node(t_process_node_params params);
int			traverse_bvh(t_bvh_traverse_params params);
int			intersect_primitive(t_object *obj, t_ray ray, double *t_temp,
				int *triangle_idx);
int			intersect_sphere(t_ray ray, t_sphere *sphere, double *t_temp);
int			intersect_plane(t_ray ray, t_plane *plane, double *t_temp);
int			intersect_cylinder(t_ray ray, t_cylinder *cylinder, double *t_temp);
int			intersect_cube(t_ray ray, t_cube *cube, double *t_temp);
int			intersect_triangle(t_ray ray, t_triangle *triangle, double *t_temp);
int			intersect_triangle_bary(t_ray ray, t_triangle *triangle, double *t_temp, t_vec3 *bary);
int			intersect_mesh(t_ray ray, t_mesh *mesh, double *t_temp, int *triangle_idx);
int			intersect_mesh_bary(t_ray ray, t_mesh *mesh, double *t_temp, int *triangle_idx, t_vec3 *bary);
int			intersect_cone(t_ray ray, t_cone *cone, double *t_temp);
int			intersect_primitive(t_object *obj, t_ray ray, double *t_temp,
				int *triangle_idx);
int			intersect_primitive_bary(t_object *obj, t_ray ray, double *t_temp,
				int *triangle_idx, t_vec3 *bary);

/* ray_intersect_aabb functions */
int			ray_intersect_aabb_scalar(t_ray_aabb_params params);
int			calculate_x_intersection(t_aabb bounds, t_vec3 ray_origin, t_vec3 ray_dir,
				double *near_far);
int			calculate_y_intersection(t_aabb bounds, t_vec3 ray_origin, t_vec3 ray_dir,
				double *near_far);
int			calculate_z_intersection(t_aabb bounds, t_vec3 ray_origin, t_vec3 ray_dir,
				double *near_far);
void		swap_values(double *a, double *b);

/* mesh_bvh functions */
void		build_mesh_bvh(t_mesh *mesh);
int			mesh_bvh_intersect(t_ray ray, t_mesh *mesh, double *t, int *tri_idx);
int			mesh_bvh_intersect_bary(t_ray ray, t_mesh *mesh,
				t_bvh_bary_params *bary_params);
void		free_mesh_bvh(t_mesh *mesh);
t_aabb		calculate_transformed_tri_aabb(t_mesh *mesh, int tri_idx);
double		calculate_aabb_surface_area(t_aabb box);

/* bvh_utils functions */
void		ft_swap_centroid(t_triangle_centroid *a, t_triangle_centroid *b);
t_aabb		aabb_union(t_aabb a, t_aabb b);
double		get_centroid_axis_value(t_triangle_centroid *centroid, int axis);

/* bvh_centroids functions */
void		precompute_triangle_centroids(t_mesh *mesh,
				t_triangle_centroid *centroids);
void		copy_indices_from_centroids(t_mesh *mesh,
				t_triangle_centroid *centroids, int *tri_indices);
t_aabb		compute_bounds_from_centroids(t_mesh *mesh,
				t_triangle_centroid *centroids, int start, int end);
int			find_best_split_axis_fast(t_triangle_centroid *centroids,
				int start, int end);
int			partition_by_threshold_fast(t_triangle_centroid *centroids,
				t_partition_params *params);

/* bvh_split functions */
int			split_node_fast(t_split_node_params *split_params);
int			process_build_node_fast(t_process_build_params *build_params);
void		build_mesh_bvh_fast(t_mesh *mesh, t_triangle_centroid *centroids);

/* bvh_allocation functions */
int			validate_bvh_allocation(t_mesh *mesh);
int			allocate_bvh_memory(t_mesh *mesh);
void		process_leaf_node(t_mesh *mesh, t_split_params *params);
int			setup_node_bounds(t_process_build_params *build_params,
				t_split_params *params);
int			handle_leaf_or_split(t_process_build_params *build_params,
				t_split_params *params, int tri_count);
int			get_triangle_original_index(t_mesh *mesh, int idx);
int			is_valid_triangle_index(int original_idx, int triangle_count);

/* bvh_intersect_utils functions */
int			ray_intersect_aabb_optimized(t_ray ray, t_aabb bounds, double max_t);
int			ray_triangle_intersect_fast(t_ray ray, t_transformed_triangle tri,
				double *t);
int			get_triangle_original_index(t_mesh *mesh, int idx);
int			is_valid_triangle_index(int original_idx, int triangle_count);

/* bvh_intersect_leaves functions */
int			test_single_triangle(t_mesh *mesh, t_ray ray, int idx,
				t_intersect_result *result);
int			test_leaf_triangles_batch(t_mesh *mesh, t_ray ray,
				t_leaf_batch_params *leaf_params);
int			test_single_triangle_bary(t_mesh *mesh, t_ray ray, int idx,
				t_intersect_bary_result *result);
int			test_leaf_triangles_batch_bary(t_mesh *mesh, t_ray ray,
				t_leaf_batch_bary_params *leaf_params);

/* bvh_traversal functions */
int			process_bvh_bary_traversal(t_bvh_bary_traverse_context *ctx);
int			initialize_bvh_traversal(t_mesh *mesh, t_ray ray,
				t_bvh_traverse_context *ctx);
int			process_bvh_traversal(t_bvh_traverse_context *ctx);
int			initialize_bvh_bary_traversal(t_mesh *mesh, t_ray ray,
				t_bvh_bary_traverse_context *ctx);
int			process_bvh_node_optimized(t_bvh_process_params *proc_params);
int			process_bvh_node_bary(t_bvh_process_bary_params *proc_params);
int			handle_leaf_node(t_bvh_process_params *proc_params,
				int left_index, int right_index);
void		push_internal_nodes(t_bvh_process_params *proc_params,
				int left_index, int right_index);
int			process_leaf_batch(t_mesh *mesh, t_ray ray,
				t_leaf_batch_context *ctx);
int			handle_leaf_node_bary(t_bvh_process_bary_params *proc_params,
				int left_index, int right_index);
void		push_internal_nodes_bary(t_bvh_process_bary_params *proc_params,
				int left_index, int right_index);

#endif
