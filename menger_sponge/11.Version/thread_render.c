/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread_render.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: parallels <parallels@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 13:30:22 by asplavni          #+#    #+#             */
/*   Updated: 2025/04/11 17:56:23 by parallels        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

void	*thread_render(void *arg)
{
	t_thread_data	*data;
	t_complex		complex;
	int				y;

	data = (t_thread_data *)arg;
	complex.x = 0.0;
	complex.y = 0.0;
	
	if (data->start_row < 0)
		data->start_row = 0;
	if (data->end_row > HEIGHT)
		data->end_row = HEIGHT;
		
	y = data->start_row;
	while (y < data->end_row)
	{
		render_pixel_row(y, data->fractal, complex);
		y++;
	}
	return (NULL);
}
