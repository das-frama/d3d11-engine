#include "assets/mesh.h"

#include "utils/utils.h"

#include "shaders/vs_shader.h"

#include <windows.h>
#include <d3d11.h>

#include "vendor/stb_ds.h"
#include "vendor/tinyobj_loader_c.h"

extern renderer* rnd;

mesh* mesh_new() {
	mesh* m = malloc(sizeof(mesh));
    memset(m, 0, sizeof(mesh));
    
	return m;
}

void mesh_delete(mesh* m) {
    m->vb.size_list = 0;
    m->vb.size_vertex = 0;
    ID3D11Buffer_Release(m->vb.buffer);
    ID3D11InputLayout_Release(m->vb.input_layout);

    m->ib.size_list = 0;
    ID3D11Buffer_Release(m->ib.buffer);

    free(m);
}

// load new mesh.
void mesh_load_obj(mesh* m, const char* filename) {
	tinyobj_attrib_t attrib;
 	tinyobj_shape_t* shapes = NULL;
  	tinyobj_material_t* materials = NULL;
  	size_t num_shapes;
  	size_t num_materials;

  	int res = tinyobj_parse_obj(
        &attrib, &shapes, &num_shapes, &materials, 
        &num_materials, filename, load_file_win32, 
        NULL, TINYOBJ_FLAG_TRIANGULATE
    );
  	if (res != TINYOBJ_SUCCESS) {
  		error("load obj file error: %s", filename);
    }

    if (num_shapes > 1) {
    	error("We are currently support only one shape");
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
  
    m->vb = d3d11_create_vertex_buffer(rnd, vertices, sizeof(vertex_mesh), attrib.num_vertices, g_vsmain, _countof(g_vsmain));
    m->ib = d3d11_create_index_buffer(rnd, indices, size_index);

    // free tinyobj.
    tinyobj_attrib_free(&attrib);
    tinyobj_shapes_free(shapes, num_shapes);
    tinyobj_materials_free(materials, num_materials);
}

static void load_file_win32(void* ctx, const char* filename, const int is_mtl, 
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

    HANDLE file = CreateFileA(tmp, GENERIC_READ, FILE_SHARE_READ, NULL, 
        OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);

    if (file == INVALID_HANDLE_VALUE) { // Model may not have materials.
        error("error file");
    }

    HANDLE fmap = CreateFileMapping(file, NULL, PAGE_READONLY, 0, 0, NULL);
    assert(fmap != INVALID_HANDLE_VALUE);

    LPVOID fmap_view = MapViewOfFile(fmap, FILE_MAP_READ, 0, 0, 0);
    assert(fmap_view != NULL);

    DWORD file_size = GetFileSize(file, NULL);
    (*len) = (size_t)file_size;
    *data = (char*)fmap_view;
}

// static void load_file_posix(void* ctx, const char* filename, const int is_mtl, 
//     const char* obj_filename, char** data, size_t* len) {

//     (*len) = 0;

//     FILE* f = fopen(filename, "r");
//     if (!f) {
//         error("could not open file %s", filename);
//     }

//     fseek(f, 0, SEEK_END);
//     long file_size = ftell(f);
//     fclose(f);

//     int fd = open(filename, O_RDONLY);
//     if (fd == -1) {
//         error("open");
//     }

//     struct stat sb;
//     if (fstat(fd, &sb) == -1) {
//         error("fstat");
//     }

//     if (!S_ISREG(sb.st_mode)) {
//         error("%s is not a file\n", "lineitem.tbl");
//     }

//     char* p = (char*)mmap(0, (size_t)file_size, PROT_READ, MAP_SHARED, fd, 0);
//     if (p == MAP_FAILED) {
//         error("mmap");
//     }

//     if (close(fd) == -1) {
//         error("close");
//         return;
//     }

//     *len = (size_t)file_size;
//     *data = p;
// }