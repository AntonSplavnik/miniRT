#include "../../includes/miniRT.h"
#include "bvh.h"
#include <time.h>

typedef struct s_build_node {
    int start;
    int end;
    int node_index;
} t_build_node;

typedef struct s_triangle_centroid {
    t_vec3 centroid;
    int original_index;
} t_triangle_centroid;

static void ft_swap_centroid(t_triangle_centroid *a, t_triangle_centroid *b)
{
    t_triangle_centroid tmp = *a;
    *a = *b;
    *b = tmp;
}

static t_aabb aabb_union(t_aabb a, t_aabb b)
{
    t_aabb res;
    res.min.x = fmin(a.min.x, b.min.x);
    res.min.y = fmin(a.min.y, b.min.y);
    res.min.z = fmin(a.min.z, b.min.z);
    res.max.x = fmax(a.max.x, b.max.x);
    res.max.y = fmax(a.max.y, b.max.y);
    res.max.z = fmax(a.max.z, b.max.z);
    return res;
}

double calculate_aabb_surface_area(t_aabb box)
{
    t_vec3 extent;

    extent = vec3_subtract(box.max, box.min);
    return 2.0 * (extent.x * extent.y + extent.y * extent.z +
                  extent.z * extent.x);
}

t_aabb calculate_transformed_tri_aabb(t_mesh *mesh, int tri_idx)
{
    t_aabb box;
    t_vec3 v0 = mesh->transformed_tris[tri_idx].v0;
    t_vec3 v1 = mesh->transformed_tris[tri_idx].v1;
    t_vec3 v2 = mesh->transformed_tris[tri_idx].v2;

    box.min.x = fmin(fmin(v0.x, v1.x), v2.x);
    box.min.y = fmin(fmin(v0.y, v1.y), v2.y);
    box.min.z = fmin(fmin(v0.z, v1.z), v2.z);
    box.max.x = fmax(fmax(v0.x, v1.x), v2.x);
    box.max.y = fmax(fmax(v0.y, v1.y), v2.y);
    box.max.z = fmax(fmax(v0.z, v1.z), v2.z);
    return box;
}

static void precompute_triangle_centroids(t_mesh *mesh,
    t_triangle_centroid *centroids)
{
    int i;
    t_transformed_triangle *tri;

    i = 0;
    while (i < mesh->triangle_count)
    {
        tri = &mesh->transformed_tris[i];
        centroids[i].centroid.x = (tri->v0.x + tri->v1.x + tri->v2.x) / 3.0;
        centroids[i].centroid.y = (tri->v0.y + tri->v1.y + tri->v2.y) / 3.0;
        centroids[i].centroid.z = (tri->v0.z + tri->v1.z + tri->v2.z) / 3.0;
        centroids[i].original_index = i;
        i++;
    }
}

static double get_centroid_axis_value(t_triangle_centroid *centroid, int axis)
{
    if (axis == 0)
        return centroid->centroid.x;
    if (axis == 1)
        return centroid->centroid.y;
    return centroid->centroid.z;
}

static int partition_by_threshold_fast(t_triangle_centroid *centroids,
    int start, int end, int axis, double threshold)
{
    int left;
    int right;
    double centroid_val;

    left = start;
    right = end;

    while (left <= right)
    {
        while (left <= end)
        {
            centroid_val = get_centroid_axis_value(&centroids[left], axis);
            if (centroid_val >= threshold)
                break;
            left++;
        }
        while (right >= start)
        {
            centroid_val = get_centroid_axis_value(&centroids[right], axis);
            if (centroid_val < threshold)
                break;
            right--;
        }
        if (left < right)
            ft_swap_centroid(&centroids[left], &centroids[right]);
        left++;
        right--;
    }
    return right + 1;
}

static t_aabb compute_bounds_from_centroids(t_mesh *mesh,
    t_triangle_centroid *centroids, int start, int end)
{
    t_aabb bounds;
    t_aabb tri_bounds;
    int i;

    bounds = calculate_transformed_tri_aabb(mesh,
                                           centroids[start].original_index);
    i = start + 1;
    while (i <= end)
    {
        tri_bounds = calculate_transformed_tri_aabb(mesh,
                                                   centroids[i].original_index);
        bounds = aabb_union(bounds, tri_bounds);
        i++;
    }
    return bounds;
}

static int find_best_split_axis_fast(t_triangle_centroid *centroids,
    int start, int end)
{
    t_vec3 min_centroid;
    t_vec3 max_centroid;
    t_vec3 extent;
    int i;

    min_centroid = centroids[start].centroid;
    max_centroid = centroids[start].centroid;

    i = start + 1;
    while (i <= end)
    {
        if (centroids[i].centroid.x < min_centroid.x)
            min_centroid.x = centroids[i].centroid.x;
        if (centroids[i].centroid.x > max_centroid.x)
            max_centroid.x = centroids[i].centroid.x;
        if (centroids[i].centroid.y < min_centroid.y)
            min_centroid.y = centroids[i].centroid.y;
        if (centroids[i].centroid.y > max_centroid.y)
            max_centroid.y = centroids[i].centroid.y;
        if (centroids[i].centroid.z < min_centroid.z)
            min_centroid.z = centroids[i].centroid.z;
        if (centroids[i].centroid.z > max_centroid.z)
            max_centroid.z = centroids[i].centroid.z;
        i++;
    }

    extent = vec3_subtract(max_centroid, min_centroid);
    if (extent.y > extent.x && extent.y > extent.z)
        return 1;
    if (extent.z > extent.x)
        return 2;
    return 0;
}

static int split_node_fast(t_mesh *mesh, t_triangle_centroid *centroids,
    t_build_node *stack, int stack_ptr, int start, int end,
    int node_index, int *node_count)
{
    int best_axis;
    double threshold;
    int split_pos;
    int left_index, right_index;

    best_axis = find_best_split_axis_fast(centroids, start, end);
    threshold = (get_centroid_axis_value(&centroids[start], best_axis) +
                 get_centroid_axis_value(&centroids[end], best_axis)) / 2.0;

    split_pos = partition_by_threshold_fast(centroids, start, end,
                                           best_axis, threshold);

    if (split_pos == start || split_pos > end)
        split_pos = start + (end - start) / 2;

    left_index = (*node_count)++;
    right_index = (*node_count)++;

    if (left_index >= mesh->bvh.max_nodes || right_index >= mesh->bvh.max_nodes)
    {
        printf("Error: node indices out of bounds: left=%d, right=%d, max=%d\n",
               left_index, right_index, mesh->bvh.max_nodes);
        return stack_ptr;
    }

    mesh->bvh.tri_indices[node_index * 2] = -left_index;
    mesh->bvh.tri_indices[node_index * 2 + 1] = -right_index;

    if (stack_ptr < MAX_STACK_SIZE - 2)
    {
        stack[stack_ptr++] = (t_build_node){split_pos, end, right_index};
        stack[stack_ptr++] = (t_build_node){start, split_pos - 1, left_index};
    }
    return stack_ptr;
}

static int process_build_node_fast(t_mesh *mesh,
    t_triangle_centroid *centroids, t_build_node *stack,
    int stack_ptr, int *node_count)
{
    t_build_node current;
    t_aabb node_bounds;
    int start, end, node_index, tri_count;

    current = stack[--stack_ptr];
    start = current.start;
    end = current.end;
    node_index = current.node_index;
    tri_count = end - start + 1;

    if (node_index >= mesh->bvh.max_nodes)
    {
        printf("Error: node_index %d >= max_nodes %d\n", node_index, mesh->bvh.max_nodes);
        return stack_ptr;
    }

    node_bounds = compute_bounds_from_centroids(mesh, centroids, start, end);
    mesh->bvh.nodes[node_index] = node_bounds;

    if (tri_count <= LEAF_TRI_THRESHOLD)
    {
        mesh->bvh.tri_indices[node_index * 2] = start;
        mesh->bvh.tri_indices[node_index * 2 + 1] = tri_count;
        return stack_ptr;
    }

    return split_node_fast(mesh, centroids, stack, stack_ptr,
                          start, end, node_index, node_count);
}

static void build_mesh_bvh_fast(t_mesh *mesh,
    t_triangle_centroid *centroids)
{
    t_build_node stack[MAX_STACK_SIZE];
    int stack_ptr;
    int node_count;

    stack_ptr = 0;
    node_count = 1;
    stack[stack_ptr++] = (t_build_node){0, mesh->triangle_count - 1, 0};

    while (stack_ptr > 0)
    {
        stack_ptr = process_build_node_fast(mesh, centroids, stack,
                                           stack_ptr, &node_count);
    }
    mesh->bvh.node_count = node_count;
}

static void copy_indices_from_centroids(t_mesh *mesh,
    t_triangle_centroid *centroids, int *tri_indices)
{
    int i;

    i = 0;
    while (i < mesh->triangle_count)
    {
        tri_indices[i] = centroids[i].original_index;
        i++;
    }
}

void build_mesh_bvh(t_mesh *mesh)
{
    t_triangle_centroid *centroids;
    int *tri_indices;
    clock_t start_time, end_time;

    if (!mesh || mesh->triangle_count == 0)
        return;

    start_time = clock();
    printf("Building BVH for mesh with %d triangles...\n", mesh->triangle_count);

    centroids = malloc(mesh->triangle_count * sizeof(t_triangle_centroid));
    if (!centroids)
    {
        printf("Failed to allocate centroids array\n");
        return;
    }
    printf("Centroids allocated successfully\n");

    precompute_triangle_centroids(mesh, centroids);
    printf("Triangle centroids precomputed\n");

    mesh->bvh.max_nodes = 2 * mesh->triangle_count - 1;
    printf("Allocating BVH nodes: %d nodes, %zu bytes\n",
           mesh->bvh.max_nodes, mesh->bvh.max_nodes * sizeof(t_aabb));
    mesh->bvh.nodes = malloc(mesh->bvh.max_nodes * sizeof(t_aabb));
    printf("Allocating BVH tri_indices: %zu bytes\n",
           mesh->bvh.max_nodes * 2 * sizeof(int));
    mesh->bvh.tri_indices = malloc(mesh->bvh.max_nodes * 2 * sizeof(int));

    if (!mesh->bvh.nodes || !mesh->bvh.tri_indices)
    {
        printf("Failed to allocate BVH arrays\n");
        free(centroids);
        free(mesh->bvh.nodes);
        free(mesh->bvh.tri_indices);
        return;
    }
    printf("BVH arrays allocated successfully\n");

    printf("Starting BVH fast build...\n");
    build_mesh_bvh_fast(mesh, centroids);
    printf("BVH fast build completed, node_count = %d\n", mesh->bvh.node_count);

    tri_indices = malloc(mesh->triangle_count * sizeof(int));
    if (tri_indices)
    {
        copy_indices_from_centroids(mesh, centroids, tri_indices);
        printf("Triangle indices copied successfully\n");

        if (mesh->bvh.node_children)
            free(mesh->bvh.node_children);
        mesh->bvh.node_children = tri_indices;
        printf("BVH triangle mapping completed\n");
    }
    else
    {
        printf("Failed to allocate triangle indices array\n");
    }

    free(centroids);

    end_time = clock();
    printf("BVH build completed in %.2f seconds\n",
           ((double)(end_time - start_time)) / CLOCKS_PER_SEC);
}
