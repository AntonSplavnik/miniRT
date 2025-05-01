/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_sphere.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 10:45:02 by abillote          #+#    #+#             */
/*   Updated: 2025/05/01 13:51:17 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "platform.h"


//set up simple scene with one sphere
void	set_up_simple_scene(t_scene *scene)
{
	t_vec3 sphere_center = vec3_create(0.0, 0.0, 2.0);
	double sphere_diameter = 6.0;
	t_color red_color = create_color(255, 0, 0);

	t_object *sphere = create_sphere(sphere_center, sphere_diameter, red_color);
	add_object(scene, sphere);

	scene->camera.position = vec3_create(0.0, 0.0, -5.0);
	scene->camera.orientation = vec3_create(0.0, 0.0, 1.0);
	scene->camera.fov = 90.0;

	scene->ambient.ratio = 0.2;
	scene->ambient.color = create_color(255, 255, 255);
}

t_vec3	sphere_normal_at_point(t_vec3 point, t_sphere sphere)
{
	t_vec3	normal;
	normal = vec3_subtract(point, sphere.center);
	return (vec3_normalize(normal));
}


void	render_simple_scene(t_scene *scene)
{
	t_ray	ray;
	int		color;
	double	t;
	t_vec3	hit_point;
	t_vec3	normal;
	double	light_intensity;

	if (!scene->objects)
		set_up_simple_scene(scene);

	t_sphere *sphere = (t_sphere *)(scene->objects->data);

	for (int y = 0; y < scene->height; y++)
	{
		for (int x = 0; x < scene->width; x++)
		{
			double u = (double)x / scene->width;
			double v = (double)y / scene->height;

			u = 2.0 * u - 1.0;
			v = 1.0 - 2.0 * v;

			u *= (double)scene->width / scene->height;

			ray.origin = scene->camera.position;
			ray.direction = vec3_normalize(vec3_create(u, v, 1.0));

			//set brackground color
			color = 0x87CEEB;

			if (ray_sphere_intersect(ray, *sphere, &t))
			{
				//calculate where the ray hit the sphere
				hit_point = vec3_add(ray.origin, vec3_scale(ray.direction, t));

				//calculate the normal at the hit point
				normal = sphere_normal_at_point(hit_point, *sphere);

				//simple lighting, just based on the normal - to improve with diffuse lighting
				light_intensity = 0.2 + 0.8 * fmax(0.0, normal.z);

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
