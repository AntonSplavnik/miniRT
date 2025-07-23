/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 11:19:24 by abillote          #+#    #+#             */
/*   Updated: 2025/07/23 21:11:18 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

void	display_progress(int current, int total, double elapsed_time)
{
	int		bar_width;
	int		progress;
	int		i;

	bar_width = 50;
	progress = (current * bar_width) / total;
	printf("\rProgress: [");
	i = 0;
	while (i < bar_width)
	{
		if (i < progress)
			printf("=");
		else if (i == progress)
			printf(">");
		else
			printf(" ");
		i++;
	}
	if (elapsed_time >= 60.0)
		printf("] %d/%d (%d%%) - %.1fm", current, total, (current * 100) / total, elapsed_time / 60.0);
	else
		printf("] %d/%d (%d%%) - %.1fs", current, total, (current * 100) / total, elapsed_time);
	fflush(stdout);
}

int	get_next_row(t_work_queue *work_queue)
{
	int	row;

	pthread_mutex_lock(&work_queue->work_mutex);
	row = work_queue->current_row;
	if (row < work_queue->total_rows)
		work_queue->current_row++;
	pthread_mutex_unlock(&work_queue->work_mutex);
	return (row);
}

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
    int y;

    int samples = scene->graphic_settings.ssaa_samples;
    if (samples < 1)
        samples = 1;

    while ((y = get_next_row(data->work_queue)) < data->work_queue->total_rows)
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
                    t_vec2 coords;
                    coords.u = x + (sx + 0.5) / (double)samples;
                    coords.v = y + (sy + 0.5) / (double)samples;

                    t_ray sample_ray;
                    compute_ray_direction(scene, &sample_ray, fov_scale, coords);

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

        pthread_mutex_lock(data->progress_mutex);
        (*data->progress_counter)++;
        if ((*data->progress_counter) % 5 == 0 || (*data->progress_counter) == data->total_rows)
        {
            double current_elapsed = mlx_get_time() - data->start_time;
            display_progress(*data->progress_counter, data->total_rows, current_elapsed);
            fflush(stdout);
        }
        pthread_mutex_unlock(data->progress_mutex);
    }
    return NULL;
}

// void	render_scene(void *param)
void	render_scene(t_scene *scene)
{
	pthread_t		threads[NUM_THREADS];
	t_thread_data	thread_data[NUM_THREADS];
	t_work_queue	work_queue;
	int				progress_counter;
	int				total_rows;
	pthread_mutex_t	progress_mutex;
	static int		render_count = 0;
	double			start_time;

	render_count++;
	start_time = mlx_get_time();
	printf("\n=== RENDER START #%d ===\n", render_count);
	if (!scene)
	{
		printf("ERROR - Scene pointer is NULL\n");
		return;
	}
	progress_counter = 0;
	total_rows = scene->height / scene->graphic_settings.resolution_factor;
	pthread_mutex_init(&progress_mutex, NULL);

	work_queue.current_row = 0;
	work_queue.total_rows = total_rows;
	pthread_mutex_init(&work_queue.work_mutex, NULL);

	printf("Starting render with work-stealing...\n");
	int i = 0;
	while (i < NUM_THREADS)
	{
		thread_data[i].scene = scene;
		thread_data[i].work_queue = &work_queue;
		thread_data[i].progress_counter = &progress_counter;
		thread_data[i].total_rows = total_rows;
		thread_data[i].progress_mutex = &progress_mutex;
		thread_data[i].start_time = start_time;

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

    pthread_mutex_destroy(&progress_mutex);
    pthread_mutex_destroy(&work_queue.work_mutex);

    double elapsed_time = mlx_get_time() - start_time;

    if (elapsed_time >= 60.0)
        printf("\n=== RENDER COMPLETE #%d (%.2f minutes) ===\n\n", render_count, elapsed_time / 60.0);
    else
        printf("\n=== RENDER COMPLETE #%d (%.2f seconds) ===\n\n", render_count, elapsed_time);

    draw_ui(scene);
}
