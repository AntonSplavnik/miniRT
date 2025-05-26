///* ************************************************************************** */
///*	                                                                */
///*                                                        :::      ::::::::   */
///*   objects_intersections.c                            :+:      :+:    :+:   */
///*                                                    +:+ +:+         +:+     */
///*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
///*                                                +#+#+#+#+#+   +#+           */
///*   Created: 2025/04/30 11:16:07 by abillote          #+#    #+#             */
///*   Updated: 2025/05/08 13:00:40 by abillote         ###   ########.fr       */
///*                                                                            */
///* ************************************************************************** */

#include "../../../includes/miniRT.h"

int ray_cone_intersect(t_ray ray, t_cone cone, double *t)
{
	t_vec3	base_center;
	t_vec3	apex_to_origin;
	double	cos_squared;
	double	dot_dir_axis;
	double	dot_origin_axis;
	double	a;
	double	b;
	double	c;
	double	discriminant;
	double	t_body;
	int		hit_body;
	int		hit_base;
	double	t1;
	double	t2;
	double	t_base;

	//Calculate base center: apex + axis * heigth
	base_center = vec3_add(cone.apex, vec3_scale(cone.axis, cone.height));

	//Vector from apex to ray origin
	apex_to_origin = vec3_subtract(ray.origin, cone.apex);

	//Precompute cos squared
	cos_squared = cos(cone.angle) * cos(cone.angle);

	//Calculate dot product between ray direction and axis > needed for coefficient a and b
	dot_dir_axis = vec3_dot(ray.direction, cone.axis);

	//Calculate dot product between apex to origin vector and axis > needed for coefficient b and c
	dot_origin_axis = vec3_dot(apex_to_origin, cone.axis);

	//Calculate quadratic coefficient
	//a = (D.axis)^2 - cos^2θ(D.D)
	a = dot_dir_axis * dot_dir_axis - cos_squared * vec3_dot(ray.direction, ray.direction);

	//b = 2[(OA.axis)(D.axis) - cos^2θ(OA.D)]
	b = 2.0 * (dot_origin_axis * dot_dir_axis - cos_squared * (vec3_dot(apex_to_origin, ray.direction)));

	//c = (OA.axis)^2 - cos^2θ(OA.OA)
	c = dot_origin_axis * dot_origin_axis - cos_squared * vec3_dot(apex_to_origin, apex_to_origin);

	 // Check for degenerate cases (ray parallel to cone surface)
	if (fabs(a) < 0.0001)
	{
		// If b is also close to zero, no intersection with cone
		if (fabs(b) < 0.0001)
			return 0;

		// Linear case: one intersection point
		t_body = -c / b;
		hit_body = (t_body > 0.0001);
	}
	else
	{
		//solve quadratic equation
		discriminant = b * b - 4 * a * c;

		// No real solutions = no intersection
		if (discriminant < 0)
			hit_body = 0;
		else
		{
			//calculate both intersection point
			t1 = (-b - sqrt(discriminant)) / (2.0 * a);
			t2 = (-b + sqrt(discriminant)) / (2.0 * a);

			 // Check if intersections are valid (positive t and within height)
			hit_body = 0;
			t_body = INFINITY;

			// Check first intersection point
			if (t1 > 0.0001)
			{
				t_vec3 hit = vec3_add(ray.origin, vec3_scale(ray.direction, t1));
				t_vec3 hit_to_apex = vec3_subtract(hit, cone.apex);
				double projection = vec3_dot(hit_to_apex, cone.axis);

				// Check if intersection is within cone height and not behind apex
				if (projection > 0 && projection < cone.height)
				{
					// Additional check: is this point inside the cone radius at this height?
					double radius_at_height = (projection / cone.height) * cone.radius;
					t_vec3 point_on_axis = vec3_add(cone.apex, vec3_scale(cone.axis, projection));
					double dist_squared = vec3_length_squared(vec3_subtract(hit, point_on_axis));

					if (dist_squared <= radius_at_height * radius_at_height * 1.01) // 1% tolerance
					{
				hit_body = 1;
				t_body = t1;
					}
				}
			}

			// Check second intersection point if needed
			if (t2 > 0.0001 && (t_body == INFINITY || t2 < t_body))
			{
				t_vec3 hit = vec3_add(ray.origin, vec3_scale(ray.direction, t2));
				t_vec3 hit_to_apex = vec3_subtract(hit, cone.apex);
				double projection = vec3_dot(hit_to_apex, cone.axis);

				// Check if intersection is within cone height and not behind apex
				if (projection > 0 && projection < cone.height)
				{
					// Additional check: is this point inside the cone radius at this height?
					double radius_at_height = (projection / cone.height) * cone.radius;
					t_vec3 point_on_axis = vec3_add(cone.apex, vec3_scale(cone.axis, projection));
					double dist_squared = vec3_length_squared(vec3_subtract(hit, point_on_axis));

					if (dist_squared <= radius_at_height * radius_at_height * 1.01) // 1% tolerance
					{
						hit_body = 1;
						t_body = t2;
					}
				}
			}
		}
	}
	 // Check for intersection with the base
	hit_base = ray_disc_intersect(ray, base_center, cone.axis, cone.radius, &t_base);

	// Find closest valid intersection
	if (hit_body && hit_base)
	{
		if (t_body < t_base)
			*t = t_body;
		else
			*t = t_base;
		return 1;
	}
	else if (hit_body)
	{
		*t = t_body;
		return 1;
	}
	else if (hit_base)
	{
		*t = t_base;
		return 1;
	}
	return 0;
}


	/*
** cone_normal_at_point: Calculate the normal vector at a point on a cone
**
** The normal at any point on the cone's surface depends on:
** 1. Whether the point is on the base (flat circular disc)
** 2. Or on the conical surface
**
** For conical surface, the normal is perpendicular to both:
** - The vector from apex to the point
** - The tangent vector around the cone at that point
*/
t_vec3 cone_normal_at_point(t_vec3 point, t_cone cone)
{
	t_vec3 base_center;

	// Calculate base center
	base_center = vec3_add(cone.apex, vec3_scale(cone.axis, cone.height));

	// Calculate base radius
	double base_radius = cone.height * tan(cone.angle);

	// Check if point is on the base
	if (is_point_in_disc(point, base_center, cone.axis, base_radius))
		return cone.axis; // Normal is the same as cone axis (pointing outward)

	// Otherwise, point is on the conical surface
	// For a point on the cone surface, the normal is perpendicular to the surface
	// First, get the vector from apex to the point
	t_vec3 apex_to_point = vec3_subtract(point, cone.apex);

	// Project this vector onto the cone axis to find a point on the axis
	double projection = vec3_dot(apex_to_point, cone.axis);
	t_vec3 axis_point = vec3_add(cone.apex, vec3_scale(cone.axis, projection));

	// The normal is perpendicular to the cone surface
	// For a cone, it's the vector from the point to the axis, adjusted by the cone angle
	t_vec3 point_to_axis = vec3_subtract(axis_point, point);

	// The normal should be perpendicular to both the axis and the tangent at that point
	t_vec3 tangent = vec3_cross(cone.axis, point_to_axis);
	t_vec3 normal = vec3_cross(tangent, point_to_axis);

	return vec3_normalize(normal);
}
