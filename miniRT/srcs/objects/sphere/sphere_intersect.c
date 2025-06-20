/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere_intersect.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 13:02:28 by abillote          #+#    #+#             */
/*   Updated: 2025/06/18 16:22:23 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/miniRT.h"

static double	calculate_sphere_equation_discriminant(t_ray ray, \
						t_sphere sphere, double *a, double *b)
{
	double	c;
	double	discriminant;
	t_vec3	oc;

	oc = vec3_subtract(ray.origin, sphere.center);
	*a = vec3_dot(ray.direction, ray.direction);
	*b = 2.0 * vec3_dot(oc, ray.direction);
	c = vec3_dot(oc, oc) - sphere.radius * sphere.radius;
	discriminant = *b * *b - (4 * *a * c);
	return (discriminant);
}

//Check if the ray intersect with the sphere.
// If yes, attribute the closest solution to t.
//To check the intersection, try to solve the sphere quadratic equation
int	ray_sphere_intersect(t_ray ray, t_sphere sphere, double *t)
{
	double	a;
	double	b;
	double	discriminant;
	double	t1;
	double	t2;

	discriminant = calculate_sphere_equation_discriminant(ray, sphere, &a, &b);
	if (discriminant < 0)
		return (0);
	t1 = (-b - sqrt(discriminant)) / (2.0 * a);
	t2 = (-b + sqrt(discriminant)) / (2.0 * a);
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
