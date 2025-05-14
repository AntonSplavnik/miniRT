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

typedef struct s_light_result {
	double diffuse;
	double specular_intensity;
	double light_distance;
	t_vec3 light_dir;
} t_light_result;

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

void	compute_ray_direction(t_scene *scene, t_ray *ray, double fov_scale, int x, int y)
{
	double	u;
	double	v;
	t_vec3	ray_dir_camera;

	//normalized based on FOV pixel coordinates
	u = (2.0 * x / (double)scene->width - 1.0) * fov_scale;
	v = (1.0 - 2.0 * y / (double)scene->height) * fov_scale;

	//appled aspect ratio correction
	u *= (double)scene->width / scene->height;

	//camera direction
	ray_dir_camera = vec3_create(u, v, 1.0);
	//apply camera rotation
	ray->direction = rotate_point(ray_dir_camera, scene->camera.rotation);
	ray->direction = vec3_normalize(ray->direction);

	//setting ray origin
	ray->origin = scene->camera.position;
}

void	compute_ray_intersaction(t_ray ray, t_object *hit_object, double t, t_vec3 *hit_point, t_vec3 *normal)
{
	//calculate where the ray hit the object
	*hit_point = vec3_add(ray.origin, vec3_scale(ray.direction, t));

	//calculate the normal at the hit point
	if (hit_object->type == SPHERE)
	{
		t_sphere *sphere = (t_sphere *)(hit_object->data);
		*normal = sphere_normal_at_point(*hit_point, *sphere);
	}
	else if (hit_object->type == CYLINDER)
	{
		t_cylinder *cylinder = (t_cylinder *)(hit_object->data);
		*normal = cylinder_normal_at_point(*hit_point, *cylinder);
	}
	else if (hit_object->type == PLANE)
	{
		t_plane *plane = (t_plane *)(hit_object->data);
		*normal = plane->normal;
		//double sided plane
		if (vec3_dot(ray.direction, *normal) > 0)
			*normal = vec3_negate(*normal);
	}
	else if (hit_object->type == CUBE)
	{
		t_cube *cube = (t_cube *)(hit_object->data);
		*normal = cube_normal_at_point(*hit_point, *cube);
	}
	else if (hit_object->type == TRIANGLE)
	{
		t_triangle *triangle = (t_triangle *)(hit_object->data);
		*normal = triangle->normal;
		// Handle double-sided triangles by flipping normal if needed
		if (vec3_dot(ray.direction, *normal) > 0)
			*normal = vec3_negate(*normal);
	}
	else if (hit_object->type == MESH)
	{
		t_mesh *mesh = (t_mesh *)(hit_object->data);
		int triangle_idx = (int)hit_object->material.reflectivity;
		if (triangle_idx >= 0 && triangle_idx < mesh->triangle_count)
		{
			*normal = mesh->triangles[triangle_idx].normal;
			// Handle double-sided triangles by flipping normal if needed
			if (vec3_dot(ray.direction, *normal) > 0)
				*normal = vec3_negate(*normal);
		}
		else
		{
			// Fallback normal if index is out of bounds
			*normal = vec3_create(0, 1, 0);
		}
	}
}

t_light_result compute_light(t_scene *scene, t_object *hit_object, t_vec3 hit_point, t_vec3 normal)
{
	t_light_result result;
	
	// Calculate vector from hit point to light source
	t_vec3 to_light = vec3_subtract(scene->lights->position, hit_point);
	result.light_distance = vec3_length(to_light);

	// Normalize to get light direction
	result.light_dir = vec3_normalize(to_light);

	// Calculate diffuse lighting - dot product of normal and light direction
	result.diffuse = fmax(0.0, vec3_dot(normal, result.light_dir));

	// Adding specular reflection:
	// 1. Calculate the view direction (from hit point to camera)
	// Used to determine if the viewer sees the specular highlight
	t_vec3 view_dir = vec3_normalize(vec3_subtract(scene->camera.position, hit_point));

	// 2. Calculate reflection direction with reflection law calculation: R = L - 2(N.L)N
	t_vec3 reflect_dir = vec3_subtract(vec3_scale(normal, 2.0 * vec3_dot(result.light_dir, normal)), result.light_dir);
	reflect_dir = vec3_normalize(reflect_dir);

	// 3. Calculate specular component
	result.specular_intensity = 0.0;
	if(scene->app.enable_specular)
	{
		double specular = pow(fmax(0.0, vec3_dot(view_dir, reflect_dir)), hit_object->material.shininess);
		result.specular_intensity = hit_object->material.specular * specular;
	}

	return (result);
}

void	render_complex_scene(t_scene *scene)
{
	t_ray		ray;
	int			color;
	t_vec3		hit_point;
	t_vec3		normal;
	double		light_intensity;
	t_light_result light_info;
	t_object	*hit_object;
	int			in_shadow;
	double		t;
	
	in_shadow = 0;

	if (!scene->objects)
		set_up_scene_triangle(scene);

	double fov_scale = tan(scene->camera.fov * M_PI / 360.0);
	
	for (int y = 0; y < scene->height; y++)
	{
		for (int x = 0; x < scene->width; x++)
		{
			compute_ray_direction(scene, &ray, fov_scale, x, y);

			//set background color
			color = (217 << 16 | 185 << 8 | 155); //beige

			
			if (find_closest_intersection(scene, ray, &t, &hit_object))
			{
				compute_ray_intersaction(ray, hit_object, t, &hit_point, &normal);

				light_info = compute_light(scene, hit_object, hit_point, normal);
				
				//Check if the hit point is in shadow
				if(scene->app.checkbox_checked)
					in_shadow = is_in_shadow(scene, hit_point, light_info.light_dir, light_info.light_distance);

				// Combine all lighting components
				if (in_shadow)
					light_intensity = scene->ambient.ratio;
				else
				{
					light_intensity = scene->ambient.ratio +
						(scene->lights->intensity * light_info.diffuse) +
						(scene->lights->intensity * light_info.specular_intensity);
				}

				//Get color from material and apply lighting
				color = get_object_color(hit_object, light_intensity);
			}
			pixel_put(x, y, &scene->img, color);
		}
	}

	//display the image
	draw_image_to_window(scene);

	// Draw the checkbox control
	draw_checkbox(scene);


	display_status(scene);
}
