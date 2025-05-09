/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   plane_intersect.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 10:25:20 by abillote          #+#    #+#             */
/*   Updated: 2025/05/09 10:34:45 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "platform.h"

int	ray_plane_intersect(t_ray ray, t_plane plane, double *t)
{
	double	denom;
	double	t_plane;

	denom = vec3_dot(ray.direction, plane.normal);

	//If nearly parallel, no intersection
	if (fabs(denom) < 0.0001)
		return 0;

	/*Calculate intersection with plane using formula:
	t = ((Po - o).N) / (d.N)
	Po = a point on the plane
	o = ray origin
	d = ray direction
	N = plane normal
	*/
	t_plane = vec3_dot(vec3_subtract(plane.point, ray.origin), plane.normal) / denom;

	//if intersectiom behind ray origin, no valid intersection
	if (t_plane < 0.001)
		return (0);

	*t = t_plane;
	return (1);
}
