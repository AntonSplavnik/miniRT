/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   objects_intersections.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 11:16:07 by abillote          #+#    #+#             */
/*   Updated: 2025/05/02 13:09:38 by abillote         ###   ########.fr       */
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
	double	root;
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

	//find the nearest solution (called roots) - should be greater than a very small value to avoid self-intersection
	root = (-b - sqrt(discriminant)) / (2.0 * a); //check the first solution
	if (root < 0.001)
	{
		root = (-b + sqrt(discriminant)) / (2.0 * a); //check for the second solution
		if (root < 0.001)
			return (0); //no solution great enough
	}
	*t = root;
	return (1);
}
