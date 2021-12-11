#include "renderer.h"

#define SHADOW_SIZE 1024

int renderer_init(struct renderer *dst, int w, int h){
    //darray_init(&dst->instances, 10);
    darray_init(&dst->meshes, 10);
    darray_init(&dst->lights, 10);
    dst->w = w;
    dst->h = h;
    
    // loading shaders
    shader_load(&dst->shader, "shaders/vert.glsl", "shaders/frag.glsl");

    shader_load(&dst->shader_forward, "shaders/lighting/vert_ssp.glsl", "shaders/layer/frag.glsl");

    shader_load(&dst->shader_shadow, "shaders/lighting/vert_shadow02.glsl", "shaders/lighting/frag_shadow_02.glsl");

    shader_load(&dst->shader_lighting, "shaders/lighting/vert_ssp.glsl", "shaders/lighting/frag_lighting.glsl");

    shader_load(&dst->shader_clip, "shaders/layer/vert.glsl", "shaders/layer/frag_clip.glsl");

    shader_load(&dst->shader_blurh, "shaders/layer/vert.glsl", "shaders/layer/frag_blurh.glsl");
    shader_load(&dst->shader_blurv,  "shaders/layer/vert.glsl", "shaders/layer/frag_blurv.glsl");

#if 0
    shader_load(&dst->shader_blurh, "shaders/layer/vert.glsl", "shaders/layer/frag_blurh.glsl");
    shader_load(&dst->shader_blurv, "shaders/layer/vert.glsl", "shaders/layer/frag_blurv.glsl");
#endif

    // initializing layers
    //gbuf_init(&dst->gbuf, w, h);
    layer_init_n(&dst->gbuf, w, h, 5);

    layer_init(&dst->light, w, h);
    layer_init(&dst->light_out, w, h);

    cubelayer_init(&dst->cl_shadow, SHADOW_SIZE, SHADOW_SIZE);

    // initializing camera
    cvert_init(&dst->camera, w, h, 60.0/180.0 * M_PI);

    return 0;
}
void renderer_free(struct renderer *dst){
    shader_free(&dst->shader);
    //gbuf_free(&dst->gbuf);
    layer_free(&dst->gbuf);
    layer_free(&dst->light);
    layer_free(&dst->light_out);
    shader_free(&dst->shader_forward);
    shader_free(&dst->shader);
    darray_free(&dst->lights);
    darray_free(&dst->meshes);
}

int renderer_render(struct renderer *src){
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    // ------------------------------------------------
    // render the scene to the gbuf

    //gbuf_bind(&src->gbuf);
    layer_bind(&src->gbuf);
    // disable blend
    GLCall(glDisable(GL_BLEND));
    //GLCall(glBlendFunc(GL_ONE, GL_ZERO));
    //GLCall(glEnable(GL_DEPTH_TEST));
    for(size_t i = 0;i < darray_len(&src->meshes);i++)
        mesh_draw(src->meshes[i], &src->camera, &src->shader);
    layer_unbind(&src->gbuf);
    //gbuf_unbind(&src->gbuf);

    // clear the summ of lightess maps

    layer_bind(&src->light_out);
    layer_unbind(&src->light_out);

    layer_bind(&src->light_out);
    // TODO: move enable/disable GL_DEPTH_TEST to draw functions.
    GLCall(glEnable(GL_BLEND));
    GLCall(glDisable(GL_DEPTH_TEST));
    GLCall(glBlendFunc(GL_ONE, GL_ONE));
    // -----------------------------------------------
    // Render lights
    for(size_t i = 0;i < darray_len(&src->lights);i++){
        // render shadow cube map
        // calculate view projection of light
        struct light *light_tmp = src->lights[i];
        
        if(light_tmp->type == LIGHT_POINT){

            renderer_render_point_shadow(src, light_tmp);
            
            layer_rebind(&src->light_out);

            layer_draw_gbuf(
                    &src->gbuf,
                    &src->shader_lighting,
                    &src->cl_shadow.texture,
                    light_tmp,
                    &src->camera);

        }
    }
    layer_unbind(&src->light_out);

    renderer_render_bloom(src);

    // reset viewport
    glViewport(0, 0, src->w, src->h);

    layer_draw(&src->light_out, &src->shader_forward);

    glBindVertexArray(0);
    glUseProgram(0);
    return 0;
}

int renderer_render_bloom(struct renderer *src){
    const size_t bloom_passes = 16;
    struct layer layers_bloom[bloom_passes];
    struct layer layer_tmp;
    layer_init(&layer_tmp, src->w, src->h);
    for(size_t i = 0;i < bloom_passes;i++){
        layer_init(&layers_bloom[i], src->w/(i*2+1), src->h/(i*2+1));
    }

    for(size_t i = 0;i < bloom_passes;i++){
        if(i == 0){
            layer_bind(&layers_bloom[0]);
            layer_draw(&src->light_out, &src->shader_clip);
            layer_unbind(&layers_bloom[0]);
        }
        else{
            layer_bind(&layers_bloom[i]);
            //layer_draw(&layers_bloom[i-1], &src->shader_blurv);
            //layer_draw(&layers_bloom[i-1], &src->shader_blurh);
            layer_draw(&layers_bloom[i-1], &src->shader_forward);
            layer_unbind(&layers_bloom[i]);
        }
    }

    layer_bind(&layer_tmp);
    layer_draw(&src->light_out, &src->shader_forward);
    for(size_t i = 0;i < bloom_passes;i++){
        //layer_draw(&layers_bloom[i], &src->shader_forward);
        layer_draw(&layers_bloom[i], &src->shader_blurh);
        layer_draw(&layers_bloom[i], &src->shader_blurv);
    }
    layer_unbind(&layer_tmp);


    layer_bind(&src->light_out);
    //layer_draw(&layers_bloom[15], &src->shader_forward);
    layer_draw(&layer_tmp, &src->shader_forward);
    layer_unbind(&src->light_out);


    layer_free(&layer_tmp);
    for(size_t i = 0;i < bloom_passes;i++){
        layer_free(&layers_bloom[i]);
    }
    return 0;
}

int renderer_render_point_shadow(struct renderer *src, struct light *light){
    struct cvert cm_cameras[6];
    cvert_init(&cm_cameras[0], 1, 1, glm_rad(90));
    glm_perspective(glm_rad(90), 1, 0.1, 100, cm_cameras[0].proj);
    glm_perspective(glm_rad(90), 1, 0.1, 100, cm_cameras[1].proj);
    glm_perspective(glm_rad(90), 1, 0.1, 100, cm_cameras[2].proj);
    glm_perspective(glm_rad(90), 1, 0.1, 100, cm_cameras[3].proj);
    glm_perspective(glm_rad(90), 1, 0.1, 100, cm_cameras[4].proj);
    glm_perspective(glm_rad(90), 1, 0.1, 100, cm_cameras[5].proj);
    cm_cameras[0].far = 100;
    cm_cameras[1].far = 100;
    cm_cameras[2].far = 100;
    cm_cameras[3].far = 100;
    cm_cameras[4].far = 100;
    cm_cameras[5].far = 100;

    glm_look(light->pos, vec3(1, 0, 0), vec3(0, -1, 0), cm_cameras[0].view);
    glm_look(light->pos, vec3(-1, 0, 0), vec3(0, -1, 0), cm_cameras[1].view);
    glm_look(light->pos, vec3(0, 1, 0), vec3(0, 0, 1), cm_cameras[2].view);
    glm_look(light->pos, vec3(0, -1, 0), vec3(0, 0, -1), cm_cameras[3].view);
    glm_look(light->pos, vec3(0, 0, 1), vec3(0, -1, 0), cm_cameras[4].view);
    glm_look(light->pos, vec3(0, 0, -1), vec3(0, -1, 0), cm_cameras[5].view);

    // calculate view projection of light
    for(size_t i = 0;i < 6;i++){
        cubelayer_bind(&src->cl_shadow, i);
        for(size_t j = 0;j < darray_len(&src->meshes);j++)
            mesh_draw_depth(src->meshes[j], &cm_cameras[i], &src->shader_shadow, light);
        //scene_draw_shadow_depth(src->scene, &cm_cameras[j], &src->shader_shadow, light);
        cubelayer_unbind(&src->cl_shadow);
    }
    return 0;
}
int renderer_mesh_register(struct renderer *dst, struct mesh *src){
    darray_push_back(&dst->meshes, src);
    return 0;
}
int renderer_mesh_unregister(struct renderer *dst, struct mesh *target){
    for(size_t i = 0;i < darray_len(&dst->meshes);i++){
        if(dst->meshes[i] == target){
            darray_pop(&dst->meshes, i);
            return 0;
        }
    }
}
int renderer_light_register(struct renderer *dst, struct light *src){
    darray_push_back(&dst->lights, src);
    return 0;
}
int renderer_light_unregister(struct renderer *dst, struct light *target){
    for(size_t i = 0;i < darray_len(&dst->lights);i++){
        if(dst->lights[i] == target){
            darray_pop(&dst->lights, i);
            return 0;
        }
    }
}
