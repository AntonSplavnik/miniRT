/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   objects_intersections.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 11:16:07 by abillote          #+#    #+#             */
/*   Updated: 2025/05/07 15:05:52 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "platform.h"

int	is_within_cylinder_height(t_ray ray, t_cylinder cylinder, double t)
{
	t_vec3	intersection;
	t_vec3	v;
	double	projected_length;

	//Calculate interesection point : P = o + td;
	intersection = vec3_add(ray.origin, vec3_scale(ray.direction, t));

	//Calculate vector from cylinder center to the intersection point : v = P - C
	v = vec3_subtract(intersection, cylinder.center);

	// Project this vector into the axis : proj = v.A
	projected_length = vec3_dot(v, cylinder.axis);

	if (fabs(projected_length) <= cylinder.height / 2.0)
		return (1);
	else
		return (0);
}

int	ray_cylinder_caps_intersect(t_ray ray, t_cylinder cylinder, double *t)
{
	double	t_min;
	int		hit;
	t_vec3	top_center;
	t_vec3	bottom_center;
	double	denom;
	double	t_top;
	double	t_bottom;
	t_vec3	hit_point;

	t_min = INFINITY;
	hit = 0;

	//Calculate cylinder top center and bottom center
	top_center = vec3_add(cylinder.center, vec3_scale(cylinder.axis, cylinder.height / 2.0));
	bottom_center = vec3_add(cylinder.center, vec3_scale(cylinder.axis, -cylinder.height / 2.0));

	//Check top cap
	denom = vec3_dot(ray.direction, cylinder.axis); //dot product between ray dir and cap normal
	if (fabs(denom) > 0.001) //Not parallel
	{
		//Find t from the ray plane intersection formula: t = ((Po - o).N) / (d.N)
		//Po = any point on the plane, in this case cylinder top center
		//o = ray origin
		//N = plane normal, in this case the cylinder axis
		//d = ray direction
		//denom already = d.N
		t_top = vec3_dot(vec3_subtract(top_center, ray.origin), cylinder.axis) / denom;
		if (t_top > 0.001)
		{
			//Check if the ray intersect within the boundary of top cap
			//Calculate hit point - hit = o + td;
			hit_point = vec3_add(ray.origin, vec3_scale(ray.direction, t_top));
			//Check if the distance from this hit to the center of the cap is < radius
			if (vec3_length(vec3_subtract(hit_point, top_center)) <= cylinder.radius)
			{
				t_min = t_top;
				hit = 1;
			}
		}
		//Check bottom cap
		t_bottom = vec3_dot(vec3_subtract(bottom_center, ray.origin), cylinder.axis) / denom;
		if (t_bottom > 0.001 && t_bottom < t_min)
		{
			hit_point = vec3_add(ray.origin, vec3_scale(ray.direction, t_bottom));
			if (vec3_length(vec3_subtract(hit_point, bottom_center)) <= cylinder.radius)
			{
				t_min = t_bottom;
				hit = 1;
			}
		}
	}
	if (hit)
	{
		*t = t_min;
		return (1);
	}
	return (0);
}

int	ray_cylinder_intersect(t_ray ray, t_cylinder cylinder, double *t)
{
	double	a;
	double	b;
	double	c;
	double	discriminant;
	t_vec3	oc;

	//calculate vector from sphere center to ray origin
	oc = vec3_subtract(ray.origin, cylinder.center);

	//calculate all needed coefficient for quadratic equation (at^2 + bt + c = 0)
	a = vec3_dot(ray.direction, ray.direction) - pow(vec3_dot(ray.direction, cylinder.axis), 2);
	b = 2 * (vec3_dot(ray.direction, oc) - (vec3_dot(ray.direction, cylinder.axis)) * \
			vec3_dot(oc, cylinder.axis));
	c = vec3_dot(oc, oc) - pow(vec3_dot(oc, cylinder.axis), 2) - cylinder.radius * cylinder.radius;

	//calculate the discrimant to know if equation has solutions
	discriminant = b * b - (4 * a * c);

	if (discriminant < 0)
		return (0); //no real solution

	// Find the nearest solution
	double t1 = (-b - sqrt(discriminant)) / (2.0 * a);
	double t2 = (-b + sqrt(discriminant)) / (2.0 * a);
	double t_min = fmin(t1, t2);
	double t_max = fmax(t1, t2);

	////Check if intersection point is within cylinder height
	if (t_min > 0.001)
	{
		if (is_within_cylinder_height(ray, cylinder, t_min))
		{
			*t = t_min;
			return (1);
		}
	}
	if (t_max > 0.001)
	{
		if (is_within_cylinder_height(ray, cylinder, t_max))
		{
			*t = t_max;
			return (1);
		}
	}

	//Check for cap intersections
	double t_cap = INFINITY;
	if (ray_cylinder_caps_intersect(ray, cylinder, &t_cap) && t_cap < *t)
	{
		*t = t_cap;
		return (1);
	}
	return (0);
}

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
		else if (current->type == CYLINDER)
		{
			t_cylinder *cylinder = (t_cylinder *)(current->data);
			if (ray_cylinder_intersect(ray, *cylinder, &t_temp) && t_temp < t_closest)
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
