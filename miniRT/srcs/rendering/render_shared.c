/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_shared.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 16:45:00 by abillote          #+#    #+#             */
/*   Updated: 2025/07/18 16:45:00 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

/*
 * Core rendering logic for processing rows of pixels
 * Used by both threaded and non-threaded rendering functions
 */
void	render_rows(t_scene *scene, double fov_scale, int samples, 
					int start_row, int end_row)
{
	int		y;
	int		x;
	t_vec3	final_color;
	int		sy;

	y = start_row;
	while (y < end_row)
	{
		x = 0;
		while (x < scene->width)
		{
			final_color = vec3_create(0, 0, 0);
			sy = 0;
			while (sy < samples)
			{
				render_pixel_samples(scene, fov_scale, &final_color, 
									x, y, samples, sy);
				sy++;
			}
			process_final_color(scene, final_color, samples, x, y);
			x += scene->graphic_settings.resolution_factor;
		}
		y += scene->graphic_settings.resolution_factor;
	}
}

/*
 * Handle pixel sampling for anti-aliasing
 */
void	render_pixel_samples(t_scene *scene, double fov_scale, t_vec3 *final_color,
						int x, int y, int samples, int sy)
{
	int		sx;
	double	px;
	double	py;
	t_ray	sample_ray;
	t_vec3	sample_color;

	sx = 0;
	while (sx < samples)
	{
		px = x + (sx + 0.5) / (double)samples;
		py = y + (sy + 0.5) / (double)samples;
		compute_ray_direction(scene, &sample_ray, fov_scale, px, py);
		sample_color = trace_ray(scene, sample_ray, 0);
		*final_color = vec3_add(*final_color, sample_color);
		sx++;
	}
}

/*
 * Process final color and apply to pixel block
 */
void	process_final_color(t_scene *scene, t_vec3 final_color, int samples,
						int x, int y)
{
	t_color_f	final_color_f;
	int			display_color;
	int			by;
	int			bx;

	final_color = vec3_scale(final_color, 1.0 / (samples * samples));
	final_color_f = vec3_to_color_f(final_color);
	display_color = color_f_to_display(final_color_f);
	by = 0;
	while (by < scene->graphic_settings.resolution_factor && y + by < HEIGHT)
	{
		bx = 0;
		while (bx < scene->graphic_settings.resolution_factor && x + bx < WIDTH)
		{
			pixel_put(x + bx, y + by, scene->app.img, display_color);
			bx++;
		}
		by++;
	}
}