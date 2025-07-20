/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   compute_rays.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/20 20:09:45 by antonsplavn       #+#    #+#             */
/*   Updated: 2025/07/20 20:09:47 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

void	compute_ray_direction(t_scene *scene, t_ray *ray, double fov_scale,
		t_vec2 coords)
{
	double	u;
	double	v;
	t_vec3	ray_dir_camera;

	u = (2.0 * coords.u / (double)scene->width - 1.0) * fov_scale;
	v = (1.0 - 2.0 * coords.v / (double)scene->height) * fov_scale;
	u *= (double)scene->width / scene->height;
	ray_dir_camera = vec3_create(u, v, 1.0);
	ray->direction = rotate_point(ray_dir_camera, scene->camera.rotation);
	ray->direction = vec3_normalize(ray->direction);
	ray->origin = scene->camera.position;
}
