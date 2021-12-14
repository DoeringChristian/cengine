#include "envmap.h"

int envmap_init(struct envmap *dst, int res){
    dst->res = res;
    dst->mmlvl = 5;

    dst->shader_eqr_to_cm = &primitive_shader_eqr_to_cm;
    dst->shader_irr       = &primitive_shader_irr;
    dst->shader_ref       = &primitive_shader_ref;
    dst->shader_brdf_int  = &primitive_shader_brdf_int;


    // -------------------------------------------------
    // input side.

    texture_init_cube_rgbf16(&dst->hdr, res, res, NULL);
    texture_init_cube_rgbf16(&dst->irr, res, res, NULL);
    texture_init_cube_rgbf16(&dst->ref, res, res, NULL);
    texture_init_rgbaf16(&dst->brdf_int, res, res, NULL);

    GLCall(glBindTexture(dst->ref.type, dst->ref.gl_tex))
    GLCall(glTexParameteri(dst->ref.type, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
    GLCall(glGenerateMipmap(dst->ref.type));

    GLCall(glGenFramebuffers(1, &dst->gl_fbo));
    GLCall(glGenRenderbuffers(1, &dst->gl_rbo));

    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, dst->gl_fbo));

    GLCall(glBindRenderbuffer(GL_RENDERBUFFER, dst->gl_rbo));
    GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, res, res));
    GLCall(glBindRenderbuffer(GL_RENDERBUFFER, 0));

    GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, dst->gl_rbo));

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        assert(0);

    GLuint attachments[1] = {
        GL_COLOR_ATTACHMENT0,
    };

    glDrawBuffers(1, attachments);

    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));

    return 0;

}
void envmap_free(struct envmap *dst){
    texture_free(&dst->hdr);
    texture_free(&dst->irr);
    GLCall(glDeleteFramebuffers(1, &dst->gl_fbo));
    GLCall(glDeleteRenderbuffers(1, &dst->gl_rbo));
}

int envmap_draw(struct envmap *src, struct shader *shader, struct camera *camera){
    shader_bind(shader);

    GLCall(glDisable(GL_DEPTH_TEST));
    //glEnable(GL_CULL_FACE);
    //glFrontFace(GL_CCW);

    shader_uniform_tex(shader, &src->hdr, "u_hdr");
    shader_uniform_mat4f(shader, "u_view", (float *)camera->view);
    shader_uniform_mat4f(shader, "u_proj", (float *)camera->proj);

    mesh3_draw(&mesh3_cube);

    shader_unbind(shader);
    return 0;
}

int envmap_draw_cube(){
    return 1;
}
int envmap_hdr_set(struct envmap *dst, struct texture *src){
    // bind fbo
    GLCall(glViewport(0, 0, dst->res, dst->res));
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, dst->gl_fbo));
    GLCall(glClearColor(0, 0, 0, 0));
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    struct camera cm_cameras[6];
    camera_init_cube(cm_cameras, vec3(0, 0, 0), 10);

    // -----------------------------------------------------------------------------
    // Calculate projection to cubemap of hdri
    
    for(size_t i = 0;i < 6;i++){
        GLCall(glBindFramebuffer(GL_FRAMEBUFFER, dst->gl_fbo));
        GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, dst->hdr.gl_tex, 0));

        shader_bind(dst->shader_eqr_to_cm);

        GLCall(glDisable(GL_DEPTH_TEST));

        shader_uniform_tex(dst->shader_eqr_to_cm, src, "u_texture");
        //shader_uniform_mat4f(dst->shader_eqr_to_cm, "u_view", (float *)primitive_cm_cameras_view[i]);
        //shader_uniform_mat4f(dst->shader_eqr_to_cm, "u_proj", (float *)primitive_cm_cameras_proj);
        shader_uniform_mat4f(dst->shader_eqr_to_cm, "u_view", (float *)cm_cameras[i].view);
        shader_uniform_mat4f(dst->shader_eqr_to_cm, "u_proj", (float *)cm_cameras[0].proj);

        mesh3_draw(&mesh3_cube);

        shader_unbind(dst->shader_eqr_to_cm);
    }

    // -----------------------------------------------------------------------------
    // Calculate the irradiance map.

    for(size_t i = 0;i < 6;i++){
        GLCall(glBindFramebuffer(GL_FRAMEBUFFER, dst->gl_fbo));
        GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, dst->irr.gl_tex, 0));


        shader_bind(dst->shader_irr);

        GLCall(glDisable(GL_DEPTH_TEST));

        shader_uniform_tex(dst->shader_irr, &dst->hdr, "u_hdr");
        shader_uniform_mat4f(dst->shader_irr, "u_view", (float *)cm_cameras[i].view);
        shader_uniform_mat4f(dst->shader_irr, "u_proj", (float *)cm_cameras[i].proj);

        mesh3_draw(&mesh3_cube);

        shader_unbind(dst->shader_irr);
    }

    // -----------------------------------------------------------------------------
    // Calculate the reflection map.

    int mmw = dst->res;
    int mmh = dst->res;

    texture_bind(&dst->ref, 0);
    
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, dst->gl_fbo));
    shader_bind(dst->shader_ref);
    for(size_t mip = 0;mip < dst->mmlvl;mip++){
        //GLCall(glFramebufferTexture2D());
        GLCall(glBindRenderbuffer(GL_RENDERBUFFER, dst->gl_rbo));
        GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, mmw, mmh));
        GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, dst->gl_rbo));
        GLCall(glViewport(0, 0, mmw, mmh));

        float roughness = (float)mip / (float)(dst->mmlvl - 1);
        shader_uniform_f(dst->shader_ref, "u_roughness", roughness);
        shader_uniform_tex(dst->shader_ref, &dst->hdr, "u_hdr");
        shader_uniform_i(dst->shader_ref, "u_resolution", dst->res);
        for(size_t i = 0;i < 6;i++){
            shader_uniform_mat4f(dst->shader_ref, "u_view", (float *)cm_cameras[i].view);
            shader_uniform_mat4f(dst->shader_ref, "u_proj", (float *)cm_cameras[i].proj);

            GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, dst->ref.gl_tex, mip));

            // -------------------------------------------------
            // draw cube

            mesh3_draw(&mesh3_cube);

            GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, mip));
        }
        mmw *= 0.5;
        mmh *= 0.5;

    }
    shader_unbind(dst->shader_ref);
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));

    GLCall(glBindTexture(dst->irr.type, dst->irr.gl_tex));
    GLCall(glGenerateMipmap(dst->irr.type));

    // -----------------------------------------------------------------------------
    // Calculate the brdf integral LUT.
    envmap_calc_brdf_lut(dst);

    return 0;
}
int envmap_calc_brdf_lut(struct envmap *dst){

    struct glbuf ibo;
    struct glbuf vbo;
    GLuint vao;

    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, dst->gl_fbo));

    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, dst->gl_fbo));
    GLCall(glBindRenderbuffer(GL_RENDERBUFFER, dst->gl_rbo));
    GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, dst->res, dst->res));
    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, dst->brdf_int.gl_tex, 0));

    GLCall(glViewport(0, 0, dst->res, dst->res));

    shader_bind(dst->shader_brdf_int);

    mesh2_draw(&mesh2_quad);

    shader_unbind(dst->shader_brdf_int);


    GLCall(glDeleteVertexArrays(1, &vao));

    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    GLCall(glClearColor(0, 0, 0, 0));
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    return 0;
}
