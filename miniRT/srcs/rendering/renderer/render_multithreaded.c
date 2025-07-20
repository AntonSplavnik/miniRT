/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_multithreaded.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 01:15:00 by antonsplavn       #+#    #+#             */
/*   Updated: 2025/07/21 01:15:00 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

/*
 * Create all rendering threads with proper error handling
 */
void	create_all_threads(t_scene *scene, int rows_per_thread,
		pthread_t *threads, t_thread_data *thread_data)
{
	int	i;

	i = 0;
	while (i < NUM_THREADS)
	{
		thread_data[i].scene = scene;
		thread_data[i].start_row = i * rows_per_thread;
		if (i == NUM_THREADS - 1)
			thread_data[i].end_row = scene->height;
		else
			thread_data[i].end_row = (i + 1) * rows_per_thread;
		if (pthread_create(&threads[i], NULL, render_thread, \
				&thread_data[i]) != 0)
		{
			perror("Thread creation failed");
			exit(1);
		}
		i++;
	}
}

/*
 * Join all rendering threads
 */
void	join_all_threads_norm(pthread_t *threads)
{
	int	i;

	i = 0;
	while (i < NUM_THREADS)
	{
		pthread_join(threads[i], NULL);
		i++;
	}
}

/*
 * Main rendering thread function that handles ray tracing with reflections,
 * and checkerboard textures - maintains exact floating point order
 */
void	*render_thread(void *arg)
{
	t_thread_data	*data;
	t_render_params	params;

	data = (t_thread_data *)arg;
	params.scene = data->scene;
	params.fov_scale = tan(data->scene->camera.fov * M_PI / 360.0);
	params.samples = data->scene->graphic_settings.ssaa_samples;
	if (params.samples < 1)
		params.samples = 1;
	render_rows(&params, data);
	return (NULL);
}

/*
 * Multi-threaded rendering function maintaining exact behavior
 */
void	render_scene(t_scene *scene)
{
	pthread_t		threads[NUM_THREADS];
	t_thread_data	thread_data[NUM_THREADS];
	int				rows_per_thread;

	if (!scene)
	{
		ft_printf("ERROR - Scene pointer is NULL\n");
		return ;
	}
	rows_per_thread = scene->height / NUM_THREADS;
	create_all_threads(scene, rows_per_thread, threads, thread_data);
	join_all_threads_norm(threads);
	draw_ui(scene);
}
