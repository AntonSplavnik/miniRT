#include "../../includes/miniRT.h"

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