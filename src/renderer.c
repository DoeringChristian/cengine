#include "renderer.h"

#define SHADOW_SIZE 1024

int renderer_init(struct renderer *dst, int w, int h){
    //darray_init(&dst->instances, 10);
    dst->w = w;
    dst->h = h;
    
    // loading shaders
    shader_load(&dst->shader, "shaders/vert.glsl", "shaders/frag.glsl");

    shader_load(&dst->shader_forward, "shaders/lighting/vert_ssp.glsl", "shaders/layer/frag.glsl");

    shader_load(&dst->shader_mul, "shaders/lighting/vert_ssp.glsl", "shaders/lighting/frag_mul.glsl");

    shader_load(&dst->shader_add, "shaders/lighting/vert_ssp.glsl", "shaders/lighting/frag_add.glsl");

    shader_load(&dst->shader_shadow, "shaders/lighting/vert_shadow02.glsl", "shaders/lighting/frag_shadow_02.glsl");

    shader_load(&dst->shader_lighting, "shaders/lighting/vert_ssp.glsl", "shaders/lighting/frag_lighting.glsl");

    // initializing layers
    gbuf_init(&dst->gbuf, w, h);

    layer_init(&dst->light, w, h);
    layer_init(&dst->light_sum, w, h);

    cubelayer_init(&dst->cl_shadow, SHADOW_SIZE, SHADOW_SIZE);

    // initializing camera
    cvert_init(&dst->camera, w, h, 60.0/180.0 * M_PI);

    dst->scene = NULL;
    return 0;
}
void renderer_free(struct renderer *dst){
    shader_free(&dst->shader);
    gbuf_free(&dst->gbuf);
    layer_free(&dst->light);
    layer_free(&dst->light_sum);
    shader_free(&dst->shader_forward);
    shader_free(&dst->shader_add);
    shader_free(&dst->shader);
}

int renderer_push(struct renderer *dst){
    // - clear all iverts from meshes
    // - push all iverts from instances
    // - update all meshes
    
    return 0;
}

int renderer_render(struct renderer *src){
    if(src->scene == NULL)
        return 1;
    glClearColor(0.f, 0.f, 0.f, 0.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    // render the scene to the gbuf

    gbuf_bind(&src->gbuf);
    scene_draw(src->scene, &src->camera, &src->shader);
    gbuf_unbind(&src->gbuf);

    // clear the summ of lightess maps
    layer_bind(&src->light_sum);
    layer_unbind(&src->light_sum);

    for(size_t i = 0;i < darray_len(&src->scene->lights);i++){
        // render shadow cube map
        // calculate view projection of light
        struct light *light_tmp = src->scene->lights[i];
        
        if(light_tmp->type == LIGHT_POINT){
            renderer_render_point_shadow(src, light_tmp);

            // render the light of the gbuf to the light layer
            layer_bind(&src->light);
            // render the gbuf with the previous light 
            gbuf_draw_ps(
                    &src->gbuf,
                    &src->shader_lighting,
                    &src->cl_shadow.texture,
                    &src->light_sum.textures[0],
                    light_tmp,
                    &src->camera);
            layer_unbind(&src->light);
        }

        // copy it to the light sum layer
        layer_bind(&src->light_sum);
        //layer_draw(&src->light_tmp, &src->shader_forward);
        layer_draw(&src->light, &src->shader_forward);
        layer_unbind(&src->light_sum);

    }

    // reset viewport
    glViewport(0, 0, src->w, src->h);

    // draw the lightness map
    //layer_draw_shader(&src->light_sum, &src->shader_forward);
    
    layer_draw(&src->light_sum, &src->shader_forward);

    glBindVertexArray(0);
    glUseProgram(0);
    return 0;
}


int renderer_scene_set(struct renderer *dst, struct scene *src){
    dst->scene = src;
    src->shader = &dst->shader;
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
        for(size_t j = 0;j < 6;j++){
            cubelayer_bind(&src->cl_shadow, j);
            scene_draw_shadow_depth(src->scene, &cm_cameras[j], &src->shader_shadow, light);
            cubelayer_unbind(&src->cl_shadow);
        }
        return 0;
}
