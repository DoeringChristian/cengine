#include "layer.h"

int layer_init(struct layer *dst, int w, int h){
    return layer_init_n(dst, w, h, 1);
}
int layer_init_n(struct layer *dst, int w, int h, int num_textures){
    return layer_init_nm(dst, w, h, num_textures, 0);
}
int layer_init_nm(struct layer *dst, int w, int h, int num_textures, GLsizei mmlvl){
    if(num_textures > 16)
        return -1;
    dst->w = w;
    dst->h = h;
    dst->mip_bound = 0;
    darray_init(&dst->textures, num_textures);

    // -----------------------------------------
    // input side
    
    GLCall(glGenFramebuffers(1, &dst->gl_fbo));
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, dst->gl_fbo));

    // push textures
    struct texture texture;
    for(size_t i = 0;i < num_textures;i++){
        // not shure weather to use f16 or f32
        texture_init_rgbaf16_m(&texture, w, h, NULL, mmlvl);
        //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, texture.gl_tex, 0);
        darray_push_back(&dst->textures, texture);
    }

    GLCall(glGenRenderbuffers(1, &dst->gl_rbo));
    GLCall(glBindRenderbuffer(GL_RENDERBUFFER, dst->gl_rbo));
    GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, w, h));
    GLCall(glBindRenderbuffer(GL_RENDERBUFFER, 0));

    GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, dst->gl_rbo));

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        return -1;

    GLuint attachments[num_textures];

    for(size_t i = 0;i < num_textures;i++){
        attachments[i] = GL_COLOR_ATTACHMENT0 + i;
    }

    glDrawBuffers(num_textures, attachments);

    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));

    return 0;
}
void layer_free(struct layer *dst){
    for(size_t i = 0;i < darray_len(&dst->textures);i++)
        texture_free(&dst->textures[i]);
    darray_free(&dst->textures);
    GLCall(glDeleteFramebuffers(1, &dst->gl_fbo));
    GLCall(glDeleteRenderbuffers(1, &dst->gl_rbo));
}
int layer_bind(struct layer *dst){
    return layer_bind_m(dst, 0);
}
int layer_bind_m(struct layer *dst, GLsizei mip){
    dst->mip_bound = mip;
    GLCall(glViewport(0, 0, dst->w, dst->h));
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, dst->gl_fbo));

    for(size_t i = 0;i < darray_len(&dst->textures);i++)
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, dst->textures[i].gl_tex, mip);

    GLCall(glClearColor(0, 0, 0, 0));
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_ONE, GL_ONE));
    GLCall(glBlendEquation(GL_FUNC_ADD));
    return 0;
}
int layer_rebind(struct layer *dst){
    GLCall(glViewport(0, 0, dst->w, dst->h));
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, dst->gl_fbo));
    GLCall(glDisable(GL_DEPTH_TEST));
    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_ONE, GL_ONE));
    GLCall(glBlendEquation(GL_FUNC_ADD));
    //GLCall(glEnable(GL_DEPTH_TEST));
    //GLCall(glEnable(GL_BLEND));
    return 0;
}
int layer_unbind(struct layer *dst){
    for(size_t i = 0;i < darray_len(&dst->textures);i++)
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, 0, 0);
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    GLCall(glClearColor(0, 0, 0, 0));
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    //GLCall(glDisable(GL_BLEND));
    //glBlendFunc(GL_ONE, GL_ZERO);
    return 0;
}
int layer_draw(struct layer *src, struct shader *shader){
    if(shader == NULL)
        return -1;

    GLCall(glDisable(GL_DEPTH_TEST));

    shader_bind(shader);

    shader_uniform_tex(shader, &src->textures[0], "u_texture");
    //texture_bind(&src->textures[0], 0);
    //shader_uniform_i(shader, "u_texture", 0);

    mesh2_draw(&mesh2_quad);

    shader_unbind(shader);
    return 0;
}
int layer_draw_n(struct layer *src, struct shader *shader){
    shader_bind(shader);

    for(size_t i = 0;i < darray_len(&src->textures);i++){
        char buf[100];
        sprintf(buf, "u_texture[%zu]", i);
        shader_uniform_tex(shader, &src->textures[i], buf);
        //texture_bind(&src->textures[i], i);
        //shader_uniform_i(shader, buf, i);
    }

    mesh2_draw(&mesh2_quad);

    shader_unbind(shader);
    return 0;
}
int layer_draw_gbuf(struct layer *src, struct shader *shader, struct texture *shadow_depth, struct light *light, struct envmap *env, struct cvert *camera){
    if(darray_len(&src->textures) < 5)
        return -1;

    GLCall(glDisable(GL_DEPTH_TEST));

    shader_bind(shader);

    // set textures
    shader_uniform_tex(shader, &src->textures[0], "u_pos");
    shader_uniform_tex(shader, &src->textures[1], "u_normal");
    shader_uniform_tex(shader, &src->textures[2], "u_albedo");
    shader_uniform_tex(shader, &src->textures[3], "u_mrao");
    shader_uniform_tex(shader, &src->textures[4], "u_emission");
    if(shadow_depth != NULL)
        shader_uniform_tex(shader, shadow_depth, "u_shadow_depth");

    // set light parameters
    if(light != NULL){
        shader_uniform_vec4f(shader, "u_light_pos", light->pos);
        shader_uniform_vec4f(shader, "u_light_color", light->color);
        shader_uniform_f(shader, "u_shadow_len", light->shadow_len);
        shader_uniform_i(shader, "u_ref_lod_max", env->mmlvl);
    }

    // set environment parameters:
    if(env != NULL){
        //shader_uniform_tex(shader, &env->brdf_int, "u_brdf");
        //shader_uniform_tex(shader, &env->ref, "u_ref");
    }

    mat4 view_inv;
    glm_mat4_inv(camera->view, view_inv);
    shader_uniform_vec4f(shader, "u_view_pos", view_inv[3]);

    mesh2_draw(&mesh2_quad);

    shader_unbind(shader);
    return 0;
}
int layer_draw_gbuf_ambient(struct layer *src, struct shader *shader, struct envmap *env, struct cvert *camera){
    if(darray_len(&src->textures) < 5)
        return -1;

    GLCall(glDisable(GL_DEPTH_TEST));

    shader_bind(shader);

    // set textures
    shader_uniform_tex(shader, &src->textures[0], "u_pos");
    shader_uniform_tex(shader, &src->textures[1], "u_normal");
    shader_uniform_tex(shader, &src->textures[2], "u_albedo");
    shader_uniform_tex(shader, &src->textures[3], "u_mrao");
    shader_uniform_tex(shader, &src->textures[4], "u_emission");

    if(env != NULL){
        //shader_uniform_tex(shader, &env->hdr, "u_hdr");
        shader_uniform_tex(shader, &env->irr, "u_irr");
        shader_uniform_tex(shader, &env->brdf_int, "u_brdf");
        shader_uniform_tex(shader, &env->ref, "u_ref");
    }

    // set camera parameters
    mat4 view_inv;
    glm_mat4_inv(camera->view, view_inv);

    shader_uniform_vec4f(shader, "u_view_pos", view_inv[3]);

    mesh2_draw(&mesh2_quad);

    shader_unbind(shader);
    return 0;

}
int layer_draw_tex(struct layer *dst, struct shader *shader, struct texture *tex){
    if(shader == NULL)
        return -1;

    shader_bind(shader);

    texture_bind(&dst->textures[0], 0);
    shader_uniform_i(shader, "u_texture1", 0);

    texture_bind(tex, 1);
    shader_uniform_i(shader, "u_texture2", 1);

    mesh2_draw(&mesh2_quad);

    shader_unbind(shader);
    return 0;
}
int layer_blend(struct layer *dst, struct layer *src1, struct layer *src2, struct shader *bshader){
    shader_bind(bshader);

    layer_bind(dst);

    texture_bind(&src1->textures[0], 0);
    shader_uniform_i(bshader, "u_texture1", 0);

    texture_bind(&src2->textures[0], 1);
    shader_uniform_i(bshader, "u_texture2", 1);

    mesh2_draw(&mesh2_quad);

    layer_unbind(dst);

    shader_unbind(bshader);
    return 0;
}
int layer_clear(struct layer *dst){
    float color_black[4] = {
        0, 0, 0, 0,
    };
    for(size_t i = 0;i < darray_len(&dst->textures);i++)
        texture_fill(&dst->textures[0], color_black);

    return 0;
}
