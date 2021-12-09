#include "gltf_loader.h"

int mesh_load_gltf(struct mesh *dst, const char *path, int idx){
#if 0
    cgltf_options options = {0};
    cgltf_data* data = NULL;
    cgltf_result result = cgltf_parse_file(&options, "scene.gltf", &data);
    if(result != cgltf_result_success)
        return 1;

    if(idx >= data->meshes_count)
        return 2;

    int idxs_count = data->meshes[idx].primitives[0].indices->count;
    int attr_count = data->meshes[idx].primitives[0].attributes_count;

    cgltf_attribute *attrs = data->meshes[idx].primitives[0].attributes;

    struct cmesh cmesh;
    cmesh_init(&cmesh);

    // -----------------------------------------
    // load geometry

    for(size_t i = 0;i < attr_count;i++){
        if(attrs->type == cgltf_attribute_type_position){
            // ---------------------------------------------
            // loading positions
            cgltf_buffer_view *bv = attrs->data->buffer_view;
            const char *uri = attrs->data->buffer_view->buffer->uri;
            FILE *fp = fopen(uri, "r");
            fseek(fp, bv->offset, SEEK_SET);
            uint8_t *buf = malloc(bv->size);

            fread(buf, 1, bv->size, fp);

            for(size_t j = 0;j < attrs->data->count;j++){
                struct vert tmp;
                if(j < darray_len(&cmesh.verts))
                    tmp = cmesh.verts[j];
                memcpy(tmp.pos, &buf[j * sizeof(float) * 3], sizeof(float) * 3);
                if(j < darray_len(&cmesh.verts))
                    cmesh.verts[j] = tmp;
                else
                    cmesh_vert_push_back(&cmesh, tmp);
            }

            free(buf);
            fclose(fp);
        }
        else if(attrs->type == cgltf_attribute_type_normal){
            // ---------------------------------------------
            // loading tris
            cgltf_buffer_view *bv = attrs->data->buffer_view;
            const char *uri = attrs->data->buffer_view->buffer->uri;
            FILE *fp = fopen(uri, "r");
            fseek(fp, bv->offset, SEEK_SET);
            uint8_t *buf = malloc(bv->size);

            fread(buf, 1, bv->size, fp);

            for(size_t j = 0;j < attrs->data->count;j++){
                struct vert tmp;
                if(j < darray_len(&cmesh.verts))
                    tmp = cmesh.verts[j];
                memcpy(tmp.normal, &buf[j * sizeof(float) * 3], sizeof(float) * 3);
                if(j < darray_len(&cmesh.verts))
                    cmesh.verts[j] = tmp;
                else
                    cmesh_vert_push_back(&cmesh, tmp);
            }

            free(buf);
            fclose(fp);
        }
        else if(attrs->type == cgltf_attribute_type_texcoord){
            // -----------------------------------------------
            // load uv
            cgltf_buffer_view *bv = attrs->data->buffer_view;
            const char *uri = attrs->data->buffer_view->buffer->uri;
            FILE *fp = fopen(uri, "r");
            fseek(fp, bv->offset, SEEK_SET);
            uint8_t *buf = malloc(bv->size);

            fread(buf, 1, bv->size, fp);

            for(size_t j = 0;j < attrs->data->count;j++){
                struct vert tmp;
                if(j < darray_len(&cmesh.verts))
                    tmp = cmesh.verts[j];
                memcpy(tmp.uv, &buf[j * sizeof(float) * 2], sizeof(float) * 2);
                if(j < darray_len(&cmesh.verts))
                    cmesh.verts[j] = tmp;
                else
                    cmesh_vert_push_back(&cmesh, tmp);
            }

            free(buf);
            fclose(fp);
        }
    }
    cgltf_texture_view texture_albedo = data->meshes[idx].primitives[0].material->pbr_metallic_roughness.base_color_texture;
    //texture_albedo.texture->image->uri;
    // TODO: load textures and tris
    texture_load(dst->tex_albedo, texture_albedo.texture->image->uri);

    mesh_init_cmesh(dst, &cmesh);

    cgltf_free(data);
#endif
}

