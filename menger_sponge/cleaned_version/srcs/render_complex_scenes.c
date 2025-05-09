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
		current = current->next;
	}
	if (hit_something)
		*t = t_closest;
	return (hit_something);
}

//set up scene with a cylinder
void	set_up_scene_cylinder(t_scene *scene)
{
	//Sphere - red
	t_vec3 sphere_red_center = vec3_create(1.5, 0.0, 2.0);
	double sphere_red_diameter = 3.0;
	t_color red_color = create_color(255, 0, 0);

	t_object *sphere_red = create_sphere(sphere_red_center, sphere_red_diameter, red_color);
	add_object(scene, sphere_red);

	//Cylinder - blue
	t_vec3	cylinder_center = vec3_create(-1.5, 0.0, 2.0);
	t_vec3	cylinder_axis = vec3_create(0.0, 1.0, 0.0);
	double cylinder_diameter = 1.5;
	double cylinder_height = 3.0;
	t_color	blue_color = create_color(0, 0, 255);

	t_object *cylinder_blue = create_cylinder(cylinder_center, cylinder_axis, cylinder_diameter, cylinder_height);
	cylinder_blue->material.color = blue_color;
	add_object(scene, cylinder_blue);

	scene->camera.position = vec3_create(0.0, 0.0, -3.0);
	scene->camera.rotation = vec3_create(0.0, 0.0, 0.0);
	scene->camera.fov = 60.0;

	scene->ambient.ratio = 0.2;
	scene->ambient.color = create_color(255, 255, 255);

	t_vec3	light_pos = vec3_create(10.0, 10.0, -10.0);
	t_color	light_color = create_color(255, 255, 255);
	t_light	*light = create_light(light_pos, 0.8, light_color);
	add_light(scene, light);

	sphere_red->material.specular = 0.5;   // High specular reflection
	sphere_red->material.shininess = 32.0; // For a shiny appearance

	cylinder_blue->material.specular = 0.8;   // Higher specular reflection
	cylinder_blue->material.shininess = 64.0; //More shiny
}

//set up scene with a cylinder
void	set_up_scene_plane(t_scene *scene)
{
	//Sphere - red
	t_vec3 sphere_center = vec3_create(1.5, 0.0, 2.0);
	double sphere_diameter = 3.0;
	t_color red_color = create_color(255, 0, 0);

	t_object *sphere_red = create_sphere(sphere_center, sphere_diameter, red_color);
	add_object(scene, sphere_red);

	//Sphere - orange
	sphere_center = vec3_create(0.5, 0.0, 2.0);
	sphere_diameter = 2.0;
	t_color orange_color = create_color(255, 165, 0);

	t_object *sphere_orange = create_sphere(sphere_center, sphere_diameter, orange_color);
	add_object(scene, sphere_orange);

	//Sphere - purple
	sphere_center = vec3_create(0.0, 1.5, 0.5);
	sphere_diameter = 0.3;
	t_color purple_color = create_color(93, 63, 211);

	t_object *sphere_purple = create_sphere(sphere_center, sphere_diameter, purple_color);
	add_object(scene, sphere_purple);

	//Cylinder - blue
	t_vec3	cylinder_center = vec3_create(-1.5, 0.0, 2.0);
	t_vec3	cylinder_axis = vec3_create(0.0, 1.0, 0.0);
	double cylinder_diameter = 1.0;
	double cylinder_height = 2.0;
	t_color	blue_color = create_color(0, 0, 255);

	t_object *cylinder_blue = create_cylinder(cylinder_center, cylinder_axis, cylinder_diameter, cylinder_height);
	cylinder_blue->material.color = blue_color;
	add_object(scene, cylinder_blue);

	//Cylinder - Pink
	cylinder_center = vec3_create(-1.5, 1.0, 1.0);
	cylinder_axis = vec3_create(1.0, 0.0, 0.0);
	cylinder_diameter = 0.5;
	cylinder_height = 3.0;
	t_color	pink_color = create_color(255, 192, 203);

	t_object *cylinder_pink = create_cylinder(cylinder_center, cylinder_axis, cylinder_diameter, cylinder_height);
	cylinder_pink->material.color = pink_color;
	add_object(scene, cylinder_pink);

	//Planes
	t_vec3	plane_point = vec3_create(0.0, -1.5, 0.0);
	t_vec3	plane_normal = vec3_create(0.0, 1.0, 0.0);
	t_color	green_color = create_color(0, 255, 0);

	t_object *floor_plane = create_plane(plane_point, plane_normal, green_color);
	add_object(scene, floor_plane);

	scene->camera.position = vec3_create(0.0, 0.0, -3.0);
	scene->camera.rotation = vec3_create(0.0, 0.0, 0.0);
	scene->camera.fov = 60.0;

	scene->ambient.ratio = 0.2;
	scene->ambient.color = create_color(255, 255, 255);

	t_vec3	light_pos = vec3_create(-10.0, 10.0, -10.0);
	t_color	light_color = create_color(255, 255, 255);
	t_light	*light = create_light(light_pos, 0.8, light_color);
	add_light(scene, light);

	sphere_red->material.specular = 0.5;   // High specular reflection
	sphere_red->material.shininess = 32.0; // For a shiny appearance

	sphere_purple->material.specular = 0.5;   // High specular reflection
	sphere_purple->material.shininess = 64.0; // For a shiny appearance

	cylinder_blue->material.specular = 0.8;   // Higher specular reflection
	cylinder_blue->material.shininess = 64.0; //More shiny
}

//set up scene with two spheres
void	set_up_scene_two_sphere(t_scene *scene)
{
	//First sphere - red
	t_vec3 sphere_red_center = vec3_create(1.5, 0.0, 2.0);
	double sphere_red_diameter = 4.0;
	t_color red_color = create_color(255, 0, 0);

	t_object *sphere_red = create_sphere(sphere_red_center, sphere_red_diameter, red_color);
	add_object(scene, sphere_red);

	//Second sphere - blue
	t_vec3 sphere_blue_center = vec3_create(-1.5, 0.0, 0.0);
	double sphere_blue_diameter = 2.0;
	t_color blue_color = create_color(0, 0, 255);

	t_object *sphere_blue = create_sphere(sphere_blue_center, sphere_blue_diameter, blue_color);
	add_object(scene, sphere_blue);

	scene->camera.position = vec3_create(0.0, 0.0, -5.0);
	scene->camera.rotation = vec3_create(0.0, 0.0, 0.0);
	scene->camera.fov = 50.0;

	scene->ambient.ratio = 0.2;
	scene->ambient.color = create_color(255, 255, 255);

	t_vec3	light_pos = vec3_create(10.0, 10.0, -10.0);
	t_color	light_color = create_color(255, 255, 255);
	t_light	*light = create_light(light_pos, 0.8, light_color);
	add_light(scene, light);

	sphere_red->material.specular = 0.5;   // High specular reflection
	sphere_red->material.shininess = 32.0; // For a shiny appearance

	sphere_blue->material.specular = 0.8;   // Higher specular reflection
	sphere_blue->material.shininess = 64.0; //More shiny
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
		set_up_scene_plane(scene);

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
