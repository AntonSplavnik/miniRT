#include "../../includes/miniRT.h"

// Set up a simple scene with a plane for testing
void set_up_scene_plane(t_scene *scene)
{
    // Create a plane
    t_plane *plane = malloc(sizeof(t_plane));
    if (!plane)
        return;
    
    // Set plane properties
    plane->point = vec3_create(0, -1, 0);  // Position at y = -1
    plane->normal = vec3_create(0, 1, 0);  // Normal pointing up
    
    // Create material for the plane
    t_material material = create_material(create_color(200, 200, 200));  // Light gray
    material.specular = 0.1;  // Low specular (mostly diffuse)
    material.shininess = 10.0;
    material.reflectivity = 0.1;  // Slight reflection
    
    // Add checkerboard pattern
    material.has_checker = 1;
    material.checker_size = 1.0;
    material.checker_color = create_color(50, 50, 50);  // Dark gray for contrast
    
    // Create object and add to scene
    t_object *obj = malloc(sizeof(t_object));
    if (!obj)
    {
        free(plane);
        return;
    }
    
    obj->type = PLANE;
    obj->data = plane;
    obj->material = material;
    obj->next = NULL;
    
    // Add to scene
    add_object(scene, obj);
    
    // Create a light source
    t_vec3 light_pos = vec3_create(2, 3, -3);
    t_light *light = create_light(light_pos, 0.8, create_color(255, 255, 255));
    add_light(scene, light);
    
    // Set ambient light
    scene->ambient.ratio = 0.2;
    scene->ambient.color = create_color(255, 255, 255);
    scene->ambient.has_ambient = 1;
    
    // Position camera
    scene->camera.position = vec3_create(0, 1, -5);
    scene->camera.direction = vec3_create(0, 0, 1);
    scene->camera.rotation = vec3_create(0, 0, 0);
    scene->camera.has_camera = 1;
    
    // Compute camera vectors
    scene->camera.forwards = get_forward_vector(scene->camera.rotation);
    scene->camera.right = get_right_vector(scene->camera.rotation);
    scene->camera.up = get_up_vector(scene->camera.rotation);
} 