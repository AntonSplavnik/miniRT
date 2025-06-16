/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bumpmap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abillote <abillote@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/03 11:09:53 by abillote          #+#    #+#             */
/*   Updated: 2025/06/05 10:41:45 by abillote         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/miniRT.h"

// Sample bump map height at UV coordinates
float sample_bump_map(t_bump_map *bump_map, double u, double v)
{
	if (!bump_map || !bump_map->height_data)
		return 0.0f;

	// Wrap UV coordinates
	u = u - floor(u);
	v = v - floor(v);
	if (u < 0) u += 1.0;
	if (v < 0) v += 1.0;

	// Convert UV to continuous pixel coordinates
	double px = u * (bump_map->width - 1);
	double py = v * (bump_map->height - 1);

	// Get integer coordinates and fractional parts
	int x0 = (int)floor(px);
	int y0 = (int)floor(py);
	int x1 = x0 + 1;
	int y1 = y0 + 1;

	double fx = px - x0; // Fractional part
	double fy = py - y0;

	// Clamp coordinates
	x0 = x0 < 0 ? 0 : (x0 >= bump_map->width ? bump_map->width - 1 : x0);
	y0 = y0 < 0 ? 0 : (y0 >= bump_map->height ? bump_map->height - 1 : y0);
	x1 = x1 < 0 ? 0 : (x1 >= bump_map->width ? bump_map->width - 1 : x1);
	y1 = y1 < 0 ? 0 : (y1 >= bump_map->height ? bump_map->height - 1 : y1);

	// Sample four neighboring heights
	float h00 = bump_map->height_data[y0 * bump_map->width + x0];
	float h10 = bump_map->height_data[y0 * bump_map->width + x1];
	float h01 = bump_map->height_data[y1 * bump_map->width + x0];
	float h11 = bump_map->height_data[y1 * bump_map->width + x1];

	// Bilinear interpolation
	float h_top = h00 * (1 - fx) + h10 * fx;
	float h_bottom = h01 * (1 - fx) + h11 * fx;
	float final_height = h_top * (1 - fy) + h_bottom * fy;

	return final_height;
}

// Calculate tangent and bitangent vectors for bump mapping
void calculate_tangent_vectors(t_vec3 normal, t_vec3 *tangent, t_vec3 *bitangent)
{
	// Choose an arbitrary vector that's not parallel to the normal
	t_vec3 arbitrary;
	if (fabs(normal.x) > 0.9)
		arbitrary = vec3_create(0, 1, 0);
	else
		arbitrary = vec3_create(1, 0, 0);

	// Calculate tangent (perpendicular to normal)
	*tangent = vec3_normalize(vec3_cross(normal, arbitrary));

	// Calculate bitangent (perpendicular to both normal and tangent)
	*bitangent = vec3_normalize(vec3_cross(normal, *tangent));
}

// Sphere-specific tangent vector calculation
void calculate_sphere_tangent_vectors(t_hit_record *hit_record, t_vec3 *tangent, t_vec3 *bitangent)
{
	t_sphere *sphere = (t_sphere *)hit_record->object->data;
	t_vec3 local_point = vec3_subtract(hit_record->point, sphere->center);
	local_point = vec3_normalize(local_point);

	// For spheres, tangent follows longitude lines, bitangent follows latitude lines
	double theta = atan2(local_point.x, local_point.z);

	// Tangent vector (along longitude)
	tangent->x = cos(theta);
	tangent->y = 0;
	tangent->z = -sin(theta);
	*tangent = vec3_normalize(*tangent);

	// Bitangent vector (along latitude)
	*bitangent = vec3_normalize(vec3_cross(hit_record->normal, *tangent));
}

// Plane-specific tangent vector calculation
void calculate_plane_tangent_vectors(t_hit_record *hit_record, t_vec3 *tangent, t_vec3 *bitangent)
{
	t_plane *plane = (t_plane *)hit_record->object->data;

	// Use the same basis vectors as UV mapping for consistency
	if (fabs(plane->normal.z) > 0.9)
	{
		*tangent = vec3_create(1, 0, 0);
		*bitangent = vec3_create(0, 1, 0);
	}
	else if (fabs(plane->normal.y) > 0.9)
	{
		*tangent = vec3_create(1, 0, 0);
		*bitangent = vec3_create(0, 0, 1);
	}
	else
	{
		*tangent = vec3_create(0, 1, 0);
		*bitangent = vec3_create(0, 0, 1);
	}

	// Ensure orthogonality to the normal
	*tangent = vec3_normalize(vec3_subtract(*tangent, vec3_scale(plane->normal, vec3_dot(*tangent, plane->normal))));
	*bitangent = vec3_normalize(vec3_cross(plane->normal, *tangent));
}

// Cylinder-specific tangent vector calculation
void calculate_cylinder_tangent_vectors(t_hit_record *hit_record, t_vec3 *tangent, t_vec3 *bitangent)
{
	t_cylinder *cylinder = (t_cylinder *)hit_record->object->data;
	t_vec3 local_point = vec3_subtract(hit_record->point, cylinder->center);

	// Check if we're on a cap
	double height_projection = vec3_dot(local_point, cylinder->axis);
	double half_height = cylinder->height / 2.0;

	if (fabs(fabs(height_projection) - half_height) < 0.001)
	{
		// We're on a cap, use planar tangent vectors
		calculate_tangent_vectors(hit_record->normal, tangent, bitangent);
	}
	else
	{
		// We're on the cylindrical surface
		// Tangent follows the cylinder axis
		*tangent = cylinder->axis;

		// Bitangent is perpendicular to both axis and radial direction
		t_vec3 radial = vec3_subtract(local_point, vec3_scale(cylinder->axis, height_projection));
		radial = vec3_normalize(radial);
		*bitangent = vec3_normalize(vec3_cross(cylinder->axis, radial));
	}
}

// Cube-specific tangent vector calculation
void calculate_cube_tangent_vectors(t_hit_record *hit_record, t_vec3 *tangent, t_vec3 *bitangent)
{
	t_cube *cube = (t_cube *)hit_record->object->data;
	t_vec3 local_point = vec3_subtract(hit_record->point, cube->center);
	double abs_x = fabs(local_point.x);
	double abs_y = fabs(local_point.y);
	double abs_z = fabs(local_point.z);

	// Determine which face we're on and set appropriate tangent vectors
	if (abs_x >= abs_y && abs_x >= abs_z)
	{
		// X-faces
		*tangent = vec3_create(0, 0, local_point.x > 0 ? -1 : 1);
		*bitangent = vec3_create(0, -1, 0);
	}
	else if (abs_y >= abs_x && abs_y >= abs_z)
	{
		// Y-faces
		*tangent = vec3_create(1, 0, 0);
		*bitangent = vec3_create(0, 0, local_point.y > 0 ? 1 : -1);
	}
	else
	{
		// Z-faces
		*tangent = vec3_create(local_point.z > 0 ? 1 : -1, 0, 0);
		*bitangent = vec3_create(0, -1, 0);
	}

	*tangent = vec3_normalize(*tangent);
	*bitangent = vec3_normalize(*bitangent);
}

t_vec3 calculate_bump_normal(t_hit_record *hit_record)
{
	t_material *material = &hit_record->object->material;

	if (!material->has_bump_map || !material->bump_map)
		return hit_record->normal;

	// Calculate sphere tangent vectors
	t_vec3 tangent, bitangent;
	if (hit_record->object->type == SPHERE)
		calculate_sphere_tangent_vectors(hit_record, &tangent, &bitangent);
	else if (hit_record->object->type == PLANE)
		calculate_plane_tangent_vectors(hit_record, &tangent, &bitangent);
	else if (hit_record->object->type == CYLINDER)
		calculate_cylinder_tangent_vectors(hit_record, &tangent, &bitangent);
	else if (hit_record->object->type == CUBE)
		calculate_cube_tangent_vectors(hit_record, &tangent, &bitangent);
	else
		calculate_tangent_vectors(hit_record->normal, &tangent, &bitangent);

	// Sample heights for gradient calculation
	float du = 1.0f / material->bump_map->width;   // For 4x4: du = 0.25
	float dv = 1.0f / material->bump_map->height;  // For 4x4: dv = 0.25

	float height_center = sample_bump_map(material->bump_map, hit_record->uv.u, hit_record->uv.v);
	float height_u = sample_bump_map(material->bump_map, hit_record->uv.u + du, hit_record->uv.v);
	float height_v = sample_bump_map(material->bump_map, hit_record->uv.u, hit_record->uv.v + dv);

	// Calculate gradients
	float gradient_u = (height_u - height_center) / du;
	float gradient_v = (height_v - height_center) / dv;

	// Scale gradients to control bump strength
	float bump_strength = 0.1f; // Adjust this value to control the bump effect strength
	gradient_u *= bump_strength;
	gradient_v *= bump_strength;

	// Apply gradients to tangent space
	t_vec3 bump_offset;
	bump_offset.x = gradient_u * tangent.x + gradient_v * bitangent.x;
	bump_offset.y = gradient_u * tangent.y + gradient_v * bitangent.y;
	bump_offset.z = gradient_u * tangent.z + gradient_v * bitangent.z;

	// Perturb the normal
	t_vec3 perturbed_normal = vec3_subtract(hit_record->normal, bump_offset);

	return vec3_normalize(perturbed_normal);
}
