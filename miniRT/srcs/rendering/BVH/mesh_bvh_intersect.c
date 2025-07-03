#include "../../includes/miniRT.h"
#include "bvh.h"

static int ray_intersect_aabb_optimized(t_ray ray, t_aabb bounds, double max_t)
{
    double t1, t2, t_near, t_far;
    double inv_dir_x, inv_dir_y, inv_dir_z;

    inv_dir_x = 1.0 / ray.direction.x;
    inv_dir_y = 1.0 / ray.direction.y;
    inv_dir_z = 1.0 / ray.direction.z;

    t1 = (bounds.min.x - ray.origin.x) * inv_dir_x;
    t2 = (bounds.max.x - ray.origin.x) * inv_dir_x;
    t_near = fmin(t1, t2);
    t_far = fmax(t1, t2);

    t1 = (bounds.min.y - ray.origin.y) * inv_dir_y;
    t2 = (bounds.max.y - ray.origin.y) * inv_dir_y;
    t_near = fmax(t_near, fmin(t1, t2));
    t_far = fmin(t_far, fmax(t1, t2));

    t1 = (bounds.min.z - ray.origin.z) * inv_dir_z;
    t2 = (bounds.max.z - ray.origin.z) * inv_dir_z;
    t_near = fmax(t_near, fmin(t1, t2));
    t_far = fmin(t_far, fmax(t1, t2));

    return t_near <= t_far && t_far > 0.001 && t_near < max_t;
}

static int ray_triangle_intersect_fast(t_ray ray,
    t_transformed_triangle tri, double *t)
{
    t_vec3 edge1, edge2, h, s, q;
    double a, f, u, v, temp_t;

    edge1 = vec3_subtract(tri.v1, tri.v0);
    edge2 = vec3_subtract(tri.v2, tri.v0);
    h = vec3_cross(ray.direction, edge2);
    a = vec3_dot(edge1, h);

    if (a > -1e-7 && a < 1e-7)
        return 0;

    f = 1.0 / a;
    s = vec3_subtract(ray.origin, tri.v0);
    u = f * vec3_dot(s, h);

    if (u < 0.0 || u > 1.0)
        return 0;

    q = vec3_cross(s, edge1);
    v = f * vec3_dot(ray.direction, q);

    if (v < 0.0 || u + v > 1.0)
        return 0;

    temp_t = f * vec3_dot(edge2, q);
    if (temp_t > 1e-7)
    {
        *t = temp_t;
        return 1;
    }
    return 0;
}

static int test_leaf_triangles_batch(t_mesh *mesh, t_ray ray, int start,
    int count, double *closest_t, int *tri_idx, int hit_something)
{
    int i, idx, original_idx;
    double current_t;
    t_transformed_triangle *tri;

    if (count <= 0 || start < 0)
        return hit_something;

    i = 0;
    while (i < count)
    {
        idx = start + i;
        if (mesh->bvh.node_children && idx < mesh->triangle_count)
            original_idx = mesh->bvh.node_children[idx];
        else
            original_idx = idx;

        if (original_idx >= 0 && original_idx < mesh->triangle_count)
        {
            tri = &mesh->transformed_tris[original_idx];

            if (ray_triangle_intersect_fast(ray, *tri, &current_t))
            {
                if (current_t < *closest_t)
                {
                    *closest_t = current_t;
                    *tri_idx = original_idx;
                    hit_something = 1;
                }
            }
        }
        i++;
    }
    return hit_something;
}

static int process_bvh_node_optimized(t_mesh *mesh, t_ray ray, int *stack,
    int *stack_ptr, double *closest_t, int *tri_idx, int hit_something)
{
    int node_index;
    t_aabb node_bounds;
    int left_index, right_index;

    node_index = stack[--(*stack_ptr)];
    node_bounds = mesh->bvh.nodes[node_index];

    if (!ray_intersect_aabb_optimized(ray, node_bounds, *closest_t))
        return hit_something;

    left_index = mesh->bvh.tri_indices[node_index * 2];
    right_index = mesh->bvh.tri_indices[node_index * 2 + 1];

    if (left_index >= 0)
    {
        return test_leaf_triangles_batch(mesh, ray, left_index, right_index,
                                        closest_t, tri_idx, hit_something);
    }
    else
    {
        if (*stack_ptr < MAX_STACK_SIZE - 2)
        {
            stack[(*stack_ptr)++] = -right_index;
            stack[(*stack_ptr)++] = -left_index;
        }
    }
    return hit_something;
}

int mesh_bvh_intersect(t_ray ray, t_mesh *mesh, double *t, int *tri_idx)
{
    int stack[MAX_STACK_SIZE];
    int stack_ptr;
    double closest_t;
    int hit_something;

    if (!mesh->bvh.nodes || mesh->bvh.node_count == 0)
        return 0;

    stack_ptr = 0;
    closest_t = INFINITY;
    hit_something = 0;
    stack[stack_ptr++] = 0;

    while (stack_ptr > 0)
    {
        hit_something = process_bvh_node_optimized(mesh, ray, stack,
                                                  &stack_ptr, &closest_t,
                                                  tri_idx, hit_something);
    }

    if (hit_something)
    {
        *t = closest_t;
        return 1;
    }
    return 0;
}
