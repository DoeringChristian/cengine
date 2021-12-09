#include "model.h"

int model_load(struct model *dst, const char *path){
#if 0
    darray_init(&dst->cmeshes, 10);
    darray_init(&dst->meshes, 10);
    darray_init(&dst->textures, 10);

    cgltf_options options = {0};
    cgltf_data* data = NULL;
    cgltf_result result = cgltf_parse_file(&options, "scene.gltf", &data);
    if (result == cgltf_result_success)
    {


        for(size_t i = 0;i < data->meshes_count;i++){

        }
        return 0;
    }
    else 
        return -1;

#endif
}
void model_free(struct model *dst){
    darray_free(&dst->cmeshes);
    darray_free(&dst->meshes);
    darray_free(&dst->textures);
}
