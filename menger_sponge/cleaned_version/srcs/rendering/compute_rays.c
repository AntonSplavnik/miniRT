/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compute_rays.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 11:17:22 by abillote          #+#    #+#             */
/*   Updated: 2025/05/21 11:17:40 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "platform.h"
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void	compute_ray_direction(t_scene *scene, t_ray *ray, double fov_scale, int x, int y)
{
	double	u;
	double	v;
	t_vec3	ray_dir_camera;

	//normalized based on FOV pixel coordinates
	u = (2.0 * x / (double)scene->width - 1.0) * fov_scale;
	v = (1.0 - 2.0 * y / (double)scene->height) * fov_scale;

	//appled aspect ratio correction
	u *= (double)scene->width / scene->height;

	//camera direction
	ray_dir_camera = vec3_create(u, v, 1.0);
	//apply camera rotation
	ray->direction = rotate_point(ray_dir_camera, scene->camera.rotation);
	ray->direction = vec3_normalize(ray->direction);

	//setting ray origin
	ray->origin = scene->camera.position;

}
