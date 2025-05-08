/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_sphere.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 10:45:02 by abillote          #+#    #+#             */
/*   Updated: 2025/05/05 10:34:37 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "platform.h"

//set up simple scene with one sphere
void	set_up_simple_scene(t_scene *scene)
{
	t_vec3 sphere_center = vec3_create(0.0, 0.0, 2.0);
	double sphere_diameter = 4.0;
	t_color red_color = create_color(255, 0, 0);

	t_object *sphere = create_sphere(sphere_center, sphere_diameter, red_color);
	add_object(scene, sphere);

	scene->camera.position = vec3_create(0.0, 0.0, -5.0);
	scene->camera.rotation = vec3_create(0.0, 0.0, 0.0);
	scene->camera.fov = 50.0;

	scene->ambient.ratio = 0.2;
	scene->ambient.color = create_color(255, 255, 255);

	t_vec3	light_pos = vec3_create(10.0, 10.0, -10.0);
	t_color	light_color = create_color(255, 255, 255);
	t_light	*light = create_light(light_pos, 0.8, light_color);
	add_light(scene, light);

	sphere->material.specular = 0.5;   // High specular reflection
	sphere->material.shininess = 32.0; // For a shiny appearance
}

void	render_simple_scene(t_scene *scene)
{
	t_ray	ray;
	int		color;
	double	t;
	t_vec3	hit_point;
	t_vec3	normal;
	double	light_intensity;
	t_vec3	light_dir;

	if (!scene->objects)
		set_up_scene_two_sphere(scene);

	t_sphere *sphere = (t_sphere *)(scene->objects->data);

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

			if (ray_sphere_intersect(ray, *sphere, &t))
			{
				//calculate where the ray hit the sphere
				hit_point = vec3_add(ray.origin, vec3_scale(ray.direction, t));

				//calculate the normal at the hit point
				normal = sphere_normal_at_point(hit_point, *sphere);

				// Calculate light direction from hit point to light source
				light_dir = vec3_normalize(vec3_subtract(scene->lights->position, hit_point));

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
				double specular = pow(fmax(0.0, vec3_dot(view_dir, reflect_dir)), scene->objects->material.shininess);
				double specular_intensity = scene->objects->material.specular * specular;

				// Combine all lighting components
				light_intensity = scene->ambient.ratio +
				(scene->lights->intensity * diffuse) +
				(scene->lights->intensity * specular_intensity);

				//Get color from material and apply lighting
				color = get_final_color(scene, light_intensity);
			}
			pixel_put(x, y, &scene->img, color);
		}
	}

	//display the image
	draw_image_to_window(scene);

	display_status(scene);
}
