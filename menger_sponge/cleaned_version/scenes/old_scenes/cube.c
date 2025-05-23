#include "miniRT.h"

//set up scene with a cube
void	set_up_scene_cube(t_scene *scene)
{
	//Cube - red
	t_vec3 cube_center = vec3_create(1.5, 0.0, 2.0);
	double cube_side_length = 2.0;
	t_color red_color = create_color(255, 0, 0);

	t_object *cube_red = create_cube(cube_center, cube_side_length, red_color);
	add_object(scene, cube_red);

	//Sphere - blue
	t_vec3 sphere_center = vec3_create(-1.5, 0.0, 2.0);
	double sphere_diameter = 1.5;
	t_color blue_color = create_color(0, 0, 255);

	t_object *sphere_blue = create_sphere(sphere_center, sphere_diameter, blue_color);
	add_object(scene, sphere_blue);

	//Floor plane
	t_vec3	plane_point = vec3_create(0.0, -2.0, 0.0);
	t_vec3	plane_normal = vec3_create(0.0, 1.0, 0.0);
	t_color	green_color = create_color(0, 255, 0);

	t_object *floor_plane = create_plane(plane_point, plane_normal, green_color);
	add_object(scene, floor_plane);

	//Set up camera
	scene->camera.position = vec3_create(0.0, 0.0, -3.0);
	scene->camera.rotation = vec3_create(0.0, 0.0, 0.0);
	scene->camera.fov = 60.0;

	//Set up ambient lighting
	scene->ambient.ratio = 0.2;
	scene->ambient.color = create_color(255, 255, 255);

	//Add light source
	t_vec3	light_pos = vec3_create(5.0, 5.0, -5.0);
	t_color	light_color = create_color(255, 255, 255);
	t_light	*light = create_light(light_pos, 0.8, light_color);
	add_light(scene, light);

	//Set material properties
	cube_red->material.specular = 0.5;
	cube_red->material.shininess = 32.0;

	sphere_blue->material.specular = 0.8;
	sphere_blue->material.shininess = 64.0;
}