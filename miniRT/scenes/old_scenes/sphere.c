#include "../../includes/miniRT.h"

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