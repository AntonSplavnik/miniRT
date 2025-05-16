#include "platform.h"

//set up scene with triangles
void	set_up_scene_triangle(t_scene *scene)
{
	// Create a simple pyramid with triangles
	t_vec3 base_center = vec3_create(0.0, -1.0, 2.0);
	t_vec3 apex = vec3_create(0.0, 1.0, 2.0);
	
	// Base vertices (square base)
	t_vec3 v0 = vec3_create(base_center.x - 1.0, base_center.y, base_center.z - 1.0);
	t_vec3 v1 = vec3_create(base_center.x + 1.0, base_center.y, base_center.z - 1.0);
	t_vec3 v2 = vec3_create(base_center.x + 1.0, base_center.y, base_center.z + 1.0);
	t_vec3 v3 = vec3_create(base_center.x - 1.0, base_center.y, base_center.z + 1.0);
	
	// Create triangle colors
	t_color red_color = create_color(255, 0, 0);
	t_color green_color = create_color(0, 255, 0);
	t_color blue_color = create_color(0, 0, 255);
	t_color yellow_color = create_color(255, 255, 0);
	t_color purple_color = create_color(128, 0, 128);
	
	// Create the pyramid's triangles
	
	// Base (two triangles to form a square)
	t_object *base_tri1 = create_triangle(v0, v1, v2, purple_color);
	add_object(scene, base_tri1);
	
	t_object *base_tri2 = create_triangle(v0, v2, v3, purple_color);
	add_object(scene, base_tri2);
	
	// Side faces
	t_object *side_tri1 = create_triangle(v0, v1, apex, red_color);
	add_object(scene, side_tri1);
	
	t_object *side_tri2 = create_triangle(v1, v2, apex, green_color);
	add_object(scene, side_tri2);
	
	t_object *side_tri3 = create_triangle(v2, v3, apex, blue_color);
	add_object(scene, side_tri3);
	
	t_object *side_tri4 = create_triangle(v3, v0, apex, yellow_color);
	add_object(scene, side_tri4);
	
	// Add a sphere
	t_vec3 sphere_center = vec3_create(-2.5, 0.0, 1.0);
	double sphere_diameter = 1.5;
	t_color sphere_color = create_color(255, 165, 0); // Orange
	
	t_object *sphere = create_sphere(sphere_center, sphere_diameter, sphere_color);
	add_object(scene, sphere);
	
	// Floor plane
	t_vec3 plane_point = vec3_create(0.0, -1.0, 0.0);
	t_vec3 plane_normal = vec3_create(0.0, 1.0, 0.0);
	t_color floor_color = create_color(200, 200, 200); // Light gray
	
	t_object *floor_plane = create_plane(plane_point, plane_normal, floor_color);
	add_object(scene, floor_plane);
	
	// Camera setup
	scene->camera.position = vec3_create(0.0, 0.5, -4.0);
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
	
	side_tri1->material.specular = 0.5;
	side_tri1->material.shininess = 32.0;
	
	side_tri2->material.specular = 0.5;
	side_tri2->material.shininess = 32.0;
	
	side_tri3->material.specular = 0.5;
	side_tri3->material.shininess = 32.0;
	
	side_tri4->material.specular = 0.5;
	side_tri4->material.shininess = 32.0;
}