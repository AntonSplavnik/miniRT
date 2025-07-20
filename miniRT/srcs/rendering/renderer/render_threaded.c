/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_threaded.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 16:45:00 by abillote          #+#    #+#             */
/*   Updated: 2025/07/18 16:45:00 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

/*
 * Main rendering thread function that handles ray tracing with reflections,
 * and checkerboard textures
 */
void *render_thread(void *arg)
{
	t_thread_data *data = (t_thread_data *)arg;
	t_scene *scene = data->scene;
	double fov_scale = tan(scene->camera.fov * M_PI / 360.0);
	int samples = scene->graphic_settings.ssaa_samples;

	if (samples < 1)
		samples = 1;
	render_rows(scene, fov_scale, samples, data->start_row, data->end_row);
	return NULL;
}

/*
 * Multi-threaded rendering function (original implementation)
 */
void	render_scene(t_scene *scene)
{
	int				rows_per_thread;
	pthread_t		threads[NUM_THREADS];
	t_thread_data	thread_data[NUM_THREADS];
	int				i;

	if (!scene)
	{
		printf("ERROR - Scene pointer is NULL\n");
		return;
	}
	rows_per_thread = scene->height / NUM_THREADS;
	i = 0;
	while (i < NUM_THREADS)
	{
		thread_data[i].scene = scene;
		thread_data[i].start_row = i * rows_per_thread;
		if(i == NUM_THREADS - 1)
			thread_data[i].end_row = scene->height;
		else
			thread_data[i].end_row = (i + 1) * rows_per_thread;
		create_thread_with_fallback(&threads[i], &thread_data[i]);
		i++;
	}
	join_all_threads(threads);
	draw_ui(scene);
}

/*
 * Create thread with fallback on failure
 */
void	create_thread_with_fallback(pthread_t *thread, t_thread_data *data)
{
	if (pthread_create(thread, NULL, render_thread, data) != 0)
	{
		perror("Thread creation failed");
		exit(1);
	}
}

/*
 * Join all rendering threads
 */
void	join_all_threads(pthread_t *threads)
{
	int	i;

	i = 0;
	while (i < NUM_THREADS)
	{
		pthread_join(threads[i], NULL);
		i++;
	}
}