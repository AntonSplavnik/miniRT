/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_complex_scene.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 10:45:02 by abillote          #+#    #+#             */
/*   Updated: 2025/05/05 10:28:58 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "platform.h"

int	find_closest_intersection(t_scene *scene, t_ray ray, double *t, t_object **hit_object)
{
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
			if (ray_triangle_intersect(ray, *triangle, &t_temp) && t_temp < t_closest)
			{
				t_closest = t_temp;
				hit_something = 1;
				*hit_object = current;
			}
		}
		else if (current->type == MESH)
		{
			t_mesh *mesh = (t_mesh *)(current->data);
			if (ray_mesh_intersect(ray, *mesh, &t_temp, &triangle_idx) && t_temp < t_closest)
			{
				t_closest = t_temp;
				hit_something = 1;
				*hit_object = current;
				// Store the triangle index in the hit object for later normal computation
				// Note: This is a hack - we're assuming the user won't mess with the pointer
				// In a real implementation, this should be stored in a hit record structure
				current->material.reflectivity = triangle_idx;
			}
		}
		current = current->next;
	}
	if (hit_something)
		*t = t_closest;
	return (hit_something);
}

void	render_complex_scene(t_scene *scene)
{
	t_ray		ray;
	int			color;
	double		t;
	t_vec3		hit_point;
	t_vec3		normal;
	double		light_intensity;
	t_vec3		light_dir;
	t_object	*hit_object;

	if (!scene->objects)
		set_up_scene_mesh(scene);  // Use the mesh scene by default

	double fov_scale = tan(scene->camera.fov * M_PI / 360.0);
	for (int y = 0; y < scene->height; y++)
	{
		for (int x = 0; x < scene->width; x++)
		{
			double u = (2.0 * x / (double)scene->width - 1.0) * fov_scale;
			double v = (1.0 - 2.0 * y / (double)scene->height) * fov_scale;

			u *= (double)scene->width / scene->height;

			t_vec3 ray_dir_camera = vec3_normalize(vec3_create(u, v, 1.0));
			ray.direction = rotate_point(ray_dir_camera, scene->camera.rotation);
			ray.direction = vec3_normalize(ray.direction);

			ray.origin = scene->camera.position;

			//set brackground color
			color = (217 << 16 | 185 << 8 | 155); //beige

			if (find_closest_intersection(scene, ray, &t, &hit_object))
			{
				//calculate where the ray hit the sphere
				hit_point = vec3_add(ray.origin, vec3_scale(ray.direction, t));

				//calculate the normal at the hit point
				if (hit_object->type == SPHERE)
				{
					t_sphere *sphere = (t_sphere *)(hit_object->data);
					normal = sphere_normal_at_point(hit_point, *sphere);
				}
				else if (hit_object->type == CYLINDER)
				{
					t_cylinder *cylinder = (t_cylinder *)(hit_object->data);
					normal = cylinder_normal_at_point(hit_point, *cylinder);
				}
				else if (hit_object->type == PLANE)
				{
					t_plane *plane = (t_plane *)(hit_object->data);
					normal = plane->normal;
					//double sided plane
					if (vec3_dot(ray.direction, normal) > 0)
						normal = vec3_negate(normal);
				}
				else if (hit_object->type == CUBE)
				{
					t_cube *cube = (t_cube *)(hit_object->data);
					normal = cube_normal_at_point(hit_point, *cube);
				}
				else if (hit_object->type == TRIANGLE)
				{
					t_triangle *triangle = (t_triangle *)(hit_object->data);
					normal = triangle->normal;
					// Handle double-sided triangles by flipping normal if needed
					if (vec3_dot(ray.direction, normal) > 0)
						normal = vec3_negate(normal);
				}
				else if (hit_object->type == MESH)
				{
					t_mesh *mesh = (t_mesh *)(hit_object->data);
					int triangle_idx = (int)hit_object->material.reflectivity;
					if (triangle_idx >= 0 && triangle_idx < mesh->triangle_count)
					{
						normal = mesh->triangles[triangle_idx].normal;
						// Handle double-sided triangles by flipping normal if needed
						if (vec3_dot(ray.direction, normal) > 0)
							normal = vec3_negate(normal);
					}
					else
					{
						// Fallback normal if index is out of bounds
						normal = vec3_create(0, 1, 0);
					}
				}

				// Calculate vector from hit point to light source
				t_vec3 to_light = vec3_subtract(scene->lights->position, hit_point);
				double light_distance = vec3_length(to_light);

				//Normalize to get light direction
				light_dir = vec3_normalize(to_light);

				// Calculate diffuse lighting - dot product of normal and light direction
				double diffuse = fmax(0.0, vec3_dot(normal, light_dir));

				//Adding specular reflection:
				//1. Calculate the view direction (from hit point to camera)
				//Used to determine if the viewers sees the specular highlight
				t_vec3 view_dir = vec3_normalize(vec3_subtract(scene->camera.position, hit_point));

				//2. Calculate reflection direction with reflection law calculation: R = L - 2(N.L)N
				t_vec3 reflect_dir = vec3_subtract(vec3_scale(normal, 2.0 * vec3_dot(light_dir, normal)), light_dir);
				reflect_dir = vec3_normalize(reflect_dir);

				//3. Calculate specular component
				double specular = pow(fmax(0.0, vec3_dot(view_dir, reflect_dir)), hit_object->material.shininess);
				double specular_intensity = hit_object->material.specular * specular;

				//Check if the hit point is in shadow
				int in_shadow = is_in_shadow(scene, hit_point, light_dir, light_distance);

				// Combine all lighting components
				if (in_shadow)
					light_intensity = scene->ambient.ratio;
				else
				{
					light_intensity = scene->ambient.ratio +
				(scene->lights->intensity * diffuse) +
				(scene->lights->intensity * specular_intensity);
				}

				//Get color from material and apply lighting
				color = get_object_color(hit_object, light_intensity);
			}
			pixel_put(x, y, &scene->img, color);
		}
	}

	//display the image
	draw_image_to_window(scene);

	display_status(scene);
}
