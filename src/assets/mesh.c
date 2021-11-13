#include "assets/mesh.h"

#include "shaders/vs_shader.h"
#include "vendor/tinyobj_loader_c.h"

#include <windows.h>

typedef struct {
    vec3 pos;
    vec2 texcoord;
    vec3 normal;
} vertex_mesh;

void load_obj(mesh* m, const char* filename);


// mesh* mesh_new() {
//     mesh* m = malloc(sizeof(mesh));
//     memset(m, 0, sizeof(mesh));
//     return m;
// }

mesh* mesh_load(const char* filename) {
    if (!file_ext_eq(filename, "obj")) {
        error("We are currently only support .obj files");
    }

    mesh* m = malloc(sizeof(mesh));
    memset(m, 0, sizeof(mesh));

    // const char* abs = file_abs(filename);
    load_obj(m, filename);

    return m;
}

void mesh_unload(mesh* m) {
    d3d11_release_vertex_buffer(m->vb);
    d3d11_release_index_buffer(m->ib);

    free(m);
}

void file_data(void* ctx, const char* filename, const int is_mtl, 
    const char* obj_filename, char** data, size_t* len) {

    char tmp[PATH_MAX];
    tmp[0] = '\0';

    if (is_mtl && obj_filename) {
        // copy base dir.
        char buf[PATH_MAX];
        buf[0] = '\0';
        strncpy(buf, obj_filename, strlen(obj_filename) + 1);

        char* c = strrchr(buf, '\\');
        c[0] = '\0';

        strncat(buf, SEPARATOR, 1);
        strncat(buf, filename, strlen(filename) - 1);
        strncpy(tmp, buf, strlen(buf) + 1);
    } else {
        strncpy(tmp, filename, strlen(filename) + 1);
    }


    *data = file_load_win32(tmp, len);
}


// load new mesh.
void load_obj(mesh* m, const char* filename) {
	tinyobj_attrib_t attrib;
 	tinyobj_shape_t* shapes = NULL;
  	tinyobj_material_t* materials = NULL;
  	size_t num_shapes;
  	size_t num_materials;

  	int res = tinyobj_parse_obj(
        &attrib, &shapes, &num_shapes, &materials, 
        &num_materials, filename, file_data, 
        NULL, TINYOBJ_FLAG_TRIANGULATE
    );
  	if (res != TINYOBJ_SUCCESS) {
  		error("load obj file error: %s", filename);
    }

    vertex_mesh* vertices = malloc(sizeof(vertex_mesh) * attrib.num_vertices);
    memset(vertices, 0, sizeof(vertex_mesh) * attrib.num_vertices);

    uint* indices = malloc(sizeof(uint) * attrib.num_faces);
    memset(vertices, 0, sizeof(uint) * attrib.num_faces);

    size_t size_index = 0;
    for (size_t i = 0; i < attrib.num_face_num_verts; i++) {
        assert(attrib.face_num_verts[i] % 3 == 0);

        for (size_t j = 0; j < (size_t)attrib.face_num_verts[i]; j++) {
            tinyobj_vertex_index_t index = attrib.faces[i * 3  + j];

            float vx = attrib.vertices[index.v_idx * 3 + 0];
            float vy = attrib.vertices[index.v_idx * 3 + 1];
            float vz = attrib.vertices[index.v_idx * 3 + 2];

            float tx = attrib.texcoords[index.vt_idx * 2 + 0];
            float ty = attrib.texcoords[index.vt_idx * 2 + 1];

            float nx = attrib.normals[index.vn_idx * 3 + 0];
            float ny = attrib.normals[index.vn_idx * 3 + 1];
            float nz = attrib.normals[index.vn_idx * 3 + 2];

            vertex_mesh v = {0};
            v.pos = vec3_new(vx, vy, vz);
            v.texcoord = vec2_new(tx, ty);
            v.normal = vec3_new(nx, ny, nz);

            vertices[index.v_idx] = v;
            indices[size_index++] = (uint)index.v_idx;
        }
    }
  
    m->vb = d3d11_create_vertex_buffer(vertices, sizeof(vertex_mesh), attrib.num_vertices, (void*)g_vsmain, _countof(g_vsmain));
    m->ib = d3d11_create_index_buffer(indices, size_index);

    // free tinyobj.
    tinyobj_attrib_free(&attrib);
    tinyobj_shapes_free(shapes, num_shapes);
    tinyobj_materials_free(materials, num_materials);
}