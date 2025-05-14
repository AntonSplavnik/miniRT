/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere_intersect.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 13:02:28 by abillote          #+#    #+#             */
/*   Updated: 2025/05/08 13:03:00 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "platform.h"

//Check if the ray intersect with the sphere. If yes, attribute the closest solution to t.
int	ray_sphere_intersect(t_ray ray, t_sphere sphere, double *t)
{
	double	a;
	double	b;
	double	c;
	double	discriminant;
	t_vec3	oc;

	//calculate vector from sphere center to ray origin
	oc = vec3_subtract(ray.origin, sphere.center);

	//calculate all needed coefficient for quadratic equation (at^2 + bt + c = 0)
	a = vec3_dot(ray.direction, ray.direction);
	b = 2.0 * vec3_dot(oc, ray.direction);
	c = vec3_dot(oc, oc) - sphere.radius * sphere.radius;

	//calculate the discrimant to know if equation has solutions
	discriminant = b * b - (4 * a * c);

	if (discriminant < 0)
		return (0); //no real solution

	//find the solutions (called roots) - should be greater than a very small value to avoid self-intersection
	double t1 = (-b - sqrt(discriminant)) / (2.0 * a);
	double t2 = (-b + sqrt(discriminant)) / (2.0 * a);

	// Find the nearest valid intersection
	if (t1 > 0.001 && (t1 < t2 || t2 < 0.001))
		*t = t1;
	else if (t2 > 0.001)
		*t = t2;
	else
		return (0);
	return (1);
}

t_vec3	sphere_normal_at_point(t_vec3 point, t_sphere sphere)
{
	t_vec3	normal;
	normal = vec3_subtract(point, sphere.center);
	return (vec3_normalize(normal));
}
