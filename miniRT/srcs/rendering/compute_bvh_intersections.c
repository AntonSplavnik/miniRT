#include "../../includes/miniRT.h"


t_aabb calculate_object_aabb(t_object *object)
{
    t_aabb bounds = {{0, 0, 0}, {0, 0, 0}}; // Initialize bounds to avoid uninitialized warning
    
    if (object->type == SPHERE)
    {
        t_sphere *sphere = (t_sphere *)object->data;
        double radius = sphere->radius;
        bounds.min = vec3_subtract(sphere->center, vec3_create(radius, radius, radius));
        bounds.max = vec3_add(sphere->center, vec3_create(radius, radius, radius));
    }
    else if (object->type == PLANE)
    {
        // Planes are infinite, so use a large but finite box
        // Centered at the plane's point and extending in all directions
        t_plane *plane = (t_plane *)object->data;
        double size = 1000.0;  // Large enough for most scenes
        bounds.min = vec3_subtract(plane->point, vec3_create(size, size, size));
        bounds.max = vec3_add(plane->point, vec3_create(size, size, size));
    }
    else if (object->type == CYLINDER)
    {
        t_cylinder *cylinder = (t_cylinder *)object->data;
        double radius = cylinder->radius;
        double half_height = cylinder->height / 2.0;
        t_vec3 axis_scaled = vec3_scale(cylinder->axis, half_height);
        t_vec3 top = vec3_add(cylinder->center, axis_scaled);
        t_vec3 bottom = vec3_subtract(cylinder->center, axis_scaled);
        
        // Create bounds that encompass both caps and the cylinder body
        bounds.min.x = fmin(top.x - radius, bottom.x - radius);
        bounds.min.y = fmin(top.y - radius, bottom.y - radius);
        bounds.min.z = fmin(top.z - radius, bottom.z - radius);
        
        bounds.max.x = fmax(top.x + radius, bottom.x + radius);
        bounds.max.y = fmax(top.y + radius, bottom.y + radius);
        bounds.max.z = fmax(top.z + radius, bottom.z + radius);
    }
    else if (object->type == CUBE)
    {
        t_cube *cube = (t_cube *)object->data;
        double half_side = cube->side_length / 2.0;
        bounds.min = vec3_subtract(cube->center, vec3_create(half_side, half_side, half_side));
        bounds.max = vec3_add(cube->center, vec3_create(half_side, half_side, half_side));
    }
    else if (object->type == CONE)
    {
        t_cone *cone = (t_cone *)object->data;
        double radius = cone->radius;
        double height = cone->height;
        
        // For a cone, we need to consider the base circle and the apex
        t_vec3 apex = cone->apex;
        t_vec3 axis_scaled = vec3_scale(cone->axis, height);
        t_vec3 base_center = vec3_subtract(apex, axis_scaled);
        
        // Create bounds that encompass both the base and the apex
        bounds.min.x = fmin(apex.x, base_center.x - radius);
        bounds.min.y = fmin(apex.y, base_center.y - radius);
        bounds.min.z = fmin(apex.z, base_center.z - radius);
        
        bounds.max.x = fmax(apex.x, base_center.x + radius);
        bounds.max.y = fmax(apex.y, base_center.y + radius);
        bounds.max.z = fmax(apex.z, base_center.z + radius);
    } 
    else if (object->type == TRIANGLE)
    {
        t_triangle *triangle = (t_triangle *)object->data;
        
        // Find min and max coordinates across all vertices
        bounds.min.x = fmin(fmin(triangle->v0.x, triangle->v1.x), triangle->v2.x);
        bounds.min.y = fmin(fmin(triangle->v0.y, triangle->v1.y), triangle->v2.y);
        bounds.min.z = fmin(fmin(triangle->v0.z, triangle->v1.z), triangle->v2.z);
        
        bounds.max.x = fmax(fmax(triangle->v0.x, triangle->v1.x), triangle->v2.x);
        bounds.max.y = fmax(fmax(triangle->v0.y, triangle->v1.y), triangle->v2.y);
        bounds.max.z = fmax(fmax(triangle->v0.z, triangle->v1.z), triangle->v2.z);
        
        // Add a small epsilon to avoid zero-volume boxes for flat triangles
        double epsilon = 0.0001;
        if (bounds.max.x - bounds.min.x < epsilon)
        {
            bounds.min.x -= epsilon;
            bounds.max.x += epsilon;
        }
        if (bounds.max.y - bounds.min.y < epsilon)
        {
            bounds.min.y -= epsilon;
            bounds.max.y += epsilon;
        }
        if (bounds.max.z - bounds.min.z < epsilon)
        {
            bounds.min.z -= epsilon;
            bounds.max.z += epsilon;
        }
    }
    else if (object->type == MESH)
    {
        t_mesh *mesh = (t_mesh *)object->data;
        
        // Initialize with extreme values
        bounds.min = vec3_create(INFINITY, INFINITY, INFINITY);
        bounds.max = vec3_create(-INFINITY, -INFINITY, -INFINITY);
        
        // Iterate through all triangles to find the bounding box
        for (int i = 0; i < mesh->triangle_count; i++)
        {
            t_triangle *tri = &mesh->triangles[i];
            
            // Update min bounds
            bounds.min.x = fmin(bounds.min.x, fmin(fmin(tri->v0.x, tri->v1.x), tri->v2.x));
            bounds.min.y = fmin(bounds.min.y, fmin(fmin(tri->v0.y, tri->v1.y), tri->v2.y));
            bounds.min.z = fmin(bounds.min.z, fmin(fmin(tri->v0.z, tri->v1.z), tri->v2.z));
            
            // Update max bounds
            bounds.max.x = fmax(bounds.max.x, fmax(fmax(tri->v0.x, tri->v1.x), tri->v2.x));
            bounds.max.y = fmax(bounds.max.y, fmax(fmax(tri->v0.y, tri->v1.y), tri->v2.y));
            bounds.max.z = fmax(bounds.max.z, fmax(fmax(tri->v0.z, tri->v1.z), tri->v2.z));
        }
        
        // Apply mesh transformation (position, rotation, scale)
        // Note: This is a simplified approach; for accurate transformed bounds,
        // you should transform all vertices first and then compute the AABB
        t_vec3 position = mesh->position;
        t_vec3 scale = mesh->scale;
        
        // Adjust for position and scale
        bounds.min.x = position.x + bounds.min.x * scale.x;
        bounds.min.y = position.y + bounds.min.y * scale.y;
        bounds.min.z = position.z + bounds.min.z * scale.z;
        
        bounds.max.x = position.x + bounds.max.x * scale.x;
        bounds.max.y = position.y + bounds.max.y * scale.y;
        bounds.max.z = position.z + bounds.max.z * scale.z;
        
        // Ensure min is actually less than max after transformation
        if (scale.x < 0)
        {
            double temp = bounds.min.x;
            bounds.min.x = bounds.max.x;
            bounds.max.x = temp;
        }
        if (scale.y < 0)
        {
            double temp = bounds.min.y;
            bounds.min.y = bounds.max.y;
            bounds.max.y = temp;
        }
        if (scale.z < 0)
        {
            double temp = bounds.min.z;
            bounds.min.z = bounds.max.z;
            bounds.max.z = temp;
        }
    }
   
    return bounds;
}

// Recursive function to build a BVH for scene objects
t_bvh_node *build_scene_bvh_recursive(t_object **objects, t_aabb *bounds, 
                                    int start, int end, int depth)
{
    if (start > end)
        return NULL;

    // Create a new node
    t_bvh_node *node = malloc(sizeof(t_bvh_node));
    if (!node)
        return NULL;

    // Initialize node
    node->left = NULL;
    node->right = NULL;
    node->is_leaf = 0;
    node->iteration = 0; // Not used for scene BVH
    node->object_ref = NULL; // Initialize object reference to NULL
    
    // If there's only one object, make it a leaf node
    if (start == end)
    {
        node->is_leaf = 1;
        // Store object reference directly in the node
        node->object_ref = objects[start];
        node->bounds = bounds[start];
        return node;
    }

    // Compute the bounding box of all objects
    t_aabb total_bounds = bounds[start];
    for (int i = start + 1; i <= end; i++)
    {
        // Expand bounds to include this object
        total_bounds.min.x = fmin(total_bounds.min.x, bounds[i].min.x);
        total_bounds.min.y = fmin(total_bounds.min.y, bounds[i].min.y);
        total_bounds.min.z = fmin(total_bounds.min.z, bounds[i].min.z);
        
        total_bounds.max.x = fmax(total_bounds.max.x, bounds[i].max.x);
        total_bounds.max.y = fmax(total_bounds.max.y, bounds[i].max.y);
        total_bounds.max.z = fmax(total_bounds.max.z, bounds[i].max.z);
    }
    
    node->bounds = total_bounds;
    
    // Find the axis with the largest extent
    double x_extent = total_bounds.max.x - total_bounds.min.x;
    double y_extent = total_bounds.max.y - total_bounds.min.y;
    double z_extent = total_bounds.max.z - total_bounds.min.z;
    
    int axis = 0; // 0 = x, 1 = y, 2 = z
    if (y_extent > x_extent && y_extent > z_extent)
        axis = 1;
    else if (z_extent > x_extent && z_extent > y_extent)
        axis = 2;
    
    // Sort objects along the chosen axis
    // Using a simple bubble sort for clarity - for production code, use a faster sort
    for (int i = start; i <= end; i++)
    {
        for (int j = i + 1; j <= end; j++)
        {
            double centroid_i, centroid_j;
            
            // Calculate centroids along the chosen axis
            if (axis == 0)
            {
                centroid_i = (bounds[i].min.x + bounds[i].max.x) * 0.5;
                centroid_j = (bounds[j].min.x + bounds[j].max.x) * 0.5;
            }
            else if (axis == 1)
            {
                centroid_i = (bounds[i].min.y + bounds[i].max.y) * 0.5;
                centroid_j = (bounds[j].min.y + bounds[j].max.y) * 0.5;
            }
            else
            {
                centroid_i = (bounds[i].min.z + bounds[i].max.z) * 0.5;
                centroid_j = (bounds[j].min.z + bounds[j].max.z) * 0.5;
            }
            
            // Swap if needed
            if (centroid_i > centroid_j)
            {
                // Swap objects
                t_object *temp_obj = objects[i];
                objects[i] = objects[j];
                objects[j] = temp_obj;
                
                // Swap bounds
                t_aabb temp_bounds = bounds[i];
                bounds[i] = bounds[j];
                bounds[j] = temp_bounds;
            }
        }
    }
    
    // Find the middle index to split at
    int mid = start + (end - start) / 2;
    
    // Recursively build left and right subtrees
    node->left = build_scene_bvh_recursive(objects, bounds, start, mid, depth + 1);
    node->right = build_scene_bvh_recursive(objects, bounds, mid + 1, end, depth + 1);
    
    return node;
}

// Build BVH for entire scene
t_bvh_node *build_scene_bvh(t_scene *scene)
{
    int object_count = 0;
    t_object *current = scene->objects;
    
    // Count objects
    while (current)
    {
        object_count++;
        current = current->next;
    }
    
    if (object_count == 0)
        return NULL;
    
    // Create array of objects and their AABBs
    t_object **objects = malloc(sizeof(t_object *) * object_count);
    t_aabb *bounds = malloc(sizeof(t_aabb) * object_count);
    
    // Fill arrays
    current = scene->objects;
    for (int i = 0; i < object_count; i++)
    {
        objects[i] = current;
        bounds[i] = calculate_object_aabb(current);
        current = current->next;
    }
    
    // Build BVH (recursive function)
    t_bvh_node *root = build_scene_bvh_recursive(objects, bounds, 0, object_count - 1, 0);
    
    // Clean up
    free(objects);
    free(bounds);
    
    return root;
}

int scene_ray_intersect_bvh(t_scene *scene, t_ray ray, double *t, t_object **hit_object, t_hit_record *hit_record)
{
    if (!scene->scene_bvh)
        return find_closest_intersection(scene, ray, t, hit_object, hit_record);
    
    // Queue for traversal (avoiding recursion for better performance)
    t_bvh_node *stack[64];
    int stack_size = 0;
    
    stack[stack_size++] = scene->scene_bvh;
    
    double closest_t = INFINITY;
    int hit_something = 0;
    
    while (stack_size > 0)
    {
        t_bvh_node *node = stack[--stack_size];
        
        double node_tmin, node_tmax;
        if (!ray_intersect_aabb_scalar(node->bounds, ray.origin, ray.direction, &node_tmin, &node_tmax))
            continue;
        
        if (node_tmin > closest_t)
            continue;
        
        if (node->is_leaf && node->object_ref)
        {
            // Direct access to the object through object_ref
            t_object *obj = (t_object *)node->object_ref;
            double t_temp;
            int triangle_idx;
            
            // Test intersection with actual object
            if (obj->type == SPHERE)
            {
                if (ray_sphere_intersect(ray, *(t_sphere *)obj->data, &t_temp) && t_temp < closest_t)
                {
                    closest_t = t_temp;
                    *hit_object = obj;
                    hit_something = 1;
                }
            }
            else if (obj->type == PLANE)
            {
                if (ray_plane_intersect(ray, *(t_plane *)obj->data, &t_temp) && t_temp < closest_t)
                {
                    closest_t = t_temp;
                    *hit_object = obj;
                    hit_something = 1;
                }
            }
            else if (obj->type == CYLINDER)
            {
                if (ray_cylinder_intersect(ray, *(t_cylinder *)obj->data, &t_temp) && t_temp < closest_t)
                {
                    closest_t = t_temp;
                    *hit_object = obj;
                    hit_something = 1;
                }
            }
            else if (obj->type == CUBE)
            {
                if (ray_cube_intersect(ray, *(t_cube *)obj->data, &t_temp) && t_temp < closest_t)
                {
                    closest_t = t_temp;
                    *hit_object = obj;
                    hit_something = 1;
                }
            }
            else if (obj->type == TRIANGLE)
            {
                if (ray_triangle_intersect(ray, *(t_triangle *)obj->data, &t_temp) && t_temp < closest_t)
                {
                    closest_t = t_temp;
                    *hit_object = obj;
                    hit_something = 1;
                }
            }
            else if (obj->type == MESH)
            {
                if (ray_mesh_intersect(ray, *(t_mesh *)obj->data, &t_temp, &triangle_idx) && t_temp < closest_t)
                {
                    closest_t = t_temp;
                    *hit_object = obj;
                    hit_something = 1;
                    hit_record->triangle_idx = triangle_idx;
                }
            }
            else if (obj->type == CONE)
            {
                if (ray_cone_intersect(ray, *(t_cone *)obj->data, &t_temp) && t_temp < closest_t)
                {
                    closest_t = t_temp;
                    *hit_object = obj;
                    hit_something = 1;
                }
            }
        }
        else
        {
            // Internal node - traverse children
            if (node->right)
                stack[stack_size++] = node->right;
            if (node->left)
                stack[stack_size++] = node->left;
        }
    }
    
    if (hit_something)
        *t = closest_t;
    
    return hit_something;
}

// Ray-AABB intersection test
int ray_intersect_aabb_scalar(t_aabb bounds, t_vec3 ray_origin, t_vec3 ray_dir,
                      double *t_min, double *t_max)
{
    // Initialize to extreme values
    double t_near = -INFINITY;
    double t_far = INFINITY;

    // Epsilon for handling near-zero ray directions
    double epsilon = 1e-6;

    // Check each dimension (x, y, z)
    // X dimension check
    if (fabs(ray_dir.x) < epsilon) {
        // Ray is parallel to the slab, check if ray origin is within slab
        if (ray_origin.x < bounds.min.x || ray_origin.x > bounds.max.x)
            return 0;
    } else {
        // Compute intersection distances
        double inv_dir = 1.0 / ray_dir.x;
        double t1 = (bounds.min.x - ray_origin.x) * inv_dir;
        double t2 = (bounds.max.x - ray_origin.x) * inv_dir;

        // Ensure t1 is the near intersection and t2 is the far one
        if (t1 > t2) {
            double temp = t1;
            t1 = t2;
            t2 = temp;
        }

        // Update the min/max intersection distances
        if (t1 > t_near) t_near = t1;
        if (t2 < t_far) t_far = t2;

        // Early rejection test
        if (t_near > t_far || t_far < 0)
            return 0;
    }

    // Y dimension check
    if (fabs(ray_dir.y) < epsilon) {
        if (ray_origin.y < bounds.min.y || ray_origin.y > bounds.max.y)
            return 0;
    } else {
        double inv_dir = 1.0 / ray_dir.y;
        double t1 = (bounds.min.y - ray_origin.y) * inv_dir;
        double t2 = (bounds.max.y - ray_origin.y) * inv_dir;

        if (t1 > t2) {
            double temp = t1;
            t1 = t2;
            t2 = temp;
        }

        if (t1 > t_near) t_near = t1;
        if (t2 < t_far) t_far = t2;

        if (t_near > t_far || t_far < 0)
            return 0;
    }

    // Z dimension check
    if (fabs(ray_dir.z) < epsilon) {
        if (ray_origin.z < bounds.min.z || ray_origin.z > bounds.max.z)
            return 0;
    } else {
        double inv_dir = 1.0 / ray_dir.z;
        double t1 = (bounds.min.z - ray_origin.z) * inv_dir;
        double t2 = (bounds.max.z - ray_origin.z) * inv_dir;

        if (t1 > t2) {
            double temp = t1;
            t1 = t2;
            t2 = temp;
        }

        if (t1 > t_near) t_near = t1;
        if (t2 < t_far) t_far = t2;

        if (t_near > t_far || t_far < 0)
            return 0;
    }

    // If we got here, there's a valid intersection
    *t_min = t_near;
    *t_max = t_far;

    return 1;
}