/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene_ray_intersect_bvh.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/23 00:00:00 by antonsplavn      #+#    #+#             */
/*   Updated: 2025/06/23 00:00:00 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

int	intersect_sphere(t_ray ray, t_sphere *sphere, double *t_temp)
{
	return (ray_sphere_intersect(ray, *sphere, t_temp));
}

int	intersect_plane(t_ray ray, t_plane *plane, double *t_temp)
{
	return (ray_plane_intersect(ray, *plane, t_temp));
}

int	intersect_cylinder(t_ray ray, t_cylinder *cylinder, double *t_temp)
{
	return (ray_cylinder_intersect(ray, *cylinder, t_temp));
}

int	intersect_cube(t_ray ray, t_cube *cube, double *t_temp)
{
	return (ray_cube_intersect(ray, *cube, t_temp));
}

int	intersect_triangle(t_ray ray, t_triangle *triangle, double *t_temp)
{
	return (ray_triangle_intersect(ray, *triangle, t_temp));
}

int	intersect_triangle_bary(t_ray ray, t_triangle *triangle, double *t_temp, t_vec3 *bary)
{
	return (ray_triangle_intersect_bary(ray, *triangle, t_temp, bary));
}
