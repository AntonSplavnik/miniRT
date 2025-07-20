/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_shared.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/21 01:13:05 by antonsplavn       #+#    #+#             */
/*   Updated: 2025/07/21 01:43:21 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

/*
 * Process one sample ray and add to final color
 */
void	process_sample_ray(t_render_params *params, int sx, int sy)
{
	t_vec2	coords;
	t_ray	sample_ray;
	t_vec3	sample_color;

	coords.u = params->xy_coords.u + (sx + 0.5) / (double)params->samples;
	coords.v = params->xy_coords.v + (sy + 0.5) / (double)params->samples;
	compute_ray_direction(params->scene, &sample_ray, params->fov_scale, \
							coords);
	sample_color = trace_ray(params->scene, sample_ray, 0);
	params->final_color = vec3_add(params->final_color, sample_color);
}

/*
 * Render single pixel with exact sampling order as original
 */
void	render_pixel(t_render_params *params)
{
	int	sy;
	int	sx;

	params->final_color = vec3_create(0, 0, 0);
	sy = 0;
	while (sy < params->samples)
	{
		sx = 0;
		while (sx < params->samples)
		{
			process_sample_ray(params, sx, sy);
			sx++;
		}
		sy++;
	}
	apply_final_color(params);
}

/*
 * Apply final color with tone mapping and fill pixel block
 * Combines color processing and pixel block filling for efficiency
 */
void	apply_final_color(t_render_params *params)
{
	t_color_f	final_color_f;
	int			display_color;
	int			by;
	int			bx;

	params->final_color = vec3_scale(params->final_color, 1.0 / \
		(params->samples * params->samples));
	final_color_f = vec3_to_color_f(params->final_color);
	display_color = color_f_to_display(final_color_f);
	by = 0;
	while (by < params->scene->graphic_settings.resolution_factor && \
			(int)params->xy_coords.v + by < HEIGHT)
	{
		bx = 0;
		while (bx < params->scene->graphic_settings.resolution_factor && \
				(int)params->xy_coords.u + bx < WIDTH)
		{
			pixel_put((int)params->xy_coords.u + bx, (int)params->xy_coords.v \
			+ by, params->scene->app.img, display_color);
			bx++;
		}
		by++;
	}
}

/*
 * Core rendering logic maintaining exact floating point order
 * Combines row and line rendering for efficiency
 */
void	render_rows(t_render_params *params, t_thread_data *data)
{
	int	y;
	int	x;

	y = data->start_row;
	while (y < data->end_row)
	{
		x = 0;
		while (x < params->scene->width)
		{
			params->xy_coords.u = x;
			params->xy_coords.v = y;
			render_pixel(params);
			x += params->scene->graphic_settings.resolution_factor;
		}
		y += params->scene->graphic_settings.resolution_factor;
	}
}

/*
 * Setup render parameters from thread data
 */
void	setup_render_params(t_render_params *params, t_thread_data *data)
{
	params->scene = data->scene;
	params->fov_scale = tan(data->scene->camera.fov * M_PI / 360.0);
	params->samples = data->scene->graphic_settings.ssaa_samples;
	if (params->samples < 1)
		params->samples = 1;
}
