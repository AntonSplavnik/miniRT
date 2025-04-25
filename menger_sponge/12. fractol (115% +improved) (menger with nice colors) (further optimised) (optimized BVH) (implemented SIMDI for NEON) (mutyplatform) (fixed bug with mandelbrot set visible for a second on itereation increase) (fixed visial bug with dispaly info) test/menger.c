/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   menger.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asplavni <asplavni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/12 15:30:00 by asplavni          #+#    #+#             */
/*   Updated: 2024/04/12 15:30:00 by asplavni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "platform.h"
#include <math.h>
#include <stdlib.h>
#include <pthread.h>

// SIMD detection for ARM
#if defined(__ARM_NEON__)
#include <arm_neon.h>
#define HAS_NEON 1
#elif defined(__ARM_NEON)
#include <arm_neon.h>
#define HAS_NEON 1
#else
#define HAS_NEON 0
#endif

// Only use SIMD on platforms where it's available
#if HAS_NEON
#define ray_intersect_aabb ray_intersect_aabb_simd
#else
#define ray_intersect_aabb ray_intersect_aabb_scalar
#endif

// Add this structure near the top of the file, after the includes
typedef struct s_menger_thread_data
{
    t_fractal   *fractal;
    int         start_y;
    int         end_y;
} t_menger_thread_data;

// Function to create and initialize a new BVH node
static t_bvh_node *create_bvh_node(t_aabb bounds, int is_leaf, int iteration)
{
    t_bvh_node *node = (t_bvh_node *)malloc(sizeof(t_bvh_node));
    if (!node)
        return (NULL);
    
    node->bounds = bounds;
    node->left = NULL;
    node->right = NULL;
    node->is_leaf = is_leaf;
    node->iteration = iteration;
    
    return (node);
}

// Recursive function to build a BVH for the Menger sponge
t_bvh_node *build_menger_bvh_recursive(t_aabb bounds, int current_iter, int max_iter)
{
    // Base case: if we've reached the maximum iterations or we're at depth 0
    if (current_iter <= 0)
    {
        return create_bvh_node(bounds, 1, current_iter);
    }
    
    t_bvh_node *node = create_bvh_node(bounds, 0, current_iter);
    if (!node)
        return (NULL);
    
    // Calculate dimensions of the current box
    double width = bounds.max.x - bounds.min.x;
    
    // Size of sub-cubes
    double sub_size = width / 3.0;
    
    // Store potential children before adding to the binary tree
    t_bvh_node *children[20]; // Max 20 children (3³ - 7 = 20)
    int child_count = 0;
    
    // Process all 27 potential sub-cubes (3x3x3 grid)
    for (int x = 0; x < 3; x++)
    {
        for (int y = 0; y < 3; y++)
        {
            for (int z = 0; z < 3; z++)
            {
                // Skip the "holes" in the Menger sponge
                // Skip center of cube and centers of all faces
                if ((x == 1 && y == 1) ||  // Center of X-Y face
                    (x == 1 && z == 1) ||  // Center of X-Z face 
                    (y == 1 && z == 1))    // Center of Y-Z face
                    continue;
                
                // Calculate bounds for this sub-cube
                t_aabb sub_bounds;
                sub_bounds.min.x = bounds.min.x + x * sub_size;
                sub_bounds.min.y = bounds.min.y + y * sub_size;
                sub_bounds.min.z = bounds.min.z + z * sub_size;
                
                sub_bounds.max.x = sub_bounds.min.x + sub_size;
                sub_bounds.max.y = sub_bounds.min.y + sub_size;
                sub_bounds.max.z = sub_bounds.min.z + sub_size;
                
                // Recursively build a BVH for this sub-cube (with decreased iteration)
                t_bvh_node *child = build_menger_bvh_recursive(sub_bounds, current_iter - 1, max_iter);
                
                if (child)
                {
                    // Store this child for later binary tree construction
                    children[child_count++] = child;
                }
            }
        }
    }
    
    // Now build a balanced binary tree from the collected children
    if (child_count > 0)
    {
        // For simplicity, we'll use a naive approach to build a roughly balanced tree
        // Repeatedly merge pairs of nodes until we have a single root
        while (child_count > 1)
        {
            int new_count = 0;
            for (int i = 0; i < child_count; i += 2)
            {
                if (i + 1 < child_count)
                {
                    // Create an internal node for this pair
                    t_aabb combined_bounds;
                    
                    // Compute combined bounds
                    combined_bounds.min.x = fmin(children[i]->bounds.min.x, children[i+1]->bounds.min.x);
                    combined_bounds.min.y = fmin(children[i]->bounds.min.y, children[i+1]->bounds.min.y);
                    combined_bounds.min.z = fmin(children[i]->bounds.min.z, children[i+1]->bounds.min.z);
                    
                    combined_bounds.max.x = fmax(children[i]->bounds.max.x, children[i+1]->bounds.max.x);
                    combined_bounds.max.y = fmax(children[i]->bounds.max.y, children[i+1]->bounds.max.y);
                    combined_bounds.max.z = fmax(children[i]->bounds.max.z, children[i+1]->bounds.max.z);
                    
                    t_bvh_node *parent = create_bvh_node(combined_bounds, 0, current_iter);
                    parent->left = children[i];
                    parent->right = children[i+1];
                    
                    children[new_count++] = parent;
                }
                else
                {
                    // Odd number of nodes, just keep this one
                    children[new_count++] = children[i];
                }
            }
            child_count = new_count;
        }
        
        // The final node becomes the root's left child
        if (child_count == 1)
        {
            // Free the original node and use the combined one instead
            t_bvh_node *temp = node;
            node = children[0];
            
            // Update iteration value from the original node
            node->iteration = temp->iteration;
            
            free(temp);
        }
    }
    else
    {
        // If we didn't add any children, make this a leaf
        node->is_leaf = 1;
    }
    
    return node;
}

// Main function to build the BVH
t_bvh_node *build_menger_bvh(int max_iterations)
{
    t_aabb bounds;
    bounds.min = (t_vec3){-1.0, -1.0, -1.0};
    bounds.max = (t_vec3){1.0, 1.0, 1.0};
    
    return build_menger_bvh_recursive(bounds, max_iterations, max_iterations);
}

// Free the BVH tree
void free_bvh(t_bvh_node *node)
{
    if (!node)
    {
        return;
    }
    
    // Free children first
    if (node->left)
    {
        free_bvh(node->left);
        node->left = NULL;
    }
    
    if (node->right)
    {
        free_bvh(node->right);
        node->right = NULL;
    }
    
    // Finally free this node
    free(node);
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

int ray_intersect_aabb_simd(t_aabb bounds, t_vec3 origin, t_vec3 dir, double *out_tmin, double *out_tmax)
{
	float32x4_t bounds_min = { bounds.min.x, bounds.min.y, bounds.min.z, 0.0f };
	float32x4_t bounds_max = { bounds.max.x, bounds.max.y, bounds.max.z, 0.0f };
	float32x4_t ray_orig    = { origin.x, origin.y, origin.z, 0.0f };
	float32x4_t ray_dir     = { dir.x, dir.y, dir.z, 1.0f };

	float32x4_t inv_dir = vrecpeq_f32(ray_dir); // reciprocal approximation
	inv_dir = vmulq_f32(vrecpsq_f32(ray_dir, inv_dir), inv_dir); // Newton-Raphson refine

	float32x4_t t1 = vmulq_f32(vsubq_f32(bounds_min, ray_orig), inv_dir);
	float32x4_t t2 = vmulq_f32(vsubq_f32(bounds_max, ray_orig), inv_dir);

	float32x4_t tmin_vec = vminq_f32(t1, t2);
	float32x4_t tmax_vec = vmaxq_f32(t1, t2);

	// t_near = max(tmin.x, tmin.y, tmin.z)
	float32x2_t tmin_low = vget_low_f32(tmin_vec);
	// float32x2_t tmin_high = vget_high_f32(tmin_vec);
	float32x2_t tmin_xy = vpmax_f32(tmin_low, tmin_low); // max(tmin.x, tmin.y)
	float t_near = fmaxf(vget_lane_f32(tmin_xy, 0), vgetq_lane_f32(tmin_vec, 2)); // max(..., tmin.z)

	// t_far = min(tmax.x, tmax.y, tmax.z)
	float32x2_t tmax_low = vget_low_f32(tmax_vec);
	// float32x2_t tmax_high = vget_high_f32(tmax_vec);
	float32x2_t tmax_xy = vpmin_f32(tmax_low, tmax_low); // min(tmax.x, tmax.y)
	float t_far = fminf(vget_lane_f32(tmax_xy, 0), vgetq_lane_f32(tmax_vec, 2)); // min(..., tmax.z)

	if (t_near > t_far || t_far < 0.0f)
		return 0;

	if (out_tmin) *out_tmin = (double)t_near;
	if (out_tmax) *out_tmax = (double)t_far;
	return 1;
}


//optimized version of the ray_intersect_bvh function 
int ray_intersect_bvh(t_bvh_node *node, t_vec3 ray_origin, t_vec3 ray_dir,
                      double *t_min, double *t_max)
{
    if (!node)
        return 0;

    double node_tmin, node_tmax;
    if (!ray_intersect_aabb(node->bounds, ray_origin, ray_dir, &node_tmin, &node_tmax))
        return 0;

    if (node->is_leaf)
    {
        *t_min = node_tmin + 0.0001;
        *t_max = node_tmax;
        return 1;
    }

    double left_tmin = INFINITY, left_tmax = -INFINITY;
    double right_tmin = INFINITY, right_tmax = -INFINITY;
    int hit_left = node->left && ray_intersect_aabb(node->left->bounds, ray_origin, ray_dir, &left_tmin, &left_tmax);
    int hit_right = node->right && ray_intersect_aabb(node->right->bounds, ray_origin, ray_dir, &right_tmin, &right_tmax);

    if (!hit_left && !hit_right)
        return 0;

    t_bvh_node *first = NULL;
    t_bvh_node *second = NULL;
    double second_tmin = 0;

    if (hit_left && hit_right) {
        if (left_tmin < right_tmin) {
            first = node->left;
            second = node->right;
            second_tmin = right_tmin;
        } else {
            first = node->right;
            second = node->left;
            second_tmin = left_tmin;
        }
    } else if (hit_left) {
        first = node->left;
    } else if (hit_right) {
        first = node->right;
    }

    double temp_min, temp_max;
    if (ray_intersect_bvh(first, ray_origin, ray_dir, &temp_min, &temp_max)) {
        *t_min = temp_min;
        *t_max = temp_max;

        // Check second child only if it might have a closer hit
        if (second && second_tmin < temp_min) {
            double other_min, other_max;
            if (ray_intersect_bvh(second, ray_origin, ray_dir, &other_min, &other_max) && other_min < *t_min) {
                *t_min = other_min;
                *t_max = other_max;
            }
        }
        return 1;
    }

    // If first missed, try second
    if (second && ray_intersect_bvh(second, ray_origin, ray_dir, &temp_min, &temp_max)) {
        *t_min = temp_min;
        *t_max = temp_max;
        return 1;
    }

    return 0;
}


/* 
// Ray-BVH intersection test (recursive)
int ray_intersect_bvh(t_bvh_node *node, t_vec3 ray_origin, t_vec3 ray_dir,
                     double *t_min, double *t_max)
{
    if (!node)
        return 0;
    
    // Check if ray intersects the node's bounding box
    double node_tmin, node_tmax;
    if (!ray_intersect_aabb(node->bounds, ray_origin, ray_dir, &node_tmin, &node_tmax))
        return 0;
    
    // If this is a leaf node, we've found an intersection
    if (node->is_leaf)
    {
        // Use slightly larger t_min to avoid z-fighting and precision issues
        // This helps with thin surfaces causing striping
        *t_min = node_tmin + 0.0001;
        *t_max = node_tmax;
        return 1;
    }
    
    // Check both children and find closest intersection
    double left_tmin = INFINITY, left_tmax = -INFINITY;
    double right_tmin = INFINITY, right_tmax = -INFINITY;
    int hit_left = 0, hit_right = 0;
    
    // First check left child
    if (node->left)
        hit_left = ray_intersect_bvh(node->left, ray_origin, ray_dir, &left_tmin, &left_tmax);
    
    // Then check right child
    if (node->right)
        hit_right = ray_intersect_bvh(node->right, ray_origin, ray_dir, &right_tmin, &right_tmax);
    
    // Find closest hit
    if (hit_left && hit_right)
    {
        // Take the closest of the two intersections
        *t_min = (left_tmin < right_tmin) ? left_tmin : right_tmin;
        *t_max = (left_tmax > right_tmax) ? left_tmax : right_tmax;
        return 1;
    }
    else if (hit_left)
    {
        *t_min = left_tmin;
        *t_max = left_tmax;
        return 1;
    }
    else if (hit_right)
    {
        *t_min = right_tmin;
        *t_max = right_tmax;
        return 1;
    }
    
    return 0;
} */

// Get environment color for reflections
int get_environment_color(t_vec3 direction)
{
    // Create a simple sky/ground gradient based on the ray direction
    double y = direction.y;
    
    // Sky gradient (blue to white)
    if (y > 0) {
        double t = y * 0.5 + 0.5;  // remap to 0.5-1.0 range
        
        // Blend from light blue to white
        int r = (int)(135 + t * 120);
        int g = (int)(206 + t * 49);
        int b = 235;
        
        return (r << 16) | (g << 8) | b;
    } 
    // Ground gradient (dark to light gray)
    else {
        double t = -y * 0.5;  // remap to 0-0.5 range
        
        // Blend from dark gray to light gray
        int gray = (int)(50 + t * 150);
        
        return (gray << 16) | (gray << 8) | gray;
    }
}

// Helper function to reflect a ray
t_vec3 reflect_ray(t_vec3 incident, t_vec3 normal)
{
    double dot = incident.x * normal.x + incident.y * normal.y + incident.z * normal.z;
    t_vec3 reflection;
    
    reflection.x = incident.x - 2.0 * dot * normal.x;
    reflection.y = incident.y - 2.0 * dot * normal.y;
    reflection.z = incident.z - 2.0 * dot * normal.z;
    
    // Normalize the reflection vector
    double len = sqrt(reflection.x * reflection.x + reflection.y * reflection.y + reflection.z * reflection.z);
    reflection.x /= len;
    reflection.y /= len;
    reflection.z /= len;
    
    return reflection;
}

// Helper function to blend two colors with a given weight
int blend_colors(int color1, int color2, double weight)
{
    int r1 = (color1 >> 16) & 0xFF;
    int g1 = (color1 >> 8) & 0xFF;
    int b1 = color1 & 0xFF;
    
    int r2 = (color2 >> 16) & 0xFF;
    int g2 = (color2 >> 8) & 0xFF;
    int b2 = color2 & 0xFF;
    
    int r = (int)(r1 * (1.0 - weight) + r2 * weight);
    int g = (int)(g1 * (1.0 - weight) + g2 * weight);
    int b = (int)(b1 * (1.0 - weight) + b2 * weight);
    
    return (r << 16) | (g << 8) | b;
}

void	init_3d(t_fractal *fractal)
{
	fractal->is_3d = 1;
	// Position camera to see the entire structure from a similar angle as the reference image
	fractal->camera.position = (t_vec3){3.0, 2.5, -2.5};
	// Look toward the origin with a slight tilt
	fractal->camera.rotation = (t_vec3){-0.6, 0.8, 0.1};
	// Use a wider FOV to see more of the scene
	fractal->camera.fov = 55.0;
	fractal->camera.aspect_ratio = (double)WIDTH / HEIGHT;
	fractal->camera.near = 0.1;
	fractal->camera.far = 20.0;

	// Default Menger sponge settings
	fractal->menger.iterations = 1;  // Start with 1 iteration (can be increased)
	fractal->menger.size = 1.0;
	fractal->menger.position = (t_vec3){0, 0, 0};
	fractal->menger.rotation = (t_vec3){0, 0, 0};
	
	// Initialize the BVH root to NULL
	fractal->menger.bvh_root = NULL;
	
	// Build the BVH for the Menger sponge
	fractal->menger.bvh_root = build_menger_bvh(fractal->menger.iterations);
	
	// Set a reasonable default resolution factor for performance
	fractal->resolution_factor = 2;  // Higher quality default (smaller value)

    // Position 4 (Top view)
    fractal->camera.fov = 80.0; // Use wider FOV to see more
    fractal->camera.position = (t_vec3){0.0, 3.0, 0.0}; // Directly above
    fractal->camera.rotation = (t_vec3){-1.57, 0.0, 0.0}; // Looking straight down
}

t_vec3	rotate_point(t_vec3 point, t_vec3 rotation)
{
	t_vec3	result;
	double	cos_x, sin_x, cos_y, sin_y, cos_z, sin_z;
	t_vec3  temp;
	
	cos_x = cos(rotation.x);
	sin_x = sin(rotation.x);
	cos_y = cos(rotation.y);
	sin_y = sin(rotation.y);
	cos_z = cos(rotation.z);
	sin_z = sin(rotation.z);

	// Rotate around X axis
	temp.x = point.x;
	temp.y = point.y * cos_x - point.z * sin_x;
	temp.z = point.y * sin_x + point.z * cos_x;

	// Rotate around Y axis
	result.x = temp.x * cos_y + temp.z * sin_y;
	result.y = temp.y;
	result.z = -temp.x * sin_y + temp.z * cos_y;

	// Rotate around Z axis (using result as input)
	temp = result;
	result.x = temp.x * cos_z - temp.y * sin_z;
	result.y = temp.x * sin_z + temp.y * cos_z;
	result.z = temp.z;

	return (result);
}

void	project_point(t_vec3 point, t_fractal *fractal, int *x, int *y)
{
	double	scale;
	double	fov_rad;

	fov_rad = fractal->camera.fov * M_PI / 180.0;
	scale = tan(fov_rad / 2.0);

	point.x = point.x / (point.z * scale);
	point.y = point.y / (point.z * scale * fractal->camera.aspect_ratio);

	*x = (int)((point.x + 1.0) * WIDTH / 2.0);
	*y = (int)((1.0 - point.y) * HEIGHT / 2.0);
}

// The is_menger_iteration function is not used anymore with BVH
// So we include it only if specifically needed for debugging
#ifdef DEBUG
// Simple check if a point is inside a cube
static int is_point_in_cube(t_vec3 point)
{
    return (fabs(point.x) < 1.0 && 
            fabs(point.y) < 1.0 && 
            fabs(point.z) < 1.0);
}

// Check if a given position should be part of the Menger sponge
static int is_menger_iteration(t_vec3 point, int iterations)
{
    int i, j, k;
    int iter;
    
    // Base case: a full cube
    if (iterations <= 0)
        return (is_point_in_cube(point));
    
    // Check each iteration of the fractal
    for (iter = 0; iter < iterations; iter++)
    {
        // Scale the coordinates
        double x = fabs(point.x) * 3.0;
        double y = fabs(point.y) * 3.0;
        double z = fabs(point.z) * 3.0;
        
        // Check if we're in one of the cut-out sections
        i = (int)floor(x) % 3;
        j = (int)floor(y) % 3;
        k = (int)floor(z) % 3;
        
        if (i == 1 && j == 1) return 0;
        if (i == 1 && k == 1) return 0;
        if (j == 1 && k == 1) return 0;
        
        // Rescale for next iteration
        point.x = fmod(x, 1.0);
        point.y = fmod(y, 1.0);
        point.z = fmod(z, 1.0);
    }
    
    return 1;
}
#endif

/* 
void *render_menger_thread(void *arg)
{
    t_menger_thread_data *data = (t_menger_thread_data *)arg;
    t_fractal *fractal = data->fractal;
    int start_y = data->start_y;
    int end_y = data->end_y;
    int resolution_factor = fractal->resolution_factor;
    
    t_vec3 ray_dir;
    t_vec3 ray_pos;
    double t_min, t_max;
    int color;
    int x, y;

    y = start_y;
    while (y < end_y)
    {
        x = 0;
        while (x < WIDTH)
        {
            // Skip pixels based on resolution factor (render lower resolution)
            if ((x % resolution_factor != 0) || (y % resolution_factor != 0))
            {
                x++;
                continue;
            }

            // Calculate ray direction based on screen coordinates
            ray_dir.x = (2.0 * x / (double)WIDTH - 1.0) * tan(fractal->camera.fov * M_PI / 360.0) 
                        * fractal->camera.aspect_ratio;
            ray_dir.y = (1.0 - 2.0 * y / (double)HEIGHT) * tan(fractal->camera.fov * M_PI / 360.0);
            ray_dir.z = 1.0;

            // Apply camera rotation
            ray_dir = rotate_point(ray_dir, fractal->camera.rotation);

            // Normalize ray direction
            double len = sqrt(ray_dir.x * ray_dir.x + ray_dir.y * ray_dir.y + ray_dir.z * ray_dir.z);
            ray_dir.x /= len;
            ray_dir.y /= len;
            ray_dir.z /= len;

            // Set ray origin at camera position
            ray_pos = fractal->camera.position;
            color = BLACK;

            // Define color palette
            int exterior_color = 0xAACCDD;  // Blue-gray exterior
            int interior_color = 0xFFA500;  // Orange/gold interior
            
            // Check if we have a valid BVH 
            if (fractal->menger.iterations == 0)
            {
                // If iterations is 0, just render a simple cube
                t_aabb cube;
                cube.min = (t_vec3){-1.0, -1.0, -1.0};
                cube.max = (t_vec3){1.0, 1.0, 1.0};
                
                if (ray_intersect_aabb(cube, ray_pos, ray_dir, &t_min, &t_max) && t_min > 0)
                {
                    // Calculate hit point
                    t_vec3 hit_point = {
                        ray_pos.x + ray_dir.x * t_min,
                        ray_pos.y + ray_dir.y * t_min,
                        ray_pos.z + ray_dir.z * t_min
                    };
                    
                    // Calculate epsilon for face detection
                    double epsilon = 0.001;
                    
                    // Determine which face was hit
                    t_vec3 normal = {0, 0, 0};
                    
                    // External faces get the exterior color
                    if (fabs(hit_point.x - 1.0) < epsilon) {
                        normal.x = 1.0;
                        color = exterior_color;
                    } 
                    else if (fabs(hit_point.x + 1.0) < epsilon) {
                        normal.x = -1.0;
                        color = exterior_color;
                    }
                    else if (fabs(hit_point.y - 1.0) < epsilon) {
                        normal.y = 1.0;
                        color = exterior_color;
                    }
                    else if (fabs(hit_point.y + 1.0) < epsilon) {
                        normal.y = -1.0;
                        color = exterior_color;
                    }
                    else if (fabs(hit_point.z - 1.0) < epsilon) {
                        normal.z = 1.0;
                        color = exterior_color;
                    }
                    else if (fabs(hit_point.z + 1.0) < epsilon) {
                        normal.z = -1.0;
                        color = exterior_color;
                    }
                    else {
                        // This shouldn't happen in a simple cube
                        color = interior_color;
                    }
                    
                    // Apply lighting
                    double light_dir_x = 0.5;
                    double light_dir_y = 0.5;
                    double light_dir_z = -1.0;
                    double len = sqrt(light_dir_x*light_dir_x + light_dir_y*light_dir_y + light_dir_z*light_dir_z);
                    light_dir_x /= len;
                    light_dir_y /= len;
                    light_dir_z /= len;
                    
                    // Calculate dot product for diffuse lighting
                    double light_intensity = -(normal.x * light_dir_x + normal.y * light_dir_y + normal.z * light_dir_z);
                    
                    // Apply ambient occlusion and lighting
                    if (light_intensity < 0.2)
                        light_intensity = 0.2;  // Ambient light minimum
                    
                    // Extract RGB components
                    int r = (color >> 16) & 0xFF;
                    int g = (color >> 8) & 0xFF;
                    int b = color & 0xFF;
                    
                    // Apply lighting
                    r = (int)(r * light_intensity);
                    g = (int)(g * light_intensity);
                    b = (int)(b * light_intensity);
                    
                    // Add specular highlights for metallic surfaces
                    if (light_intensity < 0) {
                        // In shadow, use just ambient
                        light_intensity = 0.2;
                    } else {
                        // In light, combine ambient and diffuse
                        light_intensity = 0.2 + light_intensity * 0.7;
                    }
                    
                    // Apply lighting
                    r = (int)(r * light_intensity);
                    g = (int)(g * light_intensity);
                    b = (int)(b * light_intensity);
                    
                    // Recombine
                    color = (r << 16) | (g << 8) | b;
                }
            }
            else if (fractal->menger.bvh_root)
            {
                // Use the BVH for raytracing the Menger sponge
                if (ray_intersect_bvh(fractal->menger.bvh_root, ray_pos, ray_dir, &t_min, &t_max) && t_min > 0)
                {
                    // Calculate hit point
                    t_vec3 hit_point = {
                        ray_pos.x + ray_dir.x * t_min,
                        ray_pos.y + ray_dir.y * t_min,
                        ray_pos.z + ray_dir.z * t_min
                    };
                    
                    // Calculate epsilon for face detection
                    double epsilon = 0.001;
                    t_vec3 normal = {0, 0, 0};
                    int is_interior = 0;
                    
                    // Size of the smallest cube in the structure
                    double size = 2.0 / pow(3.0, fractal->menger.iterations);
                    
                    // First determine if this is an exterior face or interior face
                    double abs_x = fabs(hit_point.x);
                    double abs_y = fabs(hit_point.y);
                    double abs_z = fabs(hit_point.z);
                    
                    // A point is on the exterior if it's on the outer boundary
                    if (fabs(abs_x - 1.0) < epsilon || 
                        fabs(abs_y - 1.0) < epsilon || 
                        fabs(abs_z - 1.0) < epsilon) {
                        // This is an exterior face
                        is_interior = 0;
                    } else {
                        // This is an interior face
                        is_interior = 1;
                    }
                    
                    // Now determine which face normal we have
                    // Check all possible cube boundaries at this iteration level
                    int hit_face = 0;
                    
                    for (double boundary = -1.0 + size; boundary <= 1.0; boundary += size)
                    {
                        if (fabs(hit_point.x - boundary) < epsilon) {
                            normal.x = 1.0;
                            hit_face = 1;
                            break;
                        }
                        else if (fabs(hit_point.x + boundary) < epsilon) {
                            normal.x = -1.0;
                            hit_face = 1;
                            break;
                        }
                        else if (fabs(hit_point.y - boundary) < epsilon) {
                            normal.y = 1.0;
                            hit_face = 1;
                            break;
                        }
                        else if (fabs(hit_point.y + boundary) < epsilon) {
                            normal.y = -1.0;
                            hit_face = 1;
                            break;
                        }
                        else if (fabs(hit_point.z - boundary) < epsilon) {
                            normal.z = 1.0;
                            hit_face = 1;
                            break;
                        }
                        else if (fabs(hit_point.z + boundary) < epsilon) {
                            normal.z = -1.0;
                            hit_face = 1;
                            break;
                        }
                    }
                    
                    // If we didn't find a face match, use the default approach
                    if (!hit_face)
                    {
                        // Figure out which component of the hit point is closest to a boundary
                        double x_dist = fmin(fabs(fmod(abs_x, size)), fabs(size - fmod(abs_x, size)));
                        double y_dist = fmin(fabs(fmod(abs_y, size)), fabs(size - fmod(abs_y, size)));
                        double z_dist = fmin(fabs(fmod(abs_z, size)), fabs(size - fmod(abs_z, size)));
                        
                        if (x_dist <= y_dist && x_dist <= z_dist) {
                            normal.x = (hit_point.x > 0) ? 1.0 : -1.0;
                        } else if (y_dist <= x_dist && y_dist <= z_dist) {
                            normal.y = (hit_point.y > 0) ? 1.0 : -1.0;
                        } else {
                            normal.z = (hit_point.z > 0) ? 1.0 : -1.0;
                        }
                    }
                    
                    // Assign color based on interior/exterior
                    if (is_interior) {
                        // Interior faces get the gold/orange color
                        color = interior_color;
                    } else {
                        // Exterior faces get the blue-gray color
                        color = exterior_color;
                    }
                    
                    // Apply ambient occlusion - darker for deep interiors
                    // The deeper we go, the darker it gets
                    double ambient = 0.3;  // Base ambient
                    
                    // Create a more detailed lighting system
                    double light_dir_x = 0.5;
                    double light_dir_y = 0.5;
                    double light_dir_z = -1.0;
                    double len = sqrt(light_dir_x*light_dir_x + light_dir_y*light_dir_y + light_dir_z*light_dir_z);
                    light_dir_x /= len;
                    light_dir_y /= len;
                    light_dir_z /= len;
                    
                    // Calculate dot product for diffuse lighting
                    double light_intensity = -(normal.x * light_dir_x + normal.y * light_dir_y + normal.z * light_dir_z);
                    
                    // Modulate ambient for interior faces
                    if (is_interior) {
                        // Calculate how deep inside the structure we are (0-1 scale)
                        double depth = (1.0 - fmax(fmax(abs_x, abs_y), abs_z)) * 0.5;
                        // Reduce ambient light based on depth
                        ambient -= depth * 0.2;
                        
                        // Also make deeper iterations darker for better depth perception
                        if (fractal->menger.iterations > 1) {
                            // Calculate local depth within the structure's recursive pattern
                            double local_x = fmod(abs_x, size * 3) / (size * 3);
                            double local_y = fmod(abs_y, size * 3) / (size * 3);
                            double local_z = fmod(abs_z, size * 3) / (size * 3);
                            
                            // If we're in a deeper level, reduce ambient more
                            double local_depth = 1.0 - fmax(fmax(local_x, local_y), local_z);
                            ambient -= local_depth * 0.1;
                        }
                    }
                    
                    // Add reflections before applying lighting
                    if (is_interior) {
                        // Gold surfaces are more reflective (metallic)
                        double reflectivity = 0.3;  // 30% reflective
                        
                        // Calculate reflection vector
                        t_vec3 reflect_dir = reflect_ray(ray_dir, normal);
                        
                        // Get environment color from the reflection direction
                        int reflected_color = get_environment_color(reflect_dir);
                        
                        // Cast a reflection ray
                        t_vec3 reflect_origin = hit_point;
                        // Slightly offset the origin to avoid self-intersection
                        reflect_origin.x += normal.x * 0.001;
                        reflect_origin.y += normal.y * 0.001;
                        reflect_origin.z += normal.z * 0.001;
                        
                        double reflect_t_min, reflect_t_max;
                        int reflection_hit = 0;
                        
                        // Check if reflection ray hits another part of the structure
                        if (fractal->menger.bvh_root) {
                            reflection_hit = ray_intersect_bvh(fractal->menger.bvh_root, 
                                                             reflect_origin, reflect_dir, 
                                                             &reflect_t_min, &reflect_t_max);
                        }
                        
                        if (reflection_hit && reflect_t_min > 0) {
                            // Hit another part of the structure
                            // For simplicity, just make the reflection darker
                            reflected_color = 0x221100;  // Dark orange-brown
                        }
                        
                        // Blend the surface color with reflection
                        color = blend_colors(color, reflected_color, reflectivity);
                    }
                    else {
                        // Exterior blue-gray surfaces are slightly reflective
                        double reflectivity = 0.15;  // 15% reflective
                        
                        // Calculate reflection vector
                        t_vec3 reflect_dir = reflect_ray(ray_dir, normal);
                        
                        // Get environment color from the reflection direction
                        int reflected_color = get_environment_color(reflect_dir);
                        
                        // Blend the surface color with reflection
                        color = blend_colors(color, reflected_color, reflectivity);
                    }
                    
                    // Apply lighting calculation
                    if (light_intensity < 0) {
                        // In shadow, use just ambient
                        light_intensity = ambient;
                    } else {
                        // In light, combine ambient and diffuse
                        light_intensity = ambient + light_intensity * 0.7;
                    }
                    
                    // Extract RGB components for lighting
                    int r = (color >> 16) & 0xFF;
                    int g = (color >> 8) & 0xFF;
                    int b = color & 0xFF;
                    
                    // Apply lighting
                    r = (int)(r * light_intensity);
                    g = (int)(g * light_intensity);
                    b = (int)(b * light_intensity);
                    
                    // Add specular highlights for metallic surfaces
                    if (is_interior) {
                        // Calculate specular intensity
                        double spec;
                        
                        // Calculate reflection vector if we didn't already
                        t_vec3 reflect_dir;
                        if (light_intensity > ambient) {
                            // We're in a lit area, calculate specular
                            reflect_dir = reflect_ray(ray_dir, normal);
                            
                            spec = reflect_dir.x * light_dir_x + 
                                 reflect_dir.y * light_dir_y + 
                                 reflect_dir.z * light_dir_z;
                                 
                            if (spec > 0) {
                                spec = pow(spec, 30.0); // Tighter specular highlight
                                
                                // Add specular to all channels (white highlight)
                                r = (int)fmin(255, r + 255 * spec * 0.6);
                                g = (int)fmin(255, g + 255 * spec * 0.6);
                                b = (int)fmin(255, b + 255 * spec * 0.5);
                            }
                        }
                    }
                    
                    // Recombine
                    color = (r << 16) | (g << 8) | b;
                }
            }
            
            // Fill a block of pixels for the low-res version
            for (int fill_y = 0; fill_y < resolution_factor && (y + fill_y) < end_y; fill_y++) {
                for (int fill_x = 0; fill_x < resolution_factor && (x + fill_x) < WIDTH; fill_x++) {
                    pixel_put(x + fill_x, y + fill_y, &fractal->img, color);
                }
            }
            x += resolution_factor;
        }
        y += resolution_factor;
    }
    
    return NULL;
} */

//optimized version of the render_menger_sponge function (inline pixel fill)
void *render_menger_thread(void *arg)
{
	t_menger_thread_data *data = (t_menger_thread_data *)arg;
	t_fractal *fractal = data->fractal;
	t_img *img = &fractal->img;
	int res = fractal->resolution_factor;
	int bpp_bytes = img->bpp / 8;

	t_vec3 ray_dir, ray_pos, hit_point, normal, reflect_dir;
	double t_min, t_max;
	int color, is_interior;
	double fov_scale = tan(fractal->camera.fov * M_PI / 360.0);
	double aspect_ratio = fractal->camera.aspect_ratio;

	t_vec3 light_dir = {0.5, 0.5, -1.0};
	double len = sqrt(light_dir.x * light_dir.x + light_dir.y * light_dir.y + light_dir.z * light_dir.z);
	light_dir.x /= len;
	light_dir.y /= len;
	light_dir.z /= len;

	for (int y = data->start_y; y < data->end_y; y += res)
	{
		for (int x = 0; x < WIDTH; x += res)
		{
			ray_dir.x = (2.0 * x / (double)WIDTH - 1.0) * fov_scale * aspect_ratio;
			ray_dir.y = (1.0 - 2.0 * y / (double)HEIGHT) * fov_scale;
			ray_dir.z = 1.0;
			ray_dir = rotate_point(ray_dir, fractal->camera.rotation);
			len = sqrt(ray_dir.x * ray_dir.x + ray_dir.y * ray_dir.y + ray_dir.z * ray_dir.z);
			ray_dir.x /= len;
			ray_dir.y /= len;
			ray_dir.z /= len;
			ray_pos = fractal->camera.position;
			color = BLACK;

			int exterior_color = 0xAACCDD;
			int interior_color = 0xFFA500;

			if (fractal->menger.iterations == 0)
			{
				t_aabb cube = {{-1.0, -1.0, -1.0}, {1.0, 1.0, 1.0}};
				if (ray_intersect_aabb(cube, ray_pos, ray_dir, &t_min, &t_max) && t_min > 0)
				{
					hit_point = (t_vec3){
						ray_pos.x + ray_dir.x * t_min,
						ray_pos.y + ray_dir.y * t_min,
						ray_pos.z + ray_dir.z * t_min
					};

					double eps = 0.001;
					normal = (t_vec3){0, 0, 0};

					if (fabs(hit_point.x - 1.0) < eps) normal.x = 1.0;
					else if (fabs(hit_point.x + 1.0) < eps) normal.x = -1.0;
					else if (fabs(hit_point.y - 1.0) < eps) normal.y = 1.0;
					else if (fabs(hit_point.y + 1.0) < eps) normal.y = -1.0;
					else if (fabs(hit_point.z - 1.0) < eps) normal.z = 1.0;
					else if (fabs(hit_point.z + 1.0) < eps) normal.z = -1.0;

					color = (normal.x || normal.y || normal.z) ? exterior_color : interior_color;

					// Lighting
					double dot = -(normal.x * light_dir.x + normal.y * light_dir.y + normal.z * light_dir.z);
					double intensity = fmax(0.2, 0.2 + fmax(dot, 0.0) * 0.7);

					int r = ((color >> 16) & 0xFF) * intensity;
					int g = ((color >> 8) & 0xFF) * intensity;
					int b = (color & 0xFF) * intensity;
					color = (r << 16) | (g << 8) | b;
				}
			}
			else if (fractal->menger.bvh_root)
			{
				if (ray_intersect_bvh(fractal->menger.bvh_root, ray_pos, ray_dir, &t_min, &t_max) && t_min > 0)
				{
					hit_point = (t_vec3){
						ray_pos.x + ray_dir.x * t_min,
						ray_pos.y + ray_dir.y * t_min,
						ray_pos.z + ray_dir.z * t_min
					};

					// === Interior/exterior and normal estimation ===
					double eps = 0.001;
					double size = 2.0 / pow(3.0, fractal->menger.iterations);
					double ax = fabs(hit_point.x);
					double ay = fabs(hit_point.y);
					double az = fabs(hit_point.z);

					is_interior = !(fabs(ax - 1.0) < eps || fabs(ay - 1.0) < eps || fabs(az - 1.0) < eps);
					normal = (t_vec3){0, 0, 0};

					// Try snapping to cube face boundaries
					for (double b = -1.0 + size; b <= 1.0; b += size)
					{
						if (fabs(hit_point.x - b) < eps) { normal.x = 1.0; break; }
						if (fabs(hit_point.y - b) < eps) { normal.y = 1.0; break; }
						if (fabs(hit_point.z - b) < eps) { normal.z = 1.0; break; }
						if (fabs(hit_point.x + b) < eps) { normal.x = -1.0; break; }
						if (fabs(hit_point.y + b) < eps) { normal.y = -1.0; break; }
						if (fabs(hit_point.z + b) < eps) { normal.z = -1.0; break; }
					}

					// Fallback: estimate closest normal
					if (normal.x == 0 && normal.y == 0 && normal.z == 0)
					{
						double dx = fmin(fabs(fmod(ax, size)), fabs(size - fmod(ax, size)));
						double dy = fmin(fabs(fmod(ay, size)), fabs(size - fmod(ay, size)));
						double dz = fmin(fabs(fmod(az, size)), fabs(size - fmod(az, size)));

						if (dx <= dy && dx <= dz) normal.x = (hit_point.x > 0) ? 1.0 : -1.0;
						else if (dy <= dx && dy <= dz) normal.y = (hit_point.y > 0) ? 1.0 : -1.0;
						else normal.z = (hit_point.z > 0) ? 1.0 : -1.0;
					}

					color = is_interior ? interior_color : exterior_color;

					// Ambient + light
					double ambient = 0.3;
					double dot = -(normal.x * light_dir.x + normal.y * light_dir.y + normal.z * light_dir.z);
					double light_intensity = (dot > 0) ? (ambient + dot * 0.7) : ambient;

					// Reflection
					double reflectivity = is_interior ? 0.3 : 0.15;
					reflect_dir = reflect_ray(ray_dir, normal);
					int reflected_color = get_environment_color(reflect_dir);

					t_vec3 reflect_origin = {
						hit_point.x + normal.x * 0.001,
						hit_point.y + normal.y * 0.001,
						hit_point.z + normal.z * 0.001
					};

					if (ray_intersect_bvh(fractal->menger.bvh_root, reflect_origin, reflect_dir, &t_min, &t_max))
					{
						reflected_color = is_interior ? 0x221100 : 0x8899AA;
					}

					color = blend_colors(color, reflected_color, reflectivity);

					// Apply lighting
					int r = ((color >> 16) & 0xFF) * light_intensity;
					int g = ((color >> 8) & 0xFF) * light_intensity;
					int b = (color & 0xFF) * light_intensity;
					color = (r << 16) | (g << 8) | b;
				}
			}

			// === INLINE PIXEL FILL ===
			for (int fy = 0; fy < res && (y + fy) < data->end_y; fy++)
			{
				int row_offset = (y + fy) * img->line_len;
				for (int fx = 0; fx < res && (x + fx) < WIDTH; fx++)
				{
					int offset = row_offset + (x + fx) * bpp_bytes;
					*(unsigned int *)(img->pixels_ptr + offset) = color;
				}
			}
		}
	}

	return NULL;
}



// Replace the render_menger_sponge function with this optimized version
void	render_menger_sponge(t_fractal *fractal)
{
    // Safety check - ensure we're in 3D mode and it's actually the Menger sponge
    if (!fractal->is_3d || ft_strncmp(fractal->name, "menger", 6) != 0)
        return;

    // Display rendering status
    display_progress(fractal, "Rendering Menger sponge...");
    
    // Clear the entire image with black to prevent any artifacts
    int x, y;
    for (y = 0; y < HEIGHT; y++)
    {
        for (x = 0; x < WIDTH; x++)
        {
            pixel_put(x, y, &fractal->img, BLACK);
        }
    }
    
    // Show black screen first to indicate processing
    draw_image_to_window(fractal);

    // Define number of threads - adjust based on system capabilities
    // Using more threads than CPU cores usually doesn't help and can hurt performance
    #define NUM_THREADS 8  // Adjust based on your CPU cores
    
    pthread_t threads[NUM_THREADS];
    t_menger_thread_data thread_data[NUM_THREADS];
    
    // Divide the screen into horizontal stripes for each thread
    int rows_per_thread = HEIGHT / NUM_THREADS;
    
    // Create and launch threads
    for (int i = 0; i < NUM_THREADS; i++)
    {
        thread_data[i].fractal = fractal;
        thread_data[i].start_y = i * rows_per_thread;
        thread_data[i].end_y = (i == NUM_THREADS - 1) ? HEIGHT : (i + 1) * rows_per_thread;
        
        pthread_create(&threads[i], NULL, render_menger_thread, &thread_data[i]);
    }
    
    // Wait for all threads to complete
    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }
    
    // Final display update - first draw the completed image
    draw_image_to_window(fractal);
    
    // THEN display status information on top of it
    display_status(fractal);
    
    // No need for progress message after rendering as status shows the info
} 