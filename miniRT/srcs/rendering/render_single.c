/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_single.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/18 16:45:00 by abillote          #+#    #+#             */
/*   Updated: 2025/07/18 16:45:00 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

/*
 * Single-threaded rendering function for systems without pthread support
 * or when debugging threading issues
 */
void	render_scene(t_scene *scene)
{
	double	fov_scale;
	int		y;
	int		samples;

	if (!scene)
	{
		printf("ERROR - Scene pointer is NULL\n");
		return;
	}
	fov_scale = tan(scene->camera.fov * M_PI / 360.0);
	y = 0;
	samples = scene->graphic_settings.ssaa_samples;
	if (samples < 1)
		samples = 1;
	render_rows(scene, fov_scale, samples, y, scene->height);
	draw_ui(scene);
}