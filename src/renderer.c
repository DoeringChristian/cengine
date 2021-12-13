#include "renderer.h"
#include "gl_util.h"

#define SHADOW_SIZE 1024

int renderer_init(struct renderer *dst, int w, int h){
    if(!primitives_initialized)
        assert(0);

    darray_init(&dst->meshes, 10);
    darray_init(&dst->lights, 10);
    dst->w = w;
    dst->h = h;
    const int oversample = 2;
    
    // setting shaders to primitive shaders.

    dst->shader          = &primitive_shader_gbuf;
    dst->shader_forward  = &primitive_shader_quad_forward;
    dst->shader_shadow   = &primitive_shader_shadow;
    dst->shader_lighting = &primitive_shader_lighting;
    dst->shader_emission = &primitive_shader_emission;
    dst->shader_ambient  = &primitive_shader_ambient;
    dst->shader_clip     = &primitive_shader_clip;
    dst->shader_blurh    = &primitive_shader_blurh;
    dst->shader_blurv    = &primitive_shader_blurv;
    dst->shader_gamma    = &primitive_shader_gamma;
    dst->shader_skybox   = &primitive_shader_skybox;


    // initializing layers
    //gbuf_init(&dst->gbuf, w, h);
    layer_init_n(&dst->gbuf, w*oversample, h*oversample, 5);

    //layer_init(&dst->light, w, h);
    layer_init(&dst->light_out, w*oversample, h*oversample);
    layer_init(&dst->layer_bloom, w, h);

    cubelayer_init_depthcube(&dst->cl_shadow, SHADOW_SIZE, SHADOW_SIZE);
    //cubelayer_init_rgbf16(&dst->cl_hdr, 512, 512);
    envmap_init(&dst->environment, 512);

    // initializing camera
    cvert_init(&dst->camera, w, h, 60.0/180.0 * M_PI);

    return 0;
}
void renderer_free(struct renderer *dst){
    //gbuf_free(&dst->gbuf);
    layer_free(&dst->gbuf);
    //layer_free(&dst->light);
    layer_free(&dst->light_out);
    layer_free(&dst->layer_bloom);

    darray_free(&dst->lights);
    darray_free(&dst->meshes);
}

int renderer_render(struct renderer *src){
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    // ------------------------------------------------
    // render the scene to the gbuf
    layer_bind(&src->gbuf);
    GLCall(glDisable(GL_BLEND));
    for(size_t i = 0;i < darray_len(&src->meshes);i++)
        mesh_render(src->meshes[i], &src->camera, src->shader);
    layer_unbind(&src->gbuf);

    // clear the summ of lights maps

    layer_bind(&src->light_out);
    layer_unbind(&src->light_out);

    layer_bind(&src->light_out);
    // TODO: move enable/disable GL_DEPTH_TEST to draw functions.
    GLCall(glEnable(GL_BLEND));
    GLCall(glEnable(GL_DEPTH_TEST));

    //GLCall(glBlendFunc(GL_ONE, GL_ONE));
    GLCall(glBlendFuncSeparate(GL_ONE, GL_ONE, GL_ZERO, GL_ZERO));

    // -------------------------------------------------
    // Render emission.

    //layer_draw_gbuf(&src->gbuf, src->shader_emission, NULL, NULL, NULL, &src->camera);

    // -------------------------------------------------
    // Render ambient.
    
    layer_draw_gbuf_ambient(&src->gbuf, src->shader_ambient, &src->environment, &src->camera);

#if 1
    // -----------------------------------------------
    // Render lights
    for(size_t i = 0;i < darray_len(&src->lights);i++){
        // render shadow cube map
        // calculate view projection of light
        struct light *light_tmp = src->lights[i];
        
        if(light_tmp->type == LIGHT_POINT){
            if(light_tmp->casts_shadow){
                renderer_render_point_shadow(src, light_tmp);

                layer_rebind(&src->light_out);

                // TODO: more efficient skybox.
                GLCall(glBlendFuncSeparate(GL_ONE, GL_ONE, GL_ONE, GL_ONE));
                GLCall(glBlendEquationSeparate(GL_FUNC_ADD, GL_MAX));
                layer_draw_gbuf(
                        &src->gbuf,
                        src->shader_lighting,
                        &src->cl_shadow.texture,
                        light_tmp,
                        &src->environment,
                        &src->camera);
            }
            else{
                // TODO:
            }

        }
    }


#endif
    // -------------------------------------------------
    // Render skybox: TODO: render skybox.
    GLCall(glBlendFuncSeparate(GL_ONE_MINUS_DST_ALPHA, GL_DST_ALPHA, GL_ONE, GL_ZERO));
    //GLCall(glBlendFunc(GL_ZERO, GL_ONE));

    envmap_draw(&src->environment, src->shader_skybox, &src->camera);

    layer_unbind(&src->light_out);




#if 1
    renderer_render_bloom(src);

    layer_bind(&src->light_out);
    shader_uniform_f(src->shader_gamma, "u_gamma", 2.2);
    layer_draw(&src->layer_bloom, src->shader_gamma);
    layer_unbind(&src->light_out);
#endif


    // reset viewport
    glViewport(0, 0, src->w, src->h);

    layer_draw(&src->light_out, src->shader_forward);

    glBindVertexArray(0);
    glUseProgram(0);
    return 0;
}

int renderer_render_bloom(struct renderer *src){
    const size_t bloom_passes = 0;
    struct layer layers_bloom[bloom_passes];
    //struct layer layer_tmp;
    //layer_init(&layer_tmp, src->w, src->h);
    for(size_t i = 0;i < bloom_passes;i++){
        layer_init(&layers_bloom[i], src->w/(i*2+1), src->h/(i*2+1));
    }

    for(size_t i = 0;i < bloom_passes;i++){
        if(i == 0){
            layer_bind(&layers_bloom[0]);
            layer_draw(&src->light_out, src->shader_clip);
            layer_unbind(&layers_bloom[0]);
        }
        else{
            layer_bind(&layers_bloom[i]);
            layer_draw(&layers_bloom[i-1], src->shader_blurv);
            layer_draw(&layers_bloom[i-1], src->shader_blurh);
            layer_unbind(&layers_bloom[i]);
        }
    }

    layer_bind(&src->layer_bloom);
    layer_draw(&src->light_out, src->shader_forward);
    for(size_t i = 0;i < bloom_passes;i++){
        layer_draw(&layers_bloom[i], src->shader_forward);
    }
    layer_unbind(&src->layer_bloom);



    //layer_free(&layer_tmp);
    for(size_t i = 0;i < bloom_passes;i++){
        layer_free(&layers_bloom[i]);
    }
    return 0;
}

int renderer_render_point_shadow(struct renderer *src, struct light *light){
    struct cvert cm_cameras[6];
    cverts_init_cube(cm_cameras, light->pos, light->shadow_len);

    // calculate view projection of light
    for(size_t i = 0;i < 6;i++){
        cubelayer_bind(&src->cl_shadow, i);

        for(size_t j = 0;j < darray_len(&src->meshes);j++)
            mesh_render_depth(src->meshes[j], &cm_cameras[i], src->shader_shadow, light);
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
    return 0;
}
int renderer_envmap_set(struct renderer *dst, struct texture *hdri){
    envmap_hdr_set(&dst->environment, hdri);
    return 0;
}
