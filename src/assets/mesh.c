#include "assets/mesh.h"

#include "shaders/mesh_vs.h"
#include "vendor/tinyobj_loader_c.h"
#include "vendor/stb_ds.h"

#include <windows.h>

mesh_data* load_obj(const char* filename);

mesh* mesh_new() {
    return (mesh*)calloc(1, sizeof(mesh));
}

mesh* mesh_new_load(const char* filename) {
    if (!file_ext_eq(filename, "obj")) {
        error("We are currently only support .obj files");
    }

    mesh* m = mesh_new();
    m->topology = TOPOLOGY_TRIANGLE_LIST;
    mesh_data* md = load_obj(filename);

    mesh_data_set(m, md);
    mesh_data_delete(md);

    return m;
}

mesh* mesh_new_data(mesh_data* md) {
    mesh* m = mesh_new();
    m->topology = TOPOLOGY_TRIANGLE_LIST;
    mesh_data_set(m, md);

    return m;
}

void mesh_delete(mesh* m) {
    d3d11_release_vertex_buffer(m->vb);
    d3d11_release_index_buffer(m->ib);
    m->vb = NULL;
    m->ib = NULL;
    free(m);
}

void mesh_data_set(mesh* m, mesh_data* md) {
    m->vb = d3d11_create_vertex_buffer(md->vertices, sizeof(vertex), md->vertices_count, (void*)g_mesh_vs, _countof(g_mesh_vs));
    m->ib = d3d11_create_index_buffer(md->indices, md->indices_count);
}

void mesh_data_delete(mesh_data* md) {
    arrfree(md->vertices);
    arrfree(md->indices);
    md->vertices = NULL;
    md->indices = NULL;
    free(md); 
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
mesh_data* load_obj(const char* filename) {
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

    mesh_data* md = malloc(sizeof(mesh_data));
    memset(md, 0, sizeof(mesh_data));

    md->vertices = NULL;
    arrsetcap(md->vertices, attrib.num_vertices);
    md->indices = NULL;
    arrsetcap(md->indices, attrib.num_faces);

    size_t size_index = 0;
    for (size_t i = 0; i < attrib.num_face_num_verts; i++) {
        assert(attrib.face_num_verts[i] % 3 == 0);
        size_t face_size = (size_t)attrib.face_num_verts[i];
        for (size_t j = 0; j < face_size; j++) {
            tinyobj_vertex_index_t index = attrib.faces[i * 3  + j];

            float vx = attrib.vertices[index.v_idx * 3 + 0];
            float vy = attrib.vertices[index.v_idx * 3 + 1];
            float vz = attrib.vertices[index.v_idx * 3 + 2];

            float tx = attrib.texcoords[index.vt_idx * 2 + 0];
            float ty = attrib.texcoords[index.vt_idx * 2 + 1];

            float nx = attrib.normals[index.vn_idx * 3 + 0];
            float ny = attrib.normals[index.vn_idx * 3 + 1];
            float nz = attrib.normals[index.vn_idx * 3 + 2];

            vertex v = {0};
            v.pos = vec3_new(vx, vy, vz);
            v.texcoord = vec2_new(tx, ty);
            v.normal = vec3_new(nx, ny, nz);

            arrput(md->vertices, v);
            arrput(md->indices, size_index + j);
        }
        size_index += face_size;
    }

    md->vertices_count = arrlen(md->vertices);
    md->indices_count = arrlen(md->indices);

    // free tinyobj.
    tinyobj_attrib_free(&attrib);
    tinyobj_shapes_free(shapes, num_shapes);
    tinyobj_materials_free(materials, num_materials);

    return md;
}