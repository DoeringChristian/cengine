#include "envmap.h"

int envmap_init(struct envmap *dst, int res){
    dst->res = res;
    dst->mmlvl = 5;

    shader_load(&dst->shader_eqr_to_cm, "shaders/layer/vert_cm.glsl", "shaders/layer/frag_eqr_to_cm.glsl");
    shader_load(&dst->shader_irr, "shaders/layer/vert_cm.glsl", "shaders/layer/frag_irr.glsl");
    shader_load(&dst->shader_ref, "shaders/layer/vert_cm.glsl", "shaders/layer/frag_ref.glsl");
    shader_load(&dst->shader_brdf_int, "shaders/layer/vert.glsl", "shaders/layer/frag_brdf_int.glsl");
    shader_load(&dst->shader_cm_pv, "shaders/layer/vert_cm_rot.glsl", "shaders/layer/frag_ref_copy.glsl");

    // -------------------------------------------------
    // input side.

    texture_init_cube_rgbf16(&dst->hdr_s, res, res, NULL);
    texture_init_cube_rgbf16(&dst->irr_s, res, res, NULL);
    texture_init_cube_rgbf16(&dst->ref_s, res, res, NULL);
    texture_init_cube_rgbf16(&dst->irr, res, res, NULL);
    texture_init_cube_rgbf16(&dst->ref, res, res, NULL);
    texture_init_rgbaf16(&dst->brdf_int, res, res, NULL);

    GLCall(glBindTexture(dst->ref_s.type, dst->ref_s.gl_tex))
    GLCall(glTexParameteri(dst->ref_s.type, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
    GLCall(glGenerateMipmap(dst->ref_s.type));

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


    // -------------------------------------------------
    // output side

    float verts[] = {
        // front
        -1.0, -1.0,  1.0,
        1.0, -1.0,  1.0,
        1.0,  1.0,  1.0,
        -1.0,  1.0,  1.0,
        // back
        -1.0, -1.0, -1.0,
        1.0, -1.0, -1.0,
        1.0,  1.0, -1.0,
        -1.0,  1.0, -1.0
    };
    int idxs[] = {
        // front
        2, 1, 0,
        0, 3, 2,
        // right
        6, 5, 1,
        1, 2, 6,
        // back
        5, 6, 7,
        7, 4, 5,
        // left
        3, 0, 4,
        4, 7, 3,
        // bottom
        1, 5, 4,
        4, 0, 1,
        // top
        6, 2, 3,
        3, 7, 6
    };

    GLCall(glGenVertexArrays(1, &dst->gl_vao));
    GLCall(glBindVertexArray(dst->gl_vao));

    glbuf_init(&dst->vbo, verts, sizeof(verts), GL_ARRAY_BUFFER, GL_STATIC_DRAW);

    glbuf_init(&dst->ibo, idxs, sizeof(idxs), GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);

    glbuf_bind(&dst->vbo);

    int idx = 0;
    GLCall(glEnableVertexAttribArray(0));
    GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, 0, sizeof(float) * 3, 0));
    //vao_attr_push_inc(idx, GL_FLOAT, 0, struct svert, uv);

    glbuf_unbind(&dst->vbo);

    GLCall(glBindVertexArray(0));

    return 0;

}
void envmap_free(struct envmap *dst){
    texture_free(&dst->hdr_s);
    texture_free(&dst->irr_s);
    glbuf_free(&dst->ibo);
    glbuf_free(&dst->vbo);
    GLCall(glDeleteVertexArrays(1, &dst->gl_vao));
    GLCall(glDeleteFramebuffers(1, &dst->gl_fbo));
    GLCall(glDeleteRenderbuffers(1, &dst->gl_rbo));
}

int envmap_draw(struct envmap *src, struct shader *shader, struct cvert *camera){
    shader_bind(shader);

    GLCall(glDisable(GL_DEPTH_TEST));
    //glEnable(GL_CULL_FACE);
    //glFrontFace(GL_CCW);

    shader_uniform_tex(shader, &src->ref_s, "u_hdr");
    shader_uniform_mat4f(shader, "u_view", (float *)camera->view);
    shader_uniform_mat4f(shader, "u_proj", (float *)camera->proj);

    GLCall(glBindVertexArray(src->gl_vao));

    glbuf_bind(&src->ibo);

    GLCall(glDrawElements(GL_TRIANGLES, 12 * 3, GL_UNSIGNED_INT, NULL));

    glbuf_unbind(&src->ibo);

    GLCall(glBindVertexArray(0));

    shader_unbind(shader);
    return 0;
}
int envmap_update(struct envmap *dst, struct cvert *camera){

    struct cvert cm_cameras[6];
    cvert_init(&cm_cameras[0], 1, 1, glm_rad(90));
    glm_perspective(glm_rad(90), 1, 0.1, 100, cm_cameras[0].proj);
    glm_perspective(glm_rad(90), 1, 0.1, 100, cm_cameras[1].proj);
    glm_perspective(glm_rad(90), 1, 0.1, 100, cm_cameras[2].proj);
    glm_perspective(glm_rad(90), 1, 0.1, 100, cm_cameras[3].proj);
    glm_perspective(glm_rad(90), 1, 0.1, 100, cm_cameras[4].proj);
    glm_perspective(glm_rad(90), 1, 0.1, 100, cm_cameras[5].proj);
    cm_cameras[0].far = 10;
    cm_cameras[1].far = 10;
    cm_cameras[2].far = 10;
    cm_cameras[3].far = 10;
    cm_cameras[4].far = 10;
    cm_cameras[5].far = 10;

    glm_look(vec3(0, 0, 0), vec3(1, 0, 0), vec3(0, -1, 0), cm_cameras[0].view);
    glm_look(vec3(0, 0, 0), vec3(-1, 0, 0), vec3(0, -1, 0), cm_cameras[1].view);
    glm_look(vec3(0, 0, 0), vec3(0, 1, 0), vec3(0, 0, 1), cm_cameras[2].view);
    glm_look(vec3(0, 0, 0), vec3(0, -1, 0), vec3(0, 0, -1), cm_cameras[3].view);
    glm_look(vec3(0, 0, 0), vec3(0, 0, 1), vec3(0, -1, 0), cm_cameras[4].view);
    glm_look(vec3(0, 0, 0), vec3(0, 0, -1), vec3(0, -1, 0), cm_cameras[5].view);

    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, dst->gl_fbo));
    GLCall(glBindRenderbuffer(GL_RENDERBUFFER, dst->gl_rbo));
    GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, dst->res, dst->res));
    GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, dst->gl_rbo));
    GLCall(glViewport(0, 0, dst->res, dst->res));
    GLCall(glClearColor(0, 0, 0, 0));
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    GLCall(glBlendFunc(GL_ONE, GL_ZERO));

#if 0
    // rotate the cubemap cameras.
    mat3 camera_view, cm_cameras_view, mat3_tmp;
    glm_mat4_pick3(camera->view, camera_view);
    for(size_t i = 0;i < 6;i++){
        glm_mat4_pick3(cm_cameras[i].view, mat3_tmp);
        glm_mat3_mul(camera_view, mat3_tmp, cm_cameras_view);
        glm_mat4_ins3(cm_cameras_view, cm_cameras[i].view);
    }
#endif

    int mmw = dst->res;
    int mmh = dst->res;
    for(size_t mip = 0;mip < dst->mmlvl;mip++){
        GLCall(glBindRenderbuffer(GL_RENDERBUFFER, dst->gl_rbo));
        GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, mmw, mmh));
        GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, dst->gl_rbo));
        GLCall(glViewport(0, 0, mmw, mmh));
        for(size_t i = 0;i < 6;i++){
            GLCall(glBindFramebuffer(GL_FRAMEBUFFER, dst->gl_fbo));
            GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, dst->ref.gl_tex, mip));

            shader_bind(&dst->shader_cm_pv);
            GLCall(glDisable(GL_DEPTH_TEST));

            shader_uniform_tex(&dst->shader_cm_pv, &dst->ref_s, "u_cubemap");
            shader_uniform_mat4f(&dst->shader_cm_pv, "u_view", (float *)cm_cameras[i].view);
            shader_uniform_mat4f(&dst->shader_cm_pv, "u_proj", (float *)cm_cameras[i].proj);
            shader_uniform_mat4f(&dst->shader_cm_pv, "u_cview", (float *)camera->view);
            shader_uniform_i(&dst->shader_cm_pv, "u_mm", mip);

            GLCall(glBindVertexArray(dst->gl_vao));

            glbuf_bind(&dst->ibo);

            GLCall(glDrawElements(GL_TRIANGLES, 12 * 3, GL_UNSIGNED_INT, NULL));

            GLCall(glBindVertexArray(0));
            shader_unbind(&dst->shader_cm_pv);
            GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, mip));
        }
        mmw *= 0.5;
        mmh *= 0.5;
    }

#if 0
    shader_bind(&dst->shader_cm_pv);

    GLCall(glClearColor(0, 0, 0, 0));
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
    GLCall(glBlendFunc(GL_ONE, GL_ZERO));

    for(size_t i = 0;i < 6;i++){
        GLCall(glBindFramebuffer(GL_FRAMEBUFFER, dst->gl_fbo));
        GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, dst->irr.gl_tex, 0));


        GLCall(glDisable(GL_DEPTH_TEST));

        shader_uniform_tex(&dst->shader_cm_pv, &dst->irr_s, "u_cubemap");
        shader_uniform_mat4f(&dst->shader_cm_pv, "u_view", (float *)cm_cameras[i].view);
        shader_uniform_mat4f(&dst->shader_cm_pv, "u_proj", (float *)cm_cameras[i].proj);
        shader_uniform_mat4f(&dst->shader_cm_pv, "u_cview", (float *)camera->view);

        GLCall(glBindVertexArray(dst->gl_vao));

        glbuf_bind(&dst->ibo);

        GLCall(glDrawElements(GL_TRIANGLES, 12 * 3, GL_UNSIGNED_INT, NULL));

        glbuf_unbind(&dst->ibo);

        GLCall(glBindVertexArray(0));
    }

    shader_unbind(&dst->shader_cm_pv);
#endif
    return 0;
}

int envmap_draw_cube(){
#if 0
    GLuint tmp_vao;
    struct glbuf tmp_vbo;
    struct glbuf tmp_ibo;

    GLCall(glGenVertexArrays(1, &tmp_vao));
    GLCall(glBindVertexArray(tmp_vao));

    float cube[] = {
        -1, -1, -1,
        -1, -1, 1,
        -1, 1, -1,
        -1, 1, 1,
        1, -1, -1, 
        1, -1, 1,
        1, 1, -1,
        1, 1, 1,
    };
    int cube_idxs[] = {
        // front
        0, 1, 2,
        2, 3, 0,
        // right
        1, 5, 6,
        6, 2, 1,
        // back
        7, 6, 5,
        5, 4, 7,
        // left
        4, 0, 3,
        3, 7, 4,
        // bottom
        4, 5, 1,
        1, 0, 4,
        // top
        3, 2, 6,
        6, 7, 3
    };

    glbuf_init(&tmp_vbo, cube, sizeof(cube), GL_ARRAY_BUFFER, GL_STATIC_DRAW);

    glbuf_init(&tmp_ibo, cube_idxs, sizeof(cube_idxs), GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);

    glbuf_bind(&tmp_ibo);

    int idx = 0;

    GLCall(glEnableVertexAttribArray(0));
    GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, 0, sizeof(float) * 3, 0));

    GLCall(glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, NULL));

    glbuf_unbind(&tmp_ibo);

    GLCall(glBindVertexArray(0));

    glbuf_free(&tmp_ibo);
    glbuf_free(&tmp_vbo);
    GLCall(glDeleteVertexArrays(1, &tmp_vao));
#endif
    return 1;
}
int envmap_hdr_set(struct envmap *dst, struct texture *src){
    // bind fbo
    GLCall(glViewport(0, 0, dst->res, dst->res));
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, dst->gl_fbo));
    GLCall(glClearColor(0, 0, 0, 0));
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));


    struct cvert cm_cameras[6];
    cvert_init(&cm_cameras[0], 1, 1, glm_rad(90));
    glm_perspective(glm_rad(90), 1, 0.1, 100, cm_cameras[0].proj);
    glm_perspective(glm_rad(90), 1, 0.1, 100, cm_cameras[1].proj);
    glm_perspective(glm_rad(90), 1, 0.1, 100, cm_cameras[2].proj);
    glm_perspective(glm_rad(90), 1, 0.1, 100, cm_cameras[3].proj);
    glm_perspective(glm_rad(90), 1, 0.1, 100, cm_cameras[4].proj);
    glm_perspective(glm_rad(90), 1, 0.1, 100, cm_cameras[5].proj);
    cm_cameras[0].far = 10;
    cm_cameras[1].far = 10;
    cm_cameras[2].far = 10;
    cm_cameras[3].far = 10;
    cm_cameras[4].far = 10;
    cm_cameras[5].far = 10;

    glm_look(vec3(0, 0, 0), vec3(1, 0, 0), vec3(0, -1, 0), cm_cameras[0].view);
    glm_look(vec3(0, 0, 0), vec3(-1, 0, 0), vec3(0, -1, 0), cm_cameras[1].view);
    glm_look(vec3(0, 0, 0), vec3(0, 1, 0), vec3(0, 0, 1), cm_cameras[2].view);
    glm_look(vec3(0, 0, 0), vec3(0, -1, 0), vec3(0, 0, -1), cm_cameras[3].view);
    glm_look(vec3(0, 0, 0), vec3(0, 0, 1), vec3(0, -1, 0), cm_cameras[4].view);
    glm_look(vec3(0, 0, 0), vec3(0, 0, -1), vec3(0, -1, 0), cm_cameras[5].view);

    // -------------------------------------------------
    // calculate view projection of light
    for(size_t i = 0;i < 6;i++){
        GLCall(glBindFramebuffer(GL_FRAMEBUFFER, dst->gl_fbo));
        GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, dst->hdr_s.gl_tex, 0));


        shader_bind(&dst->shader_eqr_to_cm);

        GLCall(glDisable(GL_DEPTH_TEST));

        shader_uniform_tex(&dst->shader_eqr_to_cm, src, "u_texture");
        shader_uniform_mat4f(&dst->shader_eqr_to_cm, "u_view", (float *)cm_cameras[i].view);
        shader_uniform_mat4f(&dst->shader_eqr_to_cm, "u_proj", (float *)cm_cameras[i].proj);

        GLCall(glBindVertexArray(dst->gl_vao));

        glbuf_bind(&dst->ibo);

        GLCall(glDrawElements(GL_TRIANGLES, 12 * 3, GL_UNSIGNED_INT, NULL));

        glbuf_unbind(&dst->ibo);

        GLCall(glBindVertexArray(0));

        shader_unbind(&dst->shader_eqr_to_cm);
    }

    // -------------------------------------------------
    // Calculate the irradiance map.

    for(size_t i = 0;i < 6;i++){
        GLCall(glBindFramebuffer(GL_FRAMEBUFFER, dst->gl_fbo));
        GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, dst->irr_s.gl_tex, 0));


        shader_bind(&dst->shader_irr);

        GLCall(glDisable(GL_DEPTH_TEST));

        shader_uniform_tex(&dst->shader_irr, &dst->hdr_s, "u_hdr");
        shader_uniform_mat4f(&dst->shader_irr, "u_view", (float *)cm_cameras[i].view);
        shader_uniform_mat4f(&dst->shader_irr, "u_proj", (float *)cm_cameras[i].proj);

        GLCall(glBindVertexArray(dst->gl_vao));

        glbuf_bind(&dst->ibo);

        GLCall(glDrawElements(GL_TRIANGLES, 12 * 3, GL_UNSIGNED_INT, NULL));

        glbuf_unbind(&dst->ibo);

        GLCall(glBindVertexArray(0));

        shader_unbind(&dst->shader_irr);
    }

    // -------------------------------------------------
    // Calculate reflection map. TODO: why do not all faces render?
    int mmw = dst->res;
    int mmh = dst->res;

    texture_bind(&dst->ref_s, 0);

    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, dst->gl_fbo));
    shader_bind(&dst->shader_ref);
    for(size_t mip = 0;mip < dst->mmlvl;mip++){
        //GLCall(glFramebufferTexture2D());
        GLCall(glBindRenderbuffer(GL_RENDERBUFFER, dst->gl_rbo));
        GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, mmw, mmh));
        GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, dst->gl_rbo));
        GLCall(glViewport(0, 0, mmw, mmh));

        float roughness = (float)mip / (float)(dst->mmlvl - 1);
        shader_uniform_f(&dst->shader_ref, "u_roughness", roughness);
        shader_uniform_tex(&dst->shader_ref, &dst->hdr_s, "u_hdr");
        shader_uniform_i(&dst->shader_ref, "u_resolution", dst->res);
        for(size_t i = 0;i < 6;i++){
            shader_uniform_mat4f(&dst->shader_ref, "u_view", (float *)cm_cameras[i].view);
            shader_uniform_mat4f(&dst->shader_ref, "u_proj", (float *)cm_cameras[i].proj);

            GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, dst->ref_s.gl_tex, mip));

            // -------------------------------------------------
            // draw cube

            //GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
            GLCall(glBindVertexArray(dst->gl_vao));

            glbuf_bind(&dst->ibo);

            GLCall(glDrawElements(GL_TRIANGLES, 12 * 3, GL_UNSIGNED_INT, NULL));

            glbuf_unbind(&dst->ibo);

            GLCall(glBindVertexArray(0));

            GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, mip));
        }
        mmw *= 0.5;
        mmh *= 0.5;

    }
    shader_unbind(&dst->shader_ref);
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));

    // -------------------------------------------------
    // Calculate the brdf integration map.
    struct glbuf ibo;
    struct glbuf vbo;
    GLuint vao;

    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, dst->gl_fbo));

    // TODO: plane draw has to be moved.
    float verts[] = {
        -1, -1,
        -1, 1,
        1, -1, 
        1, 1,
    };
    int idxs[] = {
        0, 1, 2,
        2, 3, 1,
    };

    GLCall(glGenVertexArrays(1, &vao));
    GLCall(glBindVertexArray(vao));

    glbuf_init(&vbo, verts, sizeof(verts), GL_ARRAY_BUFFER, GL_STATIC_DRAW);

    glbuf_init(&ibo, idxs, sizeof(idxs), GL_ELEMENT_ARRAY_BUFFER, GL_STATIC_DRAW);

    glbuf_bind(&vbo);

    int idx = 0;
    GLCall(glEnableVertexAttribArray(0));
    GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, 0, sizeof(float) * 2, 0));
    //vao_attr_push_inc(idx, GL_FLOAT, 0, struct svert, uv);

    glbuf_unbind(&vbo);

    GLCall(glBindVertexArray(0));


    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, dst->gl_fbo));
    GLCall(glBindRenderbuffer(GL_RENDERBUFFER, dst->gl_rbo));
    GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, dst->res, dst->res));
    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, dst->brdf_int.gl_tex, 0));

    GLCall(glViewport(0, 0, dst->res, dst->res));

    shader_bind(&dst->shader_brdf_int);


    GLCall(glBindVertexArray(vao));

    glbuf_bind(&ibo);

    GLCall(glDrawElements(GL_TRIANGLES, 4 * 3, GL_UNSIGNED_INT, NULL));

    glbuf_unbind(&ibo);

    GLCall(glBindVertexArray(0));

    shader_unbind(&dst->shader_brdf_int);


    GLCall(glDeleteVertexArrays(1, &vao));

    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    GLCall(glClearColor(0, 0, 0, 0));
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

    return 0;
}
