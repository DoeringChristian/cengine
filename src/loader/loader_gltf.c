#include "loader_gltf.h"

#ifndef CGLTF_IMPLEMENTATION
#define CGLTF_IMPLEMENTATION
#include "cgltf.h"
#endif //CGLTF_IMPLEMENTATION

int container_load_gltf(struct container *dst, const char *path){
    container_init(dst);

    cgltf_options options = {0};
    cgltf_data *data = NULL;
    cgltf_result result = cgltf_parse_file(&options, path, &data);
    if(result != cgltf_result_success)
        return -1;
    result = cgltf_load_buffers(&options, data, path);
    if(result != cgltf_result_success)
        return -1;

    
    for(size_t i = 0;i < data->meshes_count;i++){
        cgltf_mesh gltf_mesh = data->meshes[i];
        struct mesh *mesh = malloc(sizeof(struct mesh));
        mesh_init(mesh, NULL, 0, NULL, 0);

        resource_handle_name_set(&mesh->handle, gltf_mesh.name);

        //gltf_mesh.primitives->attributes_count;
        float *pos = NULL;
        size_t pos_size = 0;
        float *norm = NULL;
        size_t norm_size = 0;
        float *tan = NULL;
        size_t tan_size = 0;
        float *color = NULL;
        size_t color_size = 0;
        float *uv = NULL;
        size_t uv_size = 0;
        for(size_t j = 0;j < gltf_mesh.primitives->attributes_count;j++){

            cgltf_attribute attr = gltf_mesh.primitives->attributes[j];
            if(attr.type == cgltf_attribute_type_position){
                cgltf_accessor *acc = attr.data;

                pos_size = sizeof(float) * acc->count;
                pos = malloc(pos_size);
                memcpy(pos, acc->buffer_view->buffer->data, pos_size);
            }
            else if(attr.type == cgltf_attribute_type_normal){
                cgltf_accessor *acc = attr.data;

                norm_size = sizeof(float) * acc->count;
                norm = malloc(norm_size);
                memcpy(norm, acc->buffer_view->buffer->data, norm_size);
            }
            else if(attr.type == cgltf_attribute_type_tangent){
                cgltf_accessor *acc = attr.data;

                tan_size = sizeof(float) * acc->count;
                tan = malloc(tan_size);
                memcpy(tan, acc->buffer_view->buffer->data, tan_size);
            }
            else if(attr.type == cgltf_attribute_type_color){
                cgltf_accessor *acc = attr.data;

                color_size = sizeof(float) * acc->count;
                color = malloc(color_size);
                memcpy(color, acc->buffer_view->buffer->data, color_size);
            }
            else if(attr.type == cgltf_attribute_type_texcoord){
                cgltf_accessor *acc = attr.data;

                uv_size = sizeof(float) * acc->count;
                uv = malloc(uv_size);
                memcpy(uv, acc->buffer_view->buffer->data, uv_size);
            }
        }
        size_t vert_num = pos_size / (3 * sizeof(float));
        struct vert *verts = malloc(vert_num * sizeof(struct vert));

        //printf("%i\n", gltf_mesh.primitives->indices->stride);
        for(size_t j = 0;j < vert_num;j++){
            //printf("%f %f %f\n", pos[j * 3], pos[j * 3+1], pos[j * 3 +2]);
            if(j * (3 * sizeof(float)) < pos_size)
                glm_vec3_copy(&pos[j * 3], verts[j].pos);
            if(j * (3 * sizeof(float)) < norm_size)
                glm_vec3_copy(&norm[j * 3], verts[j].normal);
            if(j * (3 * sizeof(float)) < tan_size)
                glm_vec3_copy(&tan[j * 3], verts[j].tangent);
            if(j * (4 * sizeof(float)) < color_size)
                glm_vec4_copy(&color[j * 4], verts[j].color);
            if(j * (2 * sizeof(float)) < uv_size)
                glm_vec2_copy(&uv[j * 2], verts[j].uv);
        }

        mesh_vert_append(mesh, verts, vert_num);

        free(verts);
        free(pos);
        free(norm);
        free(tan);
        free(color);
        free(uv);
        verts = NULL;
        pos = NULL;
        norm = NULL;
        tan = NULL;
        color = NULL;
        uv = NULL;

        cgltf_accessor *iacc = gltf_mesh.primitives->indices;
        size_t idx_num;
        GLint *idx;
        if(iacc->component_type != cgltf_component_type_r_16u)
            assert(0);

        size_t tris_num = iacc->count / 3;
        struct tri *tris = malloc(sizeof(struct tri) * tris_num);

        for(size_t j = 0;j < tris_num;j++){
            tris[j].idxs[0] = ((uint16_t *)iacc->buffer_view->buffer->data)[j*3 + 0];
            tris[j].idxs[1] = ((uint16_t *)iacc->buffer_view->buffer->data)[j*3 + 1];
            tris[j].idxs[2] = ((uint16_t *)iacc->buffer_view->buffer->data)[j*3 + 2];
        }

        mesh_tri_append(mesh, tris, tris_num);

        free(tris);
        tris = NULL;


        darray_push_back(&dst->meshes, mesh);
    }


    cgltf_free(data);
    return 0;
}
