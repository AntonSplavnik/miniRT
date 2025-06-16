/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 11:19:24 by abillote          #+#    #+#             */
/*   Updated: 2025/06/11 13:29:15 by antonsplavn      ###   ########.fr       */
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
	t_thread_data *data;
	t_scene *scene;
	double fov_scale;
	// t_ray ray;
	// int color;
	int y;
	int x;
	int by;
	int bx;

	data = (t_thread_data *)arg;
	scene = data->scene;
	y = data->start_row;
	fov_scale = tan(scene->camera.fov * M_PI / 360.0);

	int samples = scene->graphic_settings.ssaa_samples; // 1 = off, 2 = 2x2 SSAA, etc.
	if (samples < 1) samples = 1;
	float f_samples = (float)samples;
	float step = 1.0f / f_samples;


	while (y < data->end_row)
	{
		x = 0;
		while (x < scene->width)
		{
			t_color final_color = {0, 0, 0};

			for (int sy = 0; sy < samples; sy++)
			{
				for (int sx = 0; sx < samples; sx++)
				{
					float px = x + (sx + 0.5f) * step;
					float py = y + (sy + 0.5f) * step;
					t_ray sample_ray;

					compute_ray_direction(scene, &sample_ray, fov_scale, px, py);
					int sample_color = trace_ray(scene, sample_ray, 0);
					t_color c = int_to_color(sample_color);
					final_color.r += c.r;
					final_color.g += c.g;
					final_color.b += c.b;
				}
			}
			int total = samples * samples;
			final_color.r /= total;
			final_color.g /= total;
			final_color.b /= total;

			int avg_color = color_to_int(final_color);
			by = 0;
			while (by < scene->graphic_settings.resolution_factor && y + by < HEIGHT)
			{
				bx = 0;
				while (bx < scene->graphic_settings.resolution_factor && x + bx < WIDTH)
				{
					pixel_put(x + bx, y + by, scene->app.img, avg_color);
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
