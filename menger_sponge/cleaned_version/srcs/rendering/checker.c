/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 16:20:01 by abillote          #+#    #+#             */
/*   Updated: 2025/05/27 11:08:17 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "miniRT.h"
#include "platform.h"
#include <math.h>



/**
 * Gets the appropriate color at a point for a material with potential checkerboard
 *
 * @param material The material properties
 * @param point The intersection point
 * @return The color at that point
 */
t_color get_checker_color(t_material material, t_object *object, t_vec3 point)
{
	// If this material doesn't have a checkerboard pattern, just return the primary color
	if (!material.has_checker)
		return material.color;

	// Choose the appropriate checker pattern function based on object type
	if (object->type == PLANE)
	{
		t_plane *plane = (t_plane *)object->data;
		if (is_checker_point_plane(*plane, point, material.checker_size))
			return material.checker_color;
		else
			return material.color;
	}
	else
	{
		// For other objects, use the general 3D checker pattern
		if (is_checker_point(point, material.checker_size))
			return material.checker_color;
		else
			return material.color;
	}
}

/**
 * Creates a checkerboard pattern specifically for planes
 *
 * @param plane The plane definition
 * @param point The point to check
 * @param checker_size Size of the checker squares
 * @return 1 if should use checker color, 0 if should use primary color
 */
int is_checker_point_plane(t_plane plane, t_vec3 point, double checker_size)
{
	t_vec3 normal = plane.normal;
	t_vec3 u_axis, v_axis;

	// Find two basis vectors that form a coordinate system on the plane
	// First, find one vector perpendicular to the normal
	if (fabs(normal.z) > 0.1 || fabs(normal.y) > 0.1)
		u_axis = vec3_normalize(vec3_cross(normal, vec3_create(1, 0, 0)));
	else
		u_axis = vec3_normalize(vec3_cross(normal, vec3_create(0, 1, 0)));

	// Find a second vector perpendicular to both normal and u_axis
	v_axis = vec3_normalize(vec3_cross(normal, u_axis));

	// Project the point onto the plane's coordinate system
	// First, get a vector from the plane point to our hit point
	t_vec3 to_point = vec3_subtract(point, plane.point);

	// Project onto the two axes
	double u = vec3_dot(to_point, u_axis) / checker_size;
	double v = vec3_dot(to_point, v_axis) / checker_size;

	// Determine the checker pattern
	int iu = (int)floor(u);
	int iv = (int)floor(v);

	return ((iu + iv) % 2 == 0) ? 1 : 0;
}

/**
 * Determines whether a point should use the primary or checker color
 *
 * @param point The intersection point in 3D space
 * @param checker_size The size of each checker square
 * @return 1 if should use checker color, 0 if should use primary color
 */
int is_checker_point(t_vec3 point, double checker_size)
{
	// Scale the point by the checker size
	double scale = 1.0 / checker_size;
	double x = point.x * scale;
	double y = point.y * scale;
	double z = point.z * scale;

	// Get the integer parts
	int ix = (int)floor(x);
	int iy = (int)floor(y);
	int iz = (int)floor(z);

	// The sum of the integer parts being even or odd determines the pattern
	// This creates a 3D checkerboard pattern
	return ((ix + iy + iz) % 2 == 0) ? 1 : 0;
}

/**
 * Determines whether a point should use the primary or checker color
 * for a 2D checkerboard pattern
 *
 * @param u The u coordinate in 2D space
 * @param v The v coordinate in 2D space
 * @param checker_size The size of each checker square
 * @return 1 if should use checker color, 0 if should use primary color
 */
int	is_checker_point_2d(double u, double v, double checker_size)
{
	u = floor(u / checker_size);
	v = floor(v / checker_size);
	if (((int)u + (int)v) % 2 == 0)
		return 1; // Use checker color
	else
		return 0; // Use primary color
}

t_vec2 spherical_map(t_vec3 point)
{
	double theta;
	double radius;
	double phi;
	double u;
	double v;

	theta = atan2(point.x, point.z);
	radius = vec3_length(point);
	if (radius == 0)
		return ((t_vec2){0, 0});
	phi = acos(point.y / radius);
	u = theta / (2 * M_PI);
	u = 1 - (u + 0.5);
	v = 1 - phi / M_PI;
	return ((t_vec2){u, v});
}
