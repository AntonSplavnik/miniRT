#include "platform.h"

//set up scene with a mesh
void	set_up_scene_mesh(t_scene *scene)
{
	// Create a cube mesh
	t_vec3 cube_center = vec3_create(0.0, 0.0, 2.0);
	double cube_size = 2.0;
	t_color cube_color = create_color(200, 100, 50); // Brownish color
	
	t_object *cube_mesh = create_cube_mesh(cube_center, cube_size, cube_color);
	add_object(scene, cube_mesh);
	
	// Add a sphere
	t_vec3 sphere_center = vec3_create(-2.5, 0.0, 1.5);
	double sphere_diameter = 1.5;
	t_color sphere_color = create_color(50, 100, 200); // Bluish color
	
	t_object *sphere = create_sphere(sphere_center, sphere_diameter, sphere_color);
	add_object(scene, sphere);
	
	// Floor plane
	t_vec3 plane_point = vec3_create(0.0, -1.5, 0.0);
	t_vec3 plane_normal = vec3_create(0.0, 1.0, 0.0);
	t_color floor_color = create_color(200, 200, 200); // Light gray
	
	t_object *floor_plane = create_plane(plane_point, plane_normal, floor_color);
	add_object(scene, floor_plane);
	
	// Camera setup
	scene->camera.position = vec3_create(0.0, 0.0, -4.0);
	scene->camera.rotation = vec3_create(0.0, 0.0, 0.0);
	scene->camera.fov = 60.0;
	
	// Lighting
	scene->ambient.ratio = 0.2;
	scene->ambient.color = create_color(255, 255, 255);
	
	t_vec3 light_pos = vec3_create(5.0, 5.0, -3.0);
	t_color light_color = create_color(255, 255, 255);
	t_light *light = create_light(light_pos, 0.8, light_color);
	add_light(scene, light);
	
	// Material properties
	sphere->material.specular = 0.7;
	sphere->material.shininess = 64.0;
	
	cube_mesh->material.specular = 0.5;
	cube_mesh->material.shininess = 32.0;
}
