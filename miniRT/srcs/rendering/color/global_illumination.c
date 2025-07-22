/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   global_illumination.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/22 17:00:00 by antonsplavn       #+#    #+#             */
/*   Updated: 2025/07/22 23:08:26 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/miniRT.h"

double	gi_random(void)
{
	static unsigned int seed = 123456789;
	seed = seed * 1664525 + 1013904223;
	return ((double)(seed >> 16) / 65536.0);
}

t_ray	create_ray(t_vec3 origin, t_vec3 direction)
{
	t_ray	ray;

	ray.origin = origin;
	ray.direction = vec3_normalize(direction);
	ray.is_gi_ray = 0;
	return (ray);
}

t_ray	create_gi_ray(t_vec3 origin, t_vec3 direction)
{
	t_ray	ray;

	ray.origin = origin;
	ray.direction = vec3_normalize(direction);
	ray.is_gi_ray = 1;
	return (ray);
}

t_vec3	sample_hemisphere_cosine(t_vec3 normal)
{
	double	r1;
	double	r2;
	double	cos_theta;
	double	sin_theta;
	double	phi;

	r1 = gi_random();
	r2 = gi_random();
	cos_theta = sqrt(r1);
	sin_theta = sqrt(1.0 - r1);
	phi = 2.0 * M_PI * r2;
	return (hemisphere_to_world(vec3_create(sin_theta * cos(phi), \
		sin_theta * sin(phi), cos_theta), normal));
}

t_vec3	hemisphere_to_world(t_vec3 sample, t_vec3 normal)
{
	t_vec3	up;
	t_vec3	tangent;
	t_vec3	bitangent;

	up = vec3_create(0, 1, 0);
	if (fabs(vec3_dot(normal, up)) > 0.9)
		up = vec3_create(1, 0, 0);
	tangent = vec3_normalize(vec3_cross(up, normal));
	bitangent = vec3_cross(normal, tangent);
	return (vec3_add(vec3_add(vec3_scale(tangent, sample.x), \
		vec3_scale(bitangent, sample.y)), vec3_scale(normal, sample.z)));
}

t_vec3	compute_indirect_lighting(t_scene *scene, t_hit_record hit_record, \
									int depth)
{
	s_global_illumination	*gi;
	t_vec3	indirect_color;
	t_vec3	sample_direction;
	t_ray	indirect_ray;
	t_vec3	sample_contribution;
	int						i;

	gi = &scene->gi;
	if (depth >= gi->max_depth)
		return (vec3_create(0, 0, 0));
	indirect_color = vec3_create(0, 0, 0);
	i = 0;
	while (i < gi->samples_per_bounce)
	{
		sample_direction = sample_hemisphere_cosine(hit_record.normal);
		indirect_ray = create_gi_ray(vec3_add(hit_record.point, \
			vec3_scale(hit_record.normal, 0.001)), sample_direction);
		sample_contribution = trace_ray(scene, indirect_ray, depth + 1);
		indirect_color = vec3_add(indirect_color, sample_contribution);
		i++;
	}
	return (vec3_scale(indirect_color, 1.0 / gi->samples_per_bounce));
}
