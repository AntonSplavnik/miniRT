/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 11:19:24 by abillote          #+#    #+#             */
/*   Updated: 2025/05/23 17:38:49 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"


// void	*render_thread_old(void *arg)
// {
// 	t_thread_data *data = (t_thread_data *)arg;
// 	t_scene	*scene = data->scene;

// 	int				color;
// 	int				in_shadow;
// 	double			t;
// 	double			fov_scale;
// 	t_ray			ray;
// 	t_vec3			hit_point;
// 	t_vec3			normal;
// 	t_light_result	light_info;
// 	t_object		*hit_object;
// 	t_light			*current_light;
// 	int				light_color;

// 	in_shadow = 0;
// 	fov_scale = tan(scene->camera.fov * M_PI / 360.0);
// 	// int	samples_per_pixel = 4;

// 	for (int y = 0; y < scene->height; y+=scene->app.resolution_factor)
// 	{
// 		for (int x = 0; x < scene->width; x+=scene->app.resolution_factor)
// 		{
// 			color = scene->background_color;

// 			// Initialize ray direction before using it
// 			compute_ray_direction(scene, &ray, fov_scale, x, y);

// 			// ray tracing
// 			if (find_closest_intersection(scene, ray, &t, &hit_object))
// 			{
// 				compute_ray_intersection(ray, hit_object, t, &hit_point, &normal);

// 				// Start with just the ambient light
// 				color = get_pixel_color(hit_object, scene->ambient.ratio, 0.0, scene->ambient.color);

// 				// Accumulate light from all lights
// 				current_light = scene->lights;
// 				while (current_light)
// 				{
// 					light_info = compute_light(scene, hit_object, hit_point, normal, current_light);

// 					//Check if the hit point is in shadow for this light
// 					if(scene->app.enable_hard_shadows)
// 						in_shadow = is_in_shadow(scene, hit_point, light_info.light_dir, light_info.light_distance);

// 					if (!in_shadow)
// 					{
// 						// Add this light's contribution

// 						// Add diffuse and specular:
// 						double diffuse_intensity = scene->ambient.ratio + (current_light->intensity * light_info.diffuse);
// 						double specular_intensity = current_light->intensity * light_info.specular_intensity;
// 						light_color = get_pixel_color(hit_object, diffuse_intensity, specular_intensity, current_light->color);

// 						// Extract RGB components
// 						int r = (light_color >> 16) & 0xFF;
// 						int g = (light_color >> 8) & 0xFF;
// 						int b = light_color & 0xFF;

// 						// Add to final color
// 						int final_r = valid_color_range(((color >> 16) & 0xFF) + r);
// 						int final_g = valid_color_range(((color >> 8) & 0xFF) + g);
// 						int final_b = valid_color_range((color & 0xFF) + b);

// 						color = (final_r << 16) | (final_g << 8) | final_b;
// 					}

// 					current_light = current_light->next;
// 				}
// 			}

// 			// Fill the entire block with this color
// 			for (int by = 0; by < scene->app.resolution_factor && y + by < HEIGHT; by++)
// 			{
// 				for (int bx = 0; bx < scene->app.resolution_factor && x + bx < WIDTH; bx++)
// 				{
// 					pixel_put(x + bx, y + by, &scene->img, color);
// 				}
// 			}
// 		}
// 	}
// 	return (NULL);
// }

/**
 * Main rendering thread function that handles ray tracing with reflections,
 * and checkerboard textures
 *
 * @param arg Thread data containing scene and thread-specific info
 * @return NULL when completed
 */
void *render_thread(void *arg)
{
	t_thread_data *data;
	t_scene *scene;
	double fov_scale;
	t_ray ray;
	int color;
	int y;
	int x;
	int by;
	int bx;

	data = (t_thread_data *)arg;
	scene = data->scene;
	y = data->start_row;
	fov_scale = tan(scene->camera.fov * M_PI / 360.0);
	while (y < data->end_row)
	{
		x = 0;
		while (x < scene->width)
		{
			compute_ray_direction(scene, &ray, fov_scale, x, y);
			color = trace_ray(scene, ray, 0);
			by = 0;
			while (by < scene->graphic_settings.resolution_factor && y + by < HEIGHT)
			{
				bx = 0;
				while (bx < scene->graphic_settings.resolution_factor && x + bx < WIDTH)
				{
					pixel_put(x + bx, y + by, scene->app.img, color);
					bx++;
				}
				by++;
			}
			x += scene->graphic_settings.resolution_factor;
		}
		y += scene->graphic_settings.resolution_factor;
	}
	return NULL;
}

void	render_scene(t_scene *scene)
{

	int	rows_per_thread;
	pthread_t	threads[NUM_THREADS];
	t_thread_data	thread_data[NUM_THREADS];

	display_progress(scene, "Rendering...");
	if (!scene)
	{
		printf("ERROR - Scene pointer is NULL\n");
		return;
	}
	rows_per_thread = scene->height / NUM_THREADS;
	int i = 0;
	while (i < NUM_THREADS)
	{
		thread_data[i].scene = scene;
		thread_data[i].start_row = i * rows_per_thread;

		if(i == NUM_THREADS - 1)
			thread_data[i].end_row = scene->height;
		else
			thread_data[i].end_row = (i + 1) * rows_per_thread;

		if (pthread_create(&threads[i], NULL, render_thread, &thread_data[i]) != 0)
        {
            // If thread creation fails, fall back to single-threaded rendering
            // (This would need to be implemented separately)
            perror("Thread creation failed");
            exit(1);
        }
		i++;
	}

    i = 0;
    while (i < NUM_THREADS)
    {
        pthread_join(threads[i], NULL);
        i++;
    }

    // Display status (FPS, settings, etc.)
    display_status(scene);
}
