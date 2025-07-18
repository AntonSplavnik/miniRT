/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compute_rays.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 11:17:22 by abillote          #+#    #+#             */
/*   Updated: 2025/07/18 18:45:00 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"


void	compute_ray_direction(t_scene *scene, t_ray *ray, double fov_scale, double x, double y)
{
	double	u;
	double	v;
	t_vec3	ray_dir_camera;

	u = (2.0 * x / (double)scene->width - 1.0) * fov_scale;
	v = (1.0 - 2.0 * y / (double)scene->height) * fov_scale;
	u *= (double)scene->width / scene->height;
	ray_dir_camera = vec3_create(u, v, 1.0);
	ray->direction = rotate_point(ray_dir_camera, scene->camera.rotation);
	ray->direction = vec3_normalize(ray->direction);
	ray->origin = scene->camera.position;
}
