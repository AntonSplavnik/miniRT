/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_scene.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 10:45:02 by abillote          #+#    #+#             */
/*   Updated: 2025/05/05 10:28:58 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"
/*
double	calculate_shadow_attenuation(t_scene *scene, t_vec3 hit_point,
	t_vec3 light_dir, double light_distance, t_hit_record hit_record)
{
	t_ray		shadow_ray;
	t_object	*hit_object;
	t_object	*last_object;
	double		t;
	double		attenuation;
	int			max_iterations;

	t_vec3 offset = vec3_scale(hit_record.normal, 0.001);
	shadow_ray.origin = vec3_add(hit_point, offset);
	shadow_ray.direction = light_dir;
	attenuation = 1.0;
	max_iterations = 5;
	last_object = NULL;
	while (max_iterations > 0 && find_closest_intersection(scene, shadow_ray, \
		&t, &hit_object, &hit_record) && t < light_distance)
	{
		if (hit_object->material.transparency > 0.001)
		{
			if (hit_object != last_object)
			{
				attenuation *= hit_object->material.transparency;
				last_object = hit_object;
			}
			if (attenuation < 0.001)
				return (0.0);
			shadow_ray.origin = vec3_add(shadow_ray.origin, \
				vec3_scale(light_dir, t + 0.001));
			light_distance -= t;
			max_iterations--;
		}
		else
			return (0.0);
	}
	return (attenuation);
} */

double	calculate_shadow_attenuation(t_scene *scene, t_vec3 hit_point,
			t_vec3 light_dir, double light_distance, t_hit_record hit_record)
{
	t_ray		shadow_ray;
	t_object	*hit_object;
	double		t;

	t_vec3 offset = vec3_scale(hit_record.normal, 0.001);
	shadow_ray.origin = vec3_add(hit_point, offset);
	shadow_ray.direction = light_dir;
	if (find_closest_intersection(scene, shadow_ray, &t, &hit_object,
		&hit_record) && t < light_distance)
	{
		if (hit_object->material.transparency > 0.001)
			return (hit_object->material.transparency);
		else
			return (0.0);
	}
	return (1.0);
}

int	is_in_shadow(t_scene *scene, t_vec3 hit_point, t_vec3 light_dir, 
					double light_distance, t_hit_record hit_record)
{
	double	shadow_factor;

	shadow_factor  = calculate_shadow_attenuation(scene, hit_point, light_dir, \
		 light_distance, hit_record);
	return (shadow_factor < 0.999);
}

int	find_closest_intersection(t_scene *scene, t_ray ray, double *t, t_object **hit_object, t_hit_record *hit_record)
{
	// If we have a scene BVH, use it for faster intersection testing
	if (scene->scene_bvh)
		return scene_ray_intersect_bvh((t_scene_ray_params){scene, ray, t, hit_object, hit_record});

	// Otherwise, fall back to the original linear search approach
	t_object	*current;
	double		t_closest;
	double		t_temp;
	int			hit_something;
	int			triangle_idx;

	current = scene->objects;
	t_closest = INFINITY;
	hit_something = 0;
	*hit_object = NULL;
	while (current)
	{
		if (current->type == SPHERE)
		{
			t_sphere *sphere = (t_sphere *)(current->data);
			if (ray_sphere_intersect(ray, *sphere, &t_temp) && t_temp < t_closest)
			{
				t_closest = t_temp;
				hit_something = 1;
				*hit_object = current;
			}
		}
		else if (current->type == CYLINDER)
		{
			t_cylinder *cylinder = (t_cylinder *)(current->data);
			if (ray_cylinder_intersect(ray, *cylinder, &t_temp) && t_temp < t_closest)
			{
				t_closest = t_temp;
				hit_something = 1;
				*hit_object = current;
			}
		}
		else if (current->type == PLANE)
		{
			t_plane	*plane = (t_plane *)(current->data);
			if (ray_plane_intersect(ray, *plane, &t_temp) && t_temp < t_closest)
			{
				t_closest = t_temp;
				hit_something = 1;
				*hit_object = current;
			}
		}
		else if (current->type == CUBE)
		{
			t_cube	*cube = (t_cube *)(current->data);
			if (ray_cube_intersect(ray, *cube, &t_temp) && t_temp < t_closest)
			{
				t_closest = t_temp;
				hit_something = 1;
				*hit_object = current;
			}
		}
		else if (current->type == TRIANGLE)
		{
			t_triangle *triangle = (t_triangle *)(current->data);
			if (ray_triangle_intersect_bary(ray, *triangle, &t_temp, &hit_record->barycentric) && t_temp < t_closest)
			{
				t_closest = t_temp;
				hit_something = 1;
				*hit_object = current;
			}
		}
		else if (current->type == MESH)
		{
			t_mesh *mesh = (t_mesh *)(current->data);
			t_mesh_intersect_params params = {ray, *mesh, &t_temp, &triangle_idx, &hit_record->barycentric};
			if (ray_mesh_intersect_bary(params) && t_temp < t_closest)
			{
				t_closest = t_temp;
				hit_something = 1;
				*hit_object = current;
				// Store the triangle index in the hit object for later normal computation
				// Note: This is a hack - we're assuming the user won't mess with the pointer
				// In a real implementation, this should be stored in a hit record structure
				hit_record->triangle_idx = triangle_idx;
			}
		}
		else if (current->type == CONE)
		{
			t_cone *cone= (t_cone *)(current->data);
			if (ray_cone_intersect(ray, *cone, &t_temp) && t_temp < t_closest)
			{
				t_closest = t_temp;
				hit_something = 1;
				*hit_object = current;
			}
		}
		current = current->next;
	}
	if (hit_something)
		*t = t_closest;
	return (hit_something);
}

void compute_ray_intersection(t_ray ray, t_object *hit_object, double t, t_hit_record *hit_record)
{
	hit_record->point = vec3_add(ray.origin, vec3_scale(ray.direction, t));
	hit_record->t = t;
	hit_record->object = hit_object;
	hit_record->material = hit_object->material;

	// 1. Calculate geometric normal based on object type
	if (hit_object->type == SPHERE)
	{
		t_sphere *sphere = (t_sphere *)(hit_object->data);
		hit_record->original_normal = sphere_normal_at_point(hit_record->point, *sphere);
	}
	else if (hit_object->type == CYLINDER)
	{
		t_cylinder *cylinder = (t_cylinder *)(hit_object->data);
		hit_record->original_normal = cylinder_normal_at_point(hit_record->point, *cylinder);
	}
	else if (hit_object->type == PLANE)
	{
		t_plane *plane = (t_plane *)(hit_object->data);
		hit_record->original_normal = plane->normal;
	}
	else if (hit_object->type == CUBE)
	{
		t_cube *cube = (t_cube *)(hit_object->data);
		hit_record->original_normal = cube_normal_at_point(hit_record->point, *cube);
	}
	else if (hit_object->type == TRIANGLE)
	{
		t_triangle *triangle = (t_triangle *)(hit_object->data);
		hit_record->original_normal = triangle->normal;

		// Use smooth shading if vertex normals are available
		if (triangle->has_vertex_normals)
		{
			// Validate barycentric coordinates before interpolation
			double bary_sum = hit_record->barycentric.x + hit_record->barycentric.y + hit_record->barycentric.z;
			if (fabs(bary_sum - 1.0) < 0.001 &&
				hit_record->barycentric.x >= 0.0 && hit_record->barycentric.x <= 1.0 &&
				hit_record->barycentric.y >= 0.0 && hit_record->barycentric.y <= 1.0 &&
				hit_record->barycentric.z >= 0.0 && hit_record->barycentric.z <= 1.0)
			{
				t_vec3 interpolated = interpolate_vertex_normal(
					hit_record->barycentric, triangle->n0, triangle->n1, triangle->n2);

				// Ensure interpolated normal is valid
				double norm_len = vec3_length(interpolated);
				if (norm_len > 0.1 && norm_len < 10.0)
				{
					hit_record->original_normal = interpolated;
				}
			}
		}

		// For triangles with vertex normals, don't flip the interpolated normal
		// The vertex normals from the OBJ file are already correctly oriented
		if (!triangle->has_vertex_normals)
		{
			// Handle double-sided triangles by flipping normal if needed (only for face normals)
			if (vec3_dot(ray.direction, hit_record->original_normal) > 0)
				hit_record->original_normal = vec3_negate(hit_record->original_normal);
		}
		}
	else if (hit_object->type == CONE)
	{
		t_cone *cone = (t_cone *)(hit_object->data);
		hit_record->original_normal = cone_normal_at_point(hit_record->point, *cone);
	}
	else if (hit_object->type == MESH)
	{
		t_mesh *mesh = (t_mesh *)(hit_object->data);
		int triangle_idx = (int)hit_record->triangle_idx;

		if (triangle_idx >= 0 && triangle_idx < mesh->triangle_count)
		{
			t_transformed_triangle *tri = &mesh->transformed_tris[triangle_idx];
			hit_record->original_normal = tri->normal;

			// Use smooth shading if vertex normals are available
			if (tri->has_vertex_normals)
			{
				// Validate barycentric coordinates before interpolation
				double bary_sum = hit_record->barycentric.x + hit_record->barycentric.y + hit_record->barycentric.z;
				if (fabs(bary_sum - 1.0) < 0.001 &&
					hit_record->barycentric.x >= 0.0 && hit_record->barycentric.x <= 1.0 &&
					hit_record->barycentric.y >= 0.0 && hit_record->barycentric.y <= 1.0 &&
					hit_record->barycentric.z >= 0.0 && hit_record->barycentric.z <= 1.0)
				{
					t_vec3 interpolated = interpolate_vertex_normal(
						hit_record->barycentric, tri->n0, tri->n1, tri->n2);

					// Ensure interpolated normal is valid
					double norm_len = vec3_length(interpolated);
					if (norm_len > 0.1 && norm_len < 10.0)
					{
						hit_record->original_normal = interpolated;
					}
				}
			}

			// For meshes with vertex normals, don't flip the interpolated normal
			// The vertex normals from the OBJ file are already correctly oriented
			if (!tri->has_vertex_normals)
			{
				// Handle double-sided triangles by flipping normal if needed (only for face normals)
				if (vec3_dot(ray.direction, hit_record->original_normal) > 0)
					hit_record->original_normal = vec3_negate(hit_record->original_normal);
			}
		}
		else
		{
			printf("ERROR: triangle_idx %d out of bounds (0-%d)\n", triangle_idx, mesh->triangle_count-1);
			// Fallback normal if index is out of bounds
			hit_record->original_normal = vec3_create(0, 1, 0);
		}
	}
		// 2. Determine if the ray is hitting the front face or inside
	if (vec3_dot(ray.direction, hit_record->original_normal) > 0)
	{
		hit_record->inside = true;
		hit_record->normal = vec3_negate(hit_record->original_normal);
	}
	else
	{
		hit_record->inside = false;
		hit_record->normal = hit_record->original_normal;
	}

	// 3. Optional: UV mapping for textures or checker
	if (hit_record->material.has_texture || hit_record->material.has_checker)
		hit_record->uv = calculate_uv_coordinates(hit_record->point, hit_object);

	// 4. Optional: bump mapping adjustment
	if (hit_record->material.has_bump_map)
		hit_record->normal = calculate_bump_normal(hit_record);
}
