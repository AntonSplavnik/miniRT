/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   objects_intersections.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 11:16:07 by abillote          #+#    #+#             */
/*   Updated: 2025/05/06 09:39:57 by abillote         ###   ########.fr       */
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

t_vec3	sphere_normal_at_point(t_vec3 point, t_sphere sphere)
{
	t_vec3	normal;
	normal = vec3_subtract(point, sphere.center);
	return (vec3_normalize(normal));
}

t_vec3	cylinder_normal_at_point(t_vec3 point, t_cylinder cylinder)
{
	t_vec3	normal;
	t_vec3	cp;
	double	projection;
	t_vec3	axis_projection;
	t_vec3	top_center;
	t_vec3	bottom_center;
	double	dist_to_top;
	double	dist_to_bottom;


	//Determine if we are on one of the caps
	//1. Calculate caps positions
	top_center = vec3_add(cylinder.center, vec3_scale(cylinder.axis, cylinder.height / 2.0));
	bottom_center = vec3_add(cylinder.center, vec3_scale(cylinder.axis, -cylinder.height / 2.0));

	//2. Calculate distance to caps from hit point
	dist_to_top = vec3_length(vec3_subtract(point, top_center));
	dist_to_bottom = vec3_length(vec3_subtract(point, bottom_center));

	//3. Check if we are on the top cap
	if (dist_to_top <= cylinder.radius && fabs(vec3_dot(vec3_subtract(point, top_center), cylinder.axis)) < 0.001)
		return (cylinder.axis);

	//4. check if we are on the bottom cap
	if (dist_to_bottom <= cylinder.radius && fabs(vec3_dot(vec3_subtract(point, bottom_center), cylinder.axis)) < 0.001)
		return (vec3_negate(cylinder.axis));

	//If not on one of the caps:
	//1. Calculate hit point to cylinder center vector
	cp = vec3_subtract(point, cylinder.center);

	//2. Calculate scalar projection along axis
	projection = vec3_dot(cp, cylinder.axis);

	//3. Calculate axis projection to find the axis point which is closest to our hit point
	axis_projection = vec3_scale(cylinder.axis, projection);

	//4. Calculate normal vector (N = cp - axis_projection)
	normal = vec3_subtract(cp, axis_projection);
	return (vec3_normalize(normal));

}

int	find_closest_intersection(t_scene *scene, t_ray ray, double *t, t_object **hit_object)
{
	t_object	*current;
	double		t_closest;
	double		t_temp;
	int			hit_something;

	current = scene->objects;
	t_closest = INFINITY;
	hit_something = 0;
	*hit_object = NULL;
	while (current)
	{
		if (current->type == SPHERE)
		{
			t_sphere *sphere = (t_sphere *)(current->data);
			if (ray_sphere_intersect(ray, *sphere, &t_temp) && t_temp < t_closest)
			{
				t_closest = t_temp;
				hit_something = 1;
				*hit_object = current;
			}
		}
		current = current->next;
	}
	if (hit_something)
		*t = t_closest;
	return (hit_something);
}
