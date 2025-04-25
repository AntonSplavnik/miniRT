/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mandelbrot3d.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: asplavni <asplavni@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 13:17:50 by asplavni          #+#    #+#             */
/*   Updated: 2024/10/07 13:17:50 by asplavni         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "platform.h"
#include <math.h>
#include <stdlib.h>
#include <pthread.h>

// Thread data structure for parallel rendering
typedef struct s_mandelbrot3d_thread_data
{
    t_fractal   *fractal;
    int         start_y;
    int         end_y;
} t_mandelbrot3d_thread_data;

// 3D Vector operations
t_vec3 vec3_add(t_vec3 a, t_vec3 b)
{
    t_vec3 result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;
    return result;
}

t_vec3 vec3_sub(t_vec3 a, t_vec3 b)
{
    t_vec3 result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    result.z = a.z - b.z;
    return result;
}

t_vec3 vec3_mul(t_vec3 a, double t)
{
    t_vec3 result;
    result.x = a.x * t;
    result.y = a.y * t;
    result.z = a.z * t;
    return result;
}

double vec3_dot(t_vec3 a, t_vec3 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

double vec3_length(t_vec3 v)
{
    return sqrt(vec3_dot(v, v));
}

t_vec3 vec3_normalize(t_vec3 v)
{
    double len = vec3_length(v);
    if (len > 0.0001)
        return vec3_mul(v, 1.0 / len);
    return v;
}

// Complex quaternion operations - no need to redefine the struct

t_quaternion quat_add(t_quaternion a, t_quaternion b)
{
    t_quaternion result;
    result.w = a.w + b.w;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;
    return result;
}

t_quaternion quat_mul(t_quaternion a, t_quaternion b)
{
    t_quaternion result;
    result.w = a.w * b.w - a.x * b.x - a.y * b.y - a.z * b.z;
    result.x = a.w * b.x + a.x * b.w + a.y * b.z - a.z * b.y;
    result.y = a.w * b.y - a.x * b.z + a.y * b.w + a.z * b.x;
    result.z = a.w * b.z + a.x * b.y - a.y * b.x + a.z * b.w;
    return result;
}

double quat_length_sq(t_quaternion q)
{
    return q.w * q.w + q.x * q.x + q.y * q.y + q.z * q.z;
}

// Height-mapped 2.5D Mandelbrot distance estimator
double mandelbrot3d_DE(t_vec3 pos, t_fractal *fractal)
{
    // Extract 2D coordinates from 3D position
    double x = pos.x;
    double y = pos.y;
    
    // Compute classic 2D Mandelbrot iteration
    double cx = x;
    double cy = y;
    double zx = 0.0;
    double zy = 0.0;
    
    double zx2 = 0.0;
    double zy2 = 0.0;
    
    // Derivative for smooth distance estimation
    double derivative = 1.0;
    
    int iterations = fractal->iterations_defintion;
    double bailout = fractal->escape_value;
    
    // Perform standard Mandelbrot iteration
    int i;
    for (i = 0; i < iterations; i++)
    {
        // Calculate derivative (for distance estimation)
        derivative = 2.0 * sqrt(zx2 + zy2) * derivative + 1.0;
        
        // Standard complex multiplication: z = z^2 + c
        double tmp = zx2 - zy2 + cx;
        zy = 2.0 * zx * zy + cy;
        zx = tmp;
        
        // Update squared values for efficiency
        zx2 = zx * zx;
        zy2 = zy * zy;
        
        // Check for bailout
        if ((zx2 + zy2) > bailout)
            break;
    }
    
    // Calculate distance from eye-ray to the 2.5D surface
    double dist_from_point = 0.0;
    
    if (i == iterations)
    {
        // Point is inside the set - create deeper valleys
        double depth = 0.2 + 0.8 * (sin(cx * 30) * sin(cy * 30) + 1.0) * 0.25;
        dist_from_point = fabs(pos.z + depth); // Create deeper terrain
    }
    else
    {
        // Point is outside the set
        // Calculate smooth iteration count for height mapping
        double smooth_i = i - log2(log2(zx2 + zy2) / log2(bailout));
        
        // Calculate Mandelbrot distance estimation on 2D plane
        double dist_2d = 0.5 * log(zx2 + zy2) * sqrt(zx2 + zy2) / derivative;
        
        // More dramatic height mapping - emphasize smaller features
        double log_smooth = log(1 + smooth_i) / log(1 + iterations);
        double height = pow(log_smooth, 0.7); // Non-linear mapping for more dramatic terrain
        
        // Add wave patterns based on distance from origin
        double dist_from_origin = sqrt(x*x + y*y);
        double wave = 0.1 * sin(dist_from_origin * 10.0);
        
        // Scale height to create more dramatic terrain
        height = height * 1.5 - 0.5; // Map to [-0.5, 1.0] range
        height += wave;
        
        // Apply additional details based on iteration patterns
        if (i < iterations / 3)
            height *= 1.2; // Enhance the main bulbs
        
        // Distance to the height-mapped surface
        dist_from_point = fabs(pos.z - height);
        
        // Blend with 2D distance field for smoother edges
        // Use smaller weight for dist_2d to create sharper features
        dist_from_point = fmin(dist_from_point, dist_2d * 1.5);
    }
    
    return dist_from_point;
}

// Generate ray from camera position toward the specified pixel
void generate_ray(int x, int y, t_fractal *fractal, t_vec3 *ray_origin, t_vec3 *ray_direction)
{
    // Calculate normalized device coordinates (-1 to 1)
    double ndc_x = (2.0 * x / WIDTH) - 1.0;
    double ndc_y = 1.0 - (2.0 * y / HEIGHT);
    
    // Adjust for aspect ratio to prevent distortion
    double aspect = (double)WIDTH / HEIGHT;
    ndc_x *= aspect;
    
    // Apply FOV (field of view)
    double fov_rad = fractal->camera.fov * M_PI / 180.0;
    double scale = tan(fov_rad * 0.5);
    ndc_x *= scale;
    ndc_y *= scale;
    
    // Create the ray direction in camera space
    t_vec3 dir = {ndc_x, ndc_y, -1.0};  // -1 because we look along negative z-axis
    dir = vec3_normalize(dir);
    
    // Apply camera rotation (simplified version)
    // For a proper implementation, we would use a rotation matrix
    *ray_direction = rotate_point(dir, fractal->camera.rotation);
    
    // Set ray origin to camera position
    *ray_origin = fractal->camera.position;
}

// Ray marching algorithm for height-mapped 2.5D Mandelbrot
int ray_march(t_vec3 origin, t_vec3 direction, t_fractal *fractal,
              double *hit_distance, t_vec3 *hit_point, t_vec3 *hit_normal)
{
    // High precision settings for better surface quality
    const double EPSILON = 0.00002;  // Precision threshold for surface detail
    const double MAX_DISTANCE = 20.0; // Limited distance for performance
    const int MAX_STEPS = 300;  // More steps for better detail capture
    
    double total_distance = 0.0;
    
    // Ray marching loop
    for (int i = 0; i < MAX_STEPS; i++)
    {
        // Calculate current point along the ray
        t_vec3 pos = vec3_add(origin, vec3_mul(direction, total_distance));
        
        // Calculate distance to the height-mapped surface
        double distance = mandelbrot3d_DE(pos, fractal);
        
        // Adaptive step size - smaller near surface for better precision
        double step_factor = 0.8;  // Default step factor
        
        // Reduce step size near the surface for better detail
        if (distance < 0.1)
            step_factor = 0.4;
        if (distance < 0.01)
            step_factor = 0.2;
            
        // Apply stepping
        total_distance += distance * step_factor;
        
        // Hit detection with adaptive epsilon
        if (distance < EPSILON || total_distance > MAX_DISTANCE)
            break;
    }
    
    // Check if we hit anything within reasonable distance
    if (total_distance < MAX_DISTANCE)
    {
        // We've hit the 2.5D surface
        *hit_distance = total_distance;
        *hit_point = vec3_add(origin, vec3_mul(direction, total_distance));
        
        // Calculate normal with central differences
        // This is crucial for proper lighting of the height-mapped surface
        const double h = 0.0005;  // Smaller step for more precise normal calculation
        t_vec3 grad;
        
        // Use central differences on height map for normal calculation
        grad.x = mandelbrot3d_DE(vec3_add(*hit_point, (t_vec3){h, 0, 0}), fractal) - 
                 mandelbrot3d_DE(vec3_add(*hit_point, (t_vec3){-h, 0, 0}), fractal);
        
        grad.y = mandelbrot3d_DE(vec3_add(*hit_point, (t_vec3){0, h, 0}), fractal) - 
                 mandelbrot3d_DE(vec3_add(*hit_point, (t_vec3){0, -h, 0}), fractal);
        
        grad.z = mandelbrot3d_DE(vec3_add(*hit_point, (t_vec3){0, 0, h}), fractal) - 
                 mandelbrot3d_DE(vec3_add(*hit_point, (t_vec3){0, 0, -h}), fractal);
        
        // Enhance z-component to emphasize height differences
        grad.z *= 1.2;
        
        // Normalize the gradient to get the normal
        *hit_normal = vec3_normalize(grad);
        
        return 1; // Hit success
    }
    
    return 0; // No hit
}

// Calculate color based on iteration count and surface position for 2.5D height-mapped Mandelbrot
int get_mandelbrot3d_color(t_vec3 hit_point, t_vec3 hit_normal, t_vec3 ray_direction, t_fractal *fractal)
{
    // Extract the 2D Mandelbrot coordinates from the hit point
    double x = hit_point.x;
    double y = hit_point.y;
    
    // For terrain coloring, we need to compute the actual Mandelbrot iteration
    double cx = x;
    double cy = y;
    double zx = 0.0;
    double zy = 0.0;
    int i;
    
    for (i = 0; i < fractal->iterations_defintion; i++)
    {
        double zx2 = zx * zx;
        double zy2 = zy * zy;
        
        if (zx2 + zy2 > fractal->escape_value)
            break;
            
        double temp = zx2 - zy2 + cx;
        zy = 2.0 * zx * zy + cy;
        zx = temp;
    }
    
    // Calculate smooth iteration count for better coloring
    double iter_normalized;
    double zx2 = zx * zx;
    double zy2 = zy * zy;
    
    if (i < fractal->iterations_defintion)
    {
        // Smooth coloring based on how quickly the point escapes
        double log_zn = log(zx2 + zy2) / 2.0;
        double nu = log(log_zn / log(fractal->escape_value)) / log(2.0);
        iter_normalized = (i + 1 - nu) / fractal->iterations_defintion;
    }
    else
    {
        // Inside the set
        iter_normalized = 0.0;
    }
    
    // Calculate distance from origin for radial patterns
    double dist_from_origin = sqrt(x*x + y*y);
    
    // Calculate polar angle for spiral patterns (adjusted for better visual)
    double angle = atan2(y, x);
    
    // Base color determination (similar to the reference image)
    double r, g, b;
    
    // Apply coloring based on polar coordinates and iterations
    // Golden-blue color scheme based on reference image
    if (i < fractal->iterations_defintion) // Outside the set
    {
        // Golden-blue variation based on iteration and angle
        double phase = iter_normalized * 15.0 + angle * 3.0 / M_PI;
        
        // Golden color base (adjusting for precise colors seen in the reference)
        r = 0.8 + 0.4 * sin(phase);
        g = 0.7 + 0.3 * sin(phase + 2.5);
        b = 0.2 + 0.8 * sin(phase + 4.0);
        
        // Add radial stripes (alternating gold and blue)
        double stripe = sin(dist_from_origin * 20.0 + iter_normalized * 30.0);
        
        if (stripe > 0.0)
        {
            // More golden
            r = r * 0.9 + 0.3;
            g = g * 0.9 + 0.3;
            b = b * 0.6;
        }
        else
        {
            // More blue-purple
            r = r * 0.6;
            g = g * 0.7;
            b = b * 0.8 + 0.4;
        }
        
        // Add fine detail based on high-frequency iteration bands
        double detail = sin(iter_normalized * 100.0) * 0.1;
        r += detail;
        g += detail;
        b += detail;
    }
    else // Inside the set
    {
        // Dark blue for inside areas
        r = 0.05;
        g = 0.05;
        b = 0.15;
        
        // Add subtle detail to inside areas
        double detail = 0.05 * sin(dist_from_origin * 40.0);
        r += detail;
        g += detail;
        b += detail * 2.0;
    }
    
    // Calculate lighting
    // Multiple light setup for better terrain visualization
    
    // Key light (main directional light)
    t_vec3 key_light_dir = vec3_normalize((t_vec3){0.5, 0.8, 1.0});
    double key_intensity = fmax(0.0, vec3_dot(hit_normal, key_light_dir));
    
    // Fill light (softer secondary light)
    t_vec3 fill_light_dir = vec3_normalize((t_vec3){-0.8, -0.3, 0.5});
    double fill_intensity = fmax(0.0, vec3_dot(hit_normal, fill_light_dir)) * 0.4;
    
    // Ambient occlusion approximation based on height
    double ambient = 0.15 + 0.05 * hit_point.z;
    
    // Calculate heightmap-specific specular highlights
    // View vector (from hit point to camera)
    t_vec3 view_dir = vec3_normalize(vec3_mul(ray_direction, -1.0));
    
    // Specular for key light
    t_vec3 key_half_vector = vec3_normalize(vec3_add(view_dir, key_light_dir));
    double key_specular = pow(fmax(0.0, vec3_dot(hit_normal, key_half_vector)), 40.0) * 0.6;
    
    // Apply lighting to base color
    double lighting = key_intensity + fill_intensity + ambient;
    
    // Special handling for rim lighting to highlight edges
    double rim = 1.0 - fabs(vec3_dot(hit_normal, view_dir));
    rim = pow(rim, 3.0) * 0.3;
    
    // Apply lighting and effects
    r = r * lighting + key_specular + rim;
    g = g * lighting + key_specular + rim;
    b = b * lighting + key_specular + rim * 0.7; // Less blue in rim light
    
    // Ensure colors are in valid range
    r = fmin(1.0, fmax(0.0, r));
    g = fmin(1.0, fmax(0.0, g));
    b = fmin(1.0, fmax(0.0, b));
    
    // Convert to RGB color - using the correct function format
    int ri = (int)(r * 255) & 0xFF;
    int gi = (int)(g * 255) & 0xFF;
    int bi = (int)(b * 255) & 0xFF;
    
    return (ri << 16) | (gi << 8) | bi;
}

// Thread function for parallel rendering
void *render_mandelbrot3d_thread(void *arg)
{
    t_mandelbrot3d_thread_data *data = (t_mandelbrot3d_thread_data *)arg;
    t_fractal *fractal = data->fractal;
    t_img *img = &fractal->img;
    
    int bpp_bytes = img->bpp / 8;
    
    // Determine resolution reduction factor (blocky preview first)
    int res = fractal->resolution_factor;
    
    // Process each pixel in assigned rows
    for (int y = data->start_y; y < data->end_y; y += res)
    {
        for (int x = 0; x < WIDTH; x += res)
        {
            t_vec3 ray_origin, ray_direction;
            double hit_distance;
            t_vec3 hit_point, hit_normal;
            
            // Generate ray for this pixel
            generate_ray(x, y, fractal, &ray_origin, &ray_direction);
            
            // Default to background color (black)
            int color = 0x000000;
            
            // Ray march to find intersection
            if (ray_march(ray_origin, ray_direction, fractal, &hit_distance, &hit_point, &hit_normal))
            {
                // Calculate color based on hit information
                color = get_mandelbrot3d_color(hit_point, hit_normal, ray_direction, fractal);
            }
            
            // Fill block of pixels for this resolution
            for (int fy = 0; fy < res && (y + fy) < data->end_y; fy++)
            {
                int row_offset = (y + fy) * img->line_len;
                for (int fx = 0; fx < res && (x + fx) < WIDTH; fx++)
                {
                    int offset = row_offset + (x + fx) * bpp_bytes;
                    if (offset >= 0 && offset < img->line_len * HEIGHT)
                        *(unsigned int *)(img->pixels_ptr + offset) = color;
                }
            }
        }
    }
    
    return NULL;
}

// Initialize 2.5D Mandelbrot parameters
void init_mandelbrot3d(t_fractal *fractal)
{
    // Set 3D mode flag
    fractal->is_3d = 1;
    
    // Set camera position for a semi-top-down view to match the reference image
    fractal->camera.position.x = 0.1;
    fractal->camera.position.y = -1.2;
    fractal->camera.position.z = 1.1;
    
    // Looking down at an angle onto the Mandelbrot set
    fractal->camera.rotation.x = -0.9;  // Look down at the set
    fractal->camera.rotation.y = 0.1;   // Slight rotation
    fractal->camera.rotation.z = 0.0;
    
    // Camera parameters
    fractal->camera.fov = 45.0;         // Tighter field of view
    fractal->camera.aspect_ratio = (double)WIDTH / HEIGHT;
    fractal->camera.near = 0.1;
    fractal->camera.far = 100.0;
    
    // Fractal parameters
    fractal->iterations_defintion = 32; // More iterations for finer detail
    fractal->escape_value = 4.0;
    fractal->resolution_factor = 2;     // Default quality
    
    // Display a message about the 2.5D Mandelbrot
    display_progress(fractal, "Initializing 2.5D Mandelbrot Terrain...");
}

// Main render function for 3D Mandelbrot
void render_mandelbrot3d(t_fractal *fractal)
{
    // Safety check
    if (!fractal->is_3d)
        return;
    
    // Display rendering status
    display_progress(fractal, "Rendering 3D Mandelbrot...");
    
    // Progressive rendering for better user experience
    
    // Define number of threads
    #define MB3D_THREADS 8
    
    pthread_t threads[MB3D_THREADS];
    t_mandelbrot3d_thread_data thread_data[MB3D_THREADS];
    
    // Divide the screen into horizontal stripes for each thread
    int rows_per_thread = HEIGHT / MB3D_THREADS;
    
    // Create and launch threads
    for (int i = 0; i < MB3D_THREADS; i++)
    {
        thread_data[i].fractal = fractal;
        thread_data[i].start_y = i * rows_per_thread;
        thread_data[i].end_y = (i == MB3D_THREADS - 1) ? HEIGHT : (i + 1) * rows_per_thread;
        
        pthread_create(&threads[i], NULL, render_mandelbrot3d_thread, &thread_data[i]);
    }
    
    // Wait for all threads to complete
    for (int i = 0; i < MB3D_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }
    
    // Update the display
    draw_image_to_window(fractal);
    
    // Show status when rendering is complete
    display_status(fractal);
} 