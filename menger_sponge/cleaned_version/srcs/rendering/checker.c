/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checker.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 16:20:01 by abillote          #+#    #+#             */
/*   Updated: 2025/05/27 16:11:30 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"


//TO DO - Make a function calling specific texture functions based on object type.
/**
 * Gets the appropriate color at a point for a material with potential checkerboard
 *
 * @param material The material properties
 * @param point The intersection point
 * @return The color at that point
 */
t_color get_checker_color(t_material material, t_object *object, t_vec3 point)
{
	t_vec2	point_2d;
	// If this material doesn't have a checkerboard pattern, just return the primary color
	if (!material.has_checker)
		return material.color;

	// Choose the appropriate checker pattern function based on object type
	if (object->type == PLANE)
	{
		t_plane *plane = (t_plane *)object->data;
		point_2d = planar_map(plane, point);
	}
	else if (object->type == SPHERE)
	{
		t_sphere *sphere = (t_sphere *)object->data;
		// Convert point to sphere local coordinates before mapping
		t_vec3 sphere_local_point = vec3_subtract(point, sphere->center);
		point_2d = spherical_map(sphere_local_point);
	}
	else if (object->type == CYLINDER)
	{
		t_cylinder *cylinder = (t_cylinder *)object->data;
		t_vec3 cylinder_local_point = vec3_subtract(point, cylinder->center);
		point_2d = cylindrical_map(cylinder_local_point, cylinder->radius, cylinder->axis);
	}
	else if (object->type == CUBE)
	{
		t_cube *cube = (t_cube *)object->data;
		point_2d =  cubic_map(cube, point);
	}
	else
	{
		point_2d = spherical_map(point); // Fallback for other objects
	}
	if (is_checker_point_2d(point_2d.u, point_2d.v, material.checker_size))
		return material.checker_color;
	else
		return material.color;
}

t_vec2 planar_map(t_plane *plane, t_vec3 point)
{
	t_vec3 normal = plane->normal;
	t_vec3 u_axis, v_axis;

	// Find two basis vectors that form a coordinate system on the plane
	if (fabs(normal.z) > 0.1 || fabs(normal.y) > 0.1)
		u_axis = vec3_normalize(vec3_cross(normal, vec3_create(1, 0, 0)));
	else
		u_axis = vec3_normalize(vec3_cross(normal, vec3_create(0, 1, 0)));

	v_axis = vec3_normalize(vec3_cross(normal, u_axis));

	// Project the point onto the plane's coordinate system
	t_vec3 to_point = vec3_subtract(point, plane->point);

	// Project onto the two axes and apply a consistent scaling factor
	// This ensures the checkerboard pattern has a consistent size
	// regardless of the plane's orientation or position
	double scale = 1.0; // Base scale for consistent size across all planes
	double u = vec3_dot(to_point, u_axis) * scale;
	double v = vec3_dot(to_point, v_axis) * scale;

	return ((t_vec2){u, v});
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
	// Scale u and v by checker_size
	double scaled_u = u * (2.0 / checker_size);
	double scaled_v = v * (2.0 / checker_size);

	// Floor to get integer coordinates
	int cell_u = (int)floor(scaled_u);
	int cell_v = (int)floor(scaled_v);

	// Check if the sum is even or odd
	return (cell_u + cell_v) % 2;
}

t_vec2 spherical_map(t_vec3 point)
{
	double theta;
	double phi;
	double u;
	double v;

	// Normalize the point to get it on unit sphere
	point = vec3_normalize(point);

	// Calculate spherical coordinates
	theta = atan2(point.x, point.z);
	phi = acos(point.y);  // acos(y) gives us phi directly since point is normalized

	// Convert to UV coordinates [0, 1]
	u = theta / (2 * M_PI);
	u = u + 0.5;  // Shift to [0, 1] range
	if (u < 0) u += 1.0;  // Handle negative theta values
	if (u > 1) u -= 1.0;

	v = phi / M_PI;  // phi ranges from 0 to π, so this gives [0, 1]

	// Divide to create square checkers instead of rectangular ones
	// Since theta spans 2π and phi spans π, we need to divide the v coordinate
	// to make the checkers square
	v /= 2.0;

	return ((t_vec2){u, v});
}

t_vec2 cylindrical_map(t_vec3 point, double radius, t_vec3 axis)
{
	t_vec2 uv;
	t_vec3 u_axis, v_axis;

	// Normalize the axis to be safe
	axis = vec3_normalize(axis);

	// Find two basis vectors perpendicular to the cylinder axis
	if (fabs(axis.z) > 0.1 || fabs(axis.y) > 0.1)
		u_axis = vec3_normalize(vec3_cross(axis, vec3_create(1, 0, 0)));
	else
		u_axis = vec3_normalize(vec3_cross(axis, vec3_create(0, 1, 0)));

	v_axis = vec3_normalize(vec3_cross(axis, u_axis));

	// Project the point onto the plane perpendicular to the axis
	double u_proj = vec3_dot(point, u_axis);
	double v_proj = vec3_dot(point, v_axis);

	// Calculate the angle around the cylinder
	double theta = atan2(v_proj, u_proj);

	// Map theta to the u coordinate [0, 1]
	uv.u = (theta + M_PI) / (2.0 * M_PI);

	// Calculate height along the cylinder axis
	double height = vec3_dot(point, axis);

	// Scale height to match circumference for square checkers
	// The circumference is 2π*r, so we scale by 1/(2π*r)
	double height_scale = 1.0 / (2.0 * M_PI * radius);
	uv.v = height * height_scale;

	return uv;
}

t_vec2 cubic_map(t_cube *cube, t_vec3 point)
{
	// Convert to cube local coordinates
	t_vec3 local_point = vec3_subtract(point, cube->center);

	// Find which face of the cube we're on by determining the largest component
	double abs_x = fabs(local_point.x);
	double abs_y = fabs(local_point.y);
	double abs_z = fabs(local_point.z);

	double u, v;
	double half_size = cube->side_length / 2.0;

	// Determine which face of the cube we're on and map accordingly
	if (abs_x >= abs_y && abs_x >= abs_z) {
		// X-faces (right or left)
		int sign = (local_point.x > 0) ? 1 : -1;
		u = (local_point.z * sign) / half_size * 0.5 + 0.5;
		v = (local_point.y) / half_size * 0.5 + 0.5;
	}
	else if (abs_y >= abs_x && abs_y >= abs_z) {
		// Y-faces (top or bottom)
		int sign = (local_point.y > 0) ? 1 : -1;
		u = (local_point.x) / half_size * 0.5 + 0.5;
		v = (local_point.z * sign) / half_size * 0.5 + 0.5;
	}
	else {
		// Z-faces (front or back)
		int sign = (local_point.z > 0) ? 1 : -1;
		u = (local_point.x * -sign) / half_size * 0.5 + 0.5;
		v = (local_point.y) / half_size * 0.5 + 0.5;
	}

	return ((t_vec2){u, v});
}

/**
 * Fallback for other objects
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
