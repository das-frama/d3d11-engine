#include "utils/geometry.h"
#include "assets/mesh.h"
#include "vendor/stb_ds.h"

mesh_data* generate_grid(float w, float d, int m, int n) {
    mesh_data* md = calloc(1, sizeof(mesh_data));

    int vertex_count = (m + n) * 2;
    int index_count = (m+1 + n+1) * 2;

    arrsetcap(md->vertices, vertex_count);
    arrsetcap(md->indices,  index_count);

    float half_width = 0.5f * w;
    float half_depth = 0.5f * d;

    float dx = w / n;
    float dz = d / m;

    // Generate vertices.
    for (int i = 0; i <= m; i++) {
        float x =  -half_width + i * dx;
        md->vertices[i].pos = vec3_new(x, 0, half_depth);
        md->vertices[i].normal = vec3_zero();
        md->vertices[i].texcoord = vec2_zero();
        md->vertices[m+n+i].pos = vec3_new(-x, 0, -half_depth);
        md->vertices[m+n+i].normal = vec3_zero();
        md->vertices[m+n+i].texcoord = vec2_zero();
    }
    for (int i = 0; i <= n; i++) {
        float z = half_depth - i * dz;
        md->vertices[m+i].pos = vec3_new(half_width, 0, z);
        md->vertices[m+i].normal = vec3_zero();
        md->vertices[m+i].texcoord = vec2_zero();
        md->vertices[(vertex_count-n+i) % vertex_count].pos = vec3_new(-half_width, 0, -z);
        md->vertices[(vertex_count-n+i) % vertex_count].normal = vec3_zero();
        md->vertices[(vertex_count-n+i) % vertex_count].texcoord = vec2_zero();
    }

    // Generate indices.
    size_t index = 0;
    for (int i = 0; i <= m; i++) {
        md->indices[index] = i;
        md->indices[index+1] = vertex_count-m-i;
        index += 2;
    }
    for (int i = 0; i <= n; i++) {
        md->indices[index] = (vertex_count-i) % vertex_count;
        md->indices[index+1] = n+i;
        index += 2;
    }

    md->vertices_count = vertex_count;
    md->indices_count  = index_count;

    return md;
}

mesh_data* generate_sphere(float radius, size_t slice_count, size_t stack_count) {
    mesh_data* md = calloc(1, sizeof(mesh_data));

    arrsetcap(md->vertices, stack_count * (slice_count + 1));
    arrsetcap(md->indices, slice_count * 3);

    vertex top_vertex = {{0, radius, 0}, {0, 0}, {0, 1, 0}};
    vertex bottom_vertex = {{0, -radius, 0}, {0, 1}, {0, -1, 0}};

    arrput(md->vertices, top_vertex);

    float phi_step = PI / stack_count;
    float theta_step = PI2 / slice_count;

    // Compute vertices.
    for (int i = 0; i <= stack_count; i++) {
        float phi = i * phi_step;

        for (int j = 0; j <= slice_count; j++) {
            float theta = j * theta_step;

            vertex v = {0};

            v.pos.x = radius * sinf(phi) * cosf(theta);
            v.pos.y = radius * cosf(phi);
            v.pos.z = radius * sinf(phi) * sinf(theta);

            v.texcoord.x = (float)j / slice_count;
            v.texcoord.y = 1.0f - (float)i / stack_count;

            v.normal = vec3_normalize(vec3_mul(v.pos, -1));

            arrput(md->vertices, v);
        }
    }

    arrput(md->vertices, bottom_vertex);

    // Compute indices for top stack.
    for (int i = 1; i <= slice_count; i++) {
        arrput(md->indices, 0);
        arrput(md->indices, i+1);
        arrput(md->indices, i);
    }

    // Compute vertices for inner stack.
    size_t base_index = 1;
    size_t ring_vertex_count = slice_count + 1;
    for (int i = 0; i < stack_count; i++) {
        for (int j = 0; j < slice_count; j++) {
            arrput(md->indices, base_index + i*ring_vertex_count + j);
            arrput(md->indices, base_index + i*ring_vertex_count + j+1);
            arrput(md->indices, base_index + (i+1)*ring_vertex_count + j);

            arrput(md->indices, base_index + (i+1)*ring_vertex_count + j);
            arrput(md->indices, base_index + i*ring_vertex_count + j+1);
            arrput(md->indices, base_index + (i+1)*ring_vertex_count + j+1);
        }
    }

    // Compute indices for bottom stack.
    size_t south_pole_index = arrlen(md->vertices);
    base_index = south_pole_index - ring_vertex_count;
    for (int i = 0; i < slice_count; i++) {
        arrput(md->indices, south_pole_index);
        arrput(md->indices, base_index + i);
        arrput(md->indices, base_index + i + 1);
    }

    md->vertices_count = arrlen(md->vertices);
    md->indices_count  = arrlen(md->indices);

    return md;
}