/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_uv_mapping.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 11:07:08 by abillote          #+#    #+#             */
/*   Updated: 2025/06/04 12:27:20 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

// Calculate UV coordinates for a sphere
t_vec2 sphere_uv_mapping(t_vec3 point, t_sphere sphere)
{
	t_vec2 uv;
	t_vec3 local_point;
	double theta, phi;

	// Convert to sphere local coordinates (relative to center)
	local_point = vec3_subtract(point, sphere.center);
	local_point = vec3_normalize(local_point);

	// Calculate spherical coordinates
	theta = atan2(local_point.x, local_point.z);
	phi = acos(-local_point.y);  // Note: negative y for proper orientation

	// Convert to UV coordinates [0, 1]
	uv.u = 0.5 + theta / (2.0 * M_PI);
	uv.v = phi / M_PI;

	// Ensure UV coordinates are in [0, 1] range
	if (uv.u < 0) uv.u += 1.0;
	if (uv.u > 1) uv.u -= 1.0;

	uv.v = 1.0 - uv.v; // Invert V coordinate for texture mapping
	uv.u = 1.0 - uv.u;

	return uv;
}

// Calculate UV coordinates for a plane
t_vec2 plane_uv_mapping(t_vec3 point, t_plane plane)
{
	t_vec2 uv;
	t_vec3 u_axis, v_axis;
	t_vec3 to_point;

	// Create two basis vectors perpendicular to the plane normal
	// Choose the most stable basis vectors based on the normal direction
	if (fabs(plane.normal.z) > 0.9)
	{
		// Normal is mostly along Z, use X and Y
		u_axis = vec3_create(1, 0, 0);
		v_axis = vec3_create(0, 1, 0);
	}
	else if (fabs(plane.normal.y) > 0.9)
	{
		// Normal is mostly along Y, use X and Z
		u_axis = vec3_create(1, 0, 0);
		v_axis = vec3_create(0, 0, 1);
	}
	else
	{
		// Normal is mostly along X, use Y and Z
		u_axis = vec3_create(0, 1, 0);
		v_axis = vec3_create(0, 0, 1);
	}

	// Make sure basis vectors are orthogonal to the normal
	u_axis = vec3_normalize(vec3_subtract(u_axis, vec3_scale(plane.normal, vec3_dot(u_axis, plane.normal))));
	v_axis = vec3_normalize(vec3_cross(plane.normal, u_axis));

	// Project the point onto the plane's coordinate system
	to_point = vec3_subtract(point, plane.point);

	// Calculate UV coordinates (scale can be adjusted for texture tiling)
	double scale = 0.1; // Adjust this value to control texture scale
	uv.u = vec3_dot(to_point, u_axis) * scale;
	uv.v = vec3_dot(to_point, v_axis) * scale;

	// Wrap UV coordinates (for repeating textures)
	uv.u = uv.u - floor(uv.u);
	uv.v = uv.v - floor(uv.v);

	return uv;
}

// Calculate UV coordinates for a cylinder
t_vec2 cylinder_uv_mapping(t_vec3 point, t_cylinder cylinder)
{
	t_vec2 uv;
	t_vec3 local_point;
	t_vec3 u_axis, v_axis;
	double theta, height_coord;

	// Convert to cylinder local coordinates
	local_point = vec3_subtract(point, cylinder.center);

	// Create basis vectors perpendicular to cylinder axis
	if (fabs(cylinder.axis.z) > 0.1 || fabs(cylinder.axis.y) > 0.1)
		u_axis = vec3_normalize(vec3_cross(cylinder.axis, vec3_create(1, 0, 0)));
	else
		u_axis = vec3_normalize(vec3_cross(cylinder.axis, vec3_create(0, 1, 0)));

	v_axis = vec3_normalize(vec3_cross(cylinder.axis, u_axis));

	// Calculate cylindrical coordinates
	double u_proj = vec3_dot(local_point, u_axis);
	double v_proj = vec3_dot(local_point, v_axis);

	// Calculate angle around cylinder
	theta = atan2(v_proj, u_proj);
	uv.u = (theta + M_PI) / (2.0 * M_PI);

	// Calculate height coordinate
	height_coord = vec3_dot(local_point, cylinder.axis);
	uv.v = (height_coord + cylinder.height / 2.0) / cylinder.height;

	// Clamp V coordinate to [0, 1]
	if (uv.v < 0) uv.v = 0;
	if (uv.v > 1) uv.v = 1;

	return uv;
}

// Calculate UV coordinates for a cube
t_vec2 cube_uv_mapping(t_vec3 point, t_cube cube)
{
	t_vec2 uv;
	t_vec3 local_point;
	double abs_x, abs_y, abs_z;
	double half_size = cube.side_length / 2.0;

	// Convert to cube local coordinates
	local_point = vec3_subtract(point, cube.center);

	// Find which face we're on
	abs_x = fabs(local_point.x);
	abs_y = fabs(local_point.y);
	abs_z = fabs(local_point.z);

	if (abs_x >= abs_y && abs_x >= abs_z)
	{
		// X-faces (left or right)
		if (local_point.x > 0)
		{
			// Right face (+X)
			uv.u = (-local_point.z / half_size + 1.0) / 2.0;
			uv.v = (-local_point.y / half_size + 1.0) / 2.0;
		}
		else
		{
			// Left face (-X)
			uv.u = (local_point.z / half_size + 1.0) / 2.0;
			uv.v = (-local_point.y / half_size + 1.0) / 2.0;
		}
	}
	else if (abs_y >= abs_x && abs_y >= abs_z)
	{
		// Y-faces (top or bottom)
		if (local_point.y > 0)
		{
			// Top face (+Y)
			uv.u = (local_point.x / half_size + 1.0) / 2.0;
			uv.v = (local_point.z / half_size + 1.0) / 2.0;
		}
		else
		{
			// Bottom face (-Y)
			uv.u = (local_point.x / half_size + 1.0) / 2.0;
			uv.v = (-local_point.z / half_size + 1.0) / 2.0;
		}
	}
	else
	{
		// Z-faces (front or back)
		if (local_point.z > 0)
		{
			// Front face (+Z)
			uv.u = (local_point.x / half_size + 1.0) / 2.0;
			uv.v = (-local_point.y / half_size + 1.0) / 2.0;
		}
		else
		{
			// Back face (-Z)
			uv.u = (-local_point.x / half_size + 1.0) / 2.0;
			uv.v = (-local_point.y / half_size + 1.0) / 2.0;
		}
	}

	return uv;
}

// Main UV mapping function that dispatches to the correct mapping based on object type
t_vec2 calculate_uv_coordinates(t_vec3 point, t_object *object)
{
	t_vec2 uv = {0.0, 0.0}; // Default UV

	switch (object->type)
	{
		case SPHERE:
			uv = sphere_uv_mapping(point, *(t_sphere *)object->data);
			break;
		case PLANE:
			uv = plane_uv_mapping(point, *(t_plane *)object->data);
			break;
		case CYLINDER:
			uv = cylinder_uv_mapping(point, *(t_cylinder *)object->data);
			break;
		case CUBE:
			uv = cube_uv_mapping(point, *(t_cube *)object->data);
			break;
		default:
			// For unsupported objects, use a simple planar mapping
			uv.u = point.x * 0.1;
			uv.v = point.y * 0.1;
			break;
	}

	return uv;
}
