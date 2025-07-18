/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 11:19:24 by abillote          #+#    #+#             */
/*   Updated: 2025/06/20 12:05:37 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

/**
 * Main rendering thread function that handles ray tracing with reflections,
 * and checkerboard textures
 *
 * @param arg Thread data containing scene and thread-specific info
 * @return NULL when completed
 */
void *render_thread(void *arg)
{
    t_thread_data *data = (t_thread_data *)arg;
    t_scene *scene = data->scene;
    double fov_scale = tan(scene->camera.fov * M_PI / 360.0);
    int y = data->start_row;

    int samples = scene->graphic_settings.ssaa_samples;
    if (samples < 1) samples = 1;

    while (y < data->end_row)
    {
        int x = 0;
        while (x < scene->width)
        {
            t_vec3 final_color = vec3_create(0, 0, 0);

            // SSAA sampling
            for (int sy = 0; sy < samples; sy++)
            {
                for (int sx = 0; sx < samples; sx++)
                {
                    double px = x + (sx + 0.5) / (double)samples;
                    double py = y + (sy + 0.5) / (double)samples;

                    t_ray sample_ray;
                    compute_ray_direction(scene, &sample_ray, fov_scale, px, py);

                    t_vec3 sample_color = trace_ray(scene, sample_ray, 0);
                    final_color = vec3_add(final_color, sample_color);
                }
            }

            // Average samples in linear space
            final_color = vec3_scale(final_color, 1.0 / (samples * samples));

            // Convert to display color with tone mapping and gamma correction
            t_color_f final_color_f = vec3_to_color_f(final_color);
            int display_color = color_f_to_display(final_color_f);

            // Fill pixel block
            for (int by = 0; by < scene->graphic_settings.resolution_factor && y + by < HEIGHT; by++)
            {
                for (int bx = 0; bx < scene->graphic_settings.resolution_factor && x + bx < WIDTH; bx++)
                {
                    pixel_put(x + bx, y + by, scene->app.img, display_color);
                }
            }

            x += scene->graphic_settings.resolution_factor;
        }
        y += scene->graphic_settings.resolution_factor;
    }
    return NULL;
}

// void	render_scene(void *param)
void	render_scene(t_scene *scene)
{
	// t_scene *scene = (t_scene *)param;

	int				rows_per_thread;
	pthread_t		threads[NUM_THREADS];
	t_thread_data	thread_data[NUM_THREADS];

	// display_progress(scene, "Rendering...");
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

    draw_ui(scene);
}
