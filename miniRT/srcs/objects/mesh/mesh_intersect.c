/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mesh_intersect.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonsplavnik <antonsplavnik@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/10 16:20:00 by abillote          #+#    #+#             */
/*   Updated: 2025/07/03 16:06:01 by antonsplavn      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../includes/miniRT.h"
#include "../../../includes/bvh.h"

///**
// * Finds the closest intersection between a ray and a mesh
// *
// * @param ray The ray to test
// * @param mesh The mesh to test
// * @param t Pointer to store the distance to intersection
// * @param triangle_idx Pointer to store the index of the hit triangle
// * @return 1 if there's an intersection, 0 otherwise
// */
//int	ray_mesh_intersect(t_ray ray, t_mesh mesh, double *t, int *triangle_idx)
//{
//	int		i;
//	double	closest_t;
//	double	current_t;
//	int		hit_something;

//	closest_t = INFINITY;
//	hit_something = 0;
//	*triangle_idx = -1;

//	// Check intersection with each triangle in the mesh
//	for (i = 0; i < mesh.triangle_count; i++)
//	{
//		// Test ray against this triangle
//		if (ray_triangle_intersect(ray, mesh.triangles[i], &current_t))
//		{
//			// Found a closer intersection?
//			if (current_t < closest_t)
//			{
//				closest_t = current_t;
//				*triangle_idx = i;
//				hit_something = 1;
//			}
//		}
//	}

//	if (hit_something)
//	{
//		*t = closest_t;
//		return (1);
//	}
//	return (0);
//}




//New version to rotate properly each triangle in the mesh according to the mesh's rotation
//

void precompute_transformed_triangles(t_mesh *mesh)
{
    printf("Starting precompute_transformed_triangles for %d triangles...\n",
           mesh ? mesh->triangle_count : -1);

    if (!mesh || !mesh->triangles || mesh->triangle_count <= 0)
    {
        printf("Invalid mesh data, returning early\n");
        return;
    }

    // Allocate memory for transformed triangles
    mesh->transformed_tris = malloc(mesh->triangle_count * sizeof(t_transformed_triangle));
    if (!mesh->transformed_tris)
        return; // Handle error

    for (int i = 0; i < mesh->triangle_count; i++) {
        // Scale vertices
        t_vec3 scaled_v0 = vec3_multiply(mesh->triangles[i].v0, mesh->scale);
        t_vec3 scaled_v1 = vec3_multiply(mesh->triangles[i].v1, mesh->scale);
        t_vec3 scaled_v2 = vec3_multiply(mesh->triangles[i].v2, mesh->scale);

        // Rotate vertices
        t_vec3 rotated_v0 = rotate_point(scaled_v0, mesh->rotation);
        t_vec3 rotated_v1 = rotate_point(scaled_v1, mesh->rotation);
        t_vec3 rotated_v2 = rotate_point(scaled_v2, mesh->rotation);

        // Translate vertices and store
        mesh->transformed_tris[i].v0 = vec3_add(rotated_v0, mesh->position);
        mesh->transformed_tris[i].v1 = vec3_add(rotated_v1, mesh->position);
        mesh->transformed_tris[i].v2 = vec3_add(rotated_v2, mesh->position);

        // Precompute normal (no need to recalculate per ray)
        t_vec3 edge1 = vec3_subtract(mesh->transformed_tris[i].v1, mesh->transformed_tris[i].v0);
        t_vec3 edge2 = vec3_subtract(mesh->transformed_tris[i].v2, mesh->transformed_tris[i].v0);
        mesh->transformed_tris[i].normal = vec3_normalize(vec3_cross(edge1, edge2));
        
        // Copy vertex normals and transform them
        mesh->transformed_tris[i].has_vertex_normals = mesh->triangles[i].has_vertex_normals;
        if (mesh->triangles[i].has_vertex_normals)
        {
            mesh->transformed_tris[i].n0 = vec3_normalize(rotate_point(mesh->triangles[i].n0, mesh->rotation));
            mesh->transformed_tris[i].n1 = vec3_normalize(rotate_point(mesh->triangles[i].n1, mesh->rotation));
            mesh->transformed_tris[i].n2 = vec3_normalize(rotate_point(mesh->triangles[i].n2, mesh->rotation));
            
        }
    }

    // Build BVH after all triangles are transformed
    printf("About to build BVH...\n");
    build_mesh_bvh(mesh);
    printf("BVH build completed successfully!\n");
}


int ray_mesh_intersect(t_ray ray, t_mesh mesh, double *t, int *triangle_idx) {
    if (mesh.bvh.nodes && mesh.bvh.node_count > 0)
    {
        return mesh_bvh_intersect(ray, (t_mesh *)&mesh, t, triangle_idx);
    }
    else
    {
        double closest_t = INFINITY;
        int hit_something = 0;

        for (int i = 0; i < mesh.triangle_count; i++) {
            double current_t;
            if (ray_triangle_intersect(ray,
                (t_triangle){
                    .v0 = mesh.transformed_tris[i].v0,
                    .v1 = mesh.transformed_tris[i].v1,
                    .v2 = mesh.transformed_tris[i].v2,
                    .normal = mesh.transformed_tris[i].normal
                },
                &current_t))
            {
                if (current_t < closest_t) {
                    closest_t = current_t;
                    *triangle_idx = i;
                    hit_something = 1;
                }
            }
        }

        if (hit_something) {
            *t = closest_t;
            return 1;
        }
        return 0;
    }
}

int ray_mesh_intersect_bary(t_ray ray, t_mesh mesh, double *t, int *triangle_idx, t_vec3 *bary) {
    if (mesh.bvh.nodes && mesh.bvh.node_count > 0)
    {
        // Use BVH directly
        return mesh_bvh_intersect_bary(ray, (t_mesh *)&mesh, t, triangle_idx, bary);
    }
    else
    {
        double closest_t = INFINITY;
        int hit_something = 0;
        t_vec3 closest_bary;

        for (int i = 0; i < mesh.triangle_count; i++) {
            double current_t;
            t_vec3 current_bary;
            if (ray_triangle_intersect_bary(ray,
                (t_triangle){
                    .v0 = mesh.transformed_tris[i].v0,
                    .v1 = mesh.transformed_tris[i].v1,
                    .v2 = mesh.transformed_tris[i].v2,
                    .normal = mesh.transformed_tris[i].normal,
                    .n0 = mesh.transformed_tris[i].n0,
                    .n1 = mesh.transformed_tris[i].n1,
                    .n2 = mesh.transformed_tris[i].n2,
                    .has_vertex_normals = mesh.transformed_tris[i].has_vertex_normals
                },
                &current_t, &current_bary))
            {
                if (current_t < closest_t) {
                    closest_t = current_t;
                    closest_bary = current_bary;
                    *triangle_idx = i;
                    hit_something = 1;
                }
            }
        }

        if (hit_something) {
            *t = closest_t;
            *bary = closest_bary;
            return 1;
        }
        return 0;
    }
}


