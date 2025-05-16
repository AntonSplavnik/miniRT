///* ************************************************************************** */
///*                                                                            */
///*                                                        :::      ::::::::   */
///*   objects_intersections.c                            :+:      :+:    :+:   */
///*                                                    +:+ +:+         +:+     */
///*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
///*                                                +#+#+#+#+#+   +#+           */
///*   Created: 2025/04/30 11:16:07 by abillote          #+#    #+#             */
///*   Updated: 2025/05/08 13:00:40 by abillote         ###   ########.fr       */
///*                                                                            */
///* ************************************************************************** */

#include "platform.h"

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
	double	t1;
	double	t2;

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
			hit_body = 0;
			t_body = INFINITY;
			if (t1 > 0.0001)
			{

			}
		}
	}



}


t_vec3	cone_normal_at_point(t_vec3 point, t_cone cone)
{



}
