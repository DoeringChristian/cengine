#include "mesh.h"

int mesh_init(struct mesh *dst, struct vert *verts, size_t verts_len, struct tri *tris, size_t tris_len){
    darray_init(&dst->textures, 2);
#if 0
    darray_init(&dst->tris, 100);
    darray_init(&dst->verts, 100);
    darray_init(&dst->iverts, 10);
#endif
    //dst->shader = shader;

    // vertex array, vertex buffer and index buffer.

    GLCall(glGenVertexArrays(1, &dst->gl_vao));
    GLCall(glBindVertexArray(dst->gl_vao));

    glbuf_init(&dst->vbo, verts, verts_len * sizeof(struct vert), GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW);

    glbuf_init(&dst->ibo, tris, tris_len * sizeof(struct tri), GL_ELEMENT_ARRAY_BUFFER, GL_DYNAMIC_DRAW);

    glbuf_bind(&dst->vbo);

#if 1
    int idx = 0;
    vao_attr_push_inc(idx, GL_FLOAT, 0, struct vert, pos);
    vao_attr_push_inc(idx, GL_FLOAT, 0, struct vert, normal);
    vao_attr_push_inc(idx, GL_FLOAT, 0, struct vert, color);
    vao_attr_push_inc(idx, GL_FLOAT, 0, struct vert, uv);

#else

    struct shader dummy = {
        .attr_idx = 0,
        .program = 0
    };

    shader_attr_push_s(&dummy, GL_FLOAT, 0, struct vert, pos);
    shader_attr_push_s(&dummy, GL_FLOAT, 0, struct vert, normal);
    shader_attr_push_s(&dummy, GL_FLOAT, 0, struct vert, color);
    shader_attr_push_s(&dummy, GL_FLOAT, 0, struct vert, uv);
#endif

    glbuf_unbind(&dst->vbo);

    // instances
    
    glbuf_init(&dst->vboi, NULL, 0, GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW);


    GLCall(glBindVertexArray(dst->gl_vao));
    glbuf_bind(&dst->vboi);

#if 1
    vao_attr_push_mat4f_div_inc(idx, GL_FLOAT, 0, struct ivert, trans);

    GLCall(glVertexAttribDivisor(idx, 1));
    vao_attr_push_inc(idx, GL_FLOAT, 0, struct ivert, tex_idx_offset);

#else
    shader_attr_push_mat4f_div_s(&dummy, 0, struct ivert, trans);

    GLCall(glVertexAttribDivisor(dummy.attr_idx, 1));
    shader_attr_push_s(&dummy, GL_FLOAT, 0, struct ivert, tex_idx_offset);
#endif


    glbuf_unbind(&dst->vboi);

    // set global rotation and translation.

    dst->rot[0] = 0.0;
    dst->rot[1] = 0.0;
    dst->rot[2] = 0.0;
    dst->rot[3] = 1.0;

    dst->pos[0] = 0;
    dst->pos[1] = 0;
    dst->pos[2] = 0;

    // init light texture
    // init first instance
# if 0
    struct ivert i1;
    mat4_rotation_axis(i1.trans, (float []){0, 0, 1}, 0);
    mesh_ivert_push_back(dst, i1);
#endif

    return 0;
}

void mesh_free(struct mesh *dst){
    glbuf_free(&dst->vbo);
    glbuf_free(&dst->ibo);
    glbuf_free(&dst->vboi);
    GLCall(glDeleteVertexArrays(1, &dst->gl_vao));
    darray_free(&dst->textures);
}

int mesh_push(struct mesh *dst){
#if 0
    GLCall(glBindVertexArray(dst->gl_vao));

    if(dst->gl_vbo_size != darray_size(&dst->verts)){
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, dst->gl_vbo));
        GLCall(glBufferData(GL_ARRAY_BUFFER, darray_size(&dst->verts), dst->verts, GL_DYNAMIC_DRAW));
        dst->gl_vbo_size = darray_size(&dst->verts);
    }
    else{
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, dst->gl_vbo));
        GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, dst->gl_vbo_size, dst->verts));
    }

    if(dst->gl_ibo_size != darray_size(&dst->tris)){
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, dst->gl_ibo));
        GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, darray_size(&dst->tris), dst->tris, GL_DYNAMIC_DRAW));
        dst->gl_ibo_size = darray_size(&dst->tris);
    }
    else{
        GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, dst->gl_ibo));
        GLCall(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, dst->gl_ibo_size, dst->tris));
    }

    if(dst->gl_vboi_size != darray_size(&dst->verts)){
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, dst->gl_vboi));
        GLCall(glBufferData(GL_ARRAY_BUFFER, darray_size(&dst->iverts), dst->iverts, GL_DYNAMIC_DRAW));
        dst->gl_vboi_size = darray_size(&dst->iverts);
    }
    else{
        GLCall(glBindBuffer(GL_ARRAY_BUFFER, dst->gl_vboi));
        GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, dst->gl_vboi_size, dst->iverts));
    }

#endif
    return 0;
}

int mesh_draw(struct mesh *src, struct camera *camera, struct shader *shader){
    shader_bind(shader);

    GLCall(glBindVertexArray(src->gl_vao));

    mat4 mat_tmp;
    
    glm_quat_mat4(src->rot, mat_tmp);
    glm_translate(mat_tmp, src->pos);

    shader_uniform_mat4f(shader, "u_trans", (float *)mat_tmp);

    shader_uniform_mat4f(shader, "u_proj", (float *)camera->mat);

    size_t slot = 0;

    for(slot = 0;slot < darray_len(&src->textures) && slot < MAX_TEXTURES;slot++){
        texture_bind(&src->textures[slot], slot);
        char buf[100] = {0};
        snprintf(buf, 100, "u_sampler[%zu]", slot);
        shader_uniform_i(shader, buf, slot);
    }

    // add lights as texture
#if 0
    if(src->lights != NULL){
        texture_bind(src->lights, slot);
        shader_uniform_i(src->shader, "u_lights", slot);
        shader_uniform_i(src->shader, "u_lights_w", src->lights->w);
        shader_uniform_i(src->shader, "u_lights_h", src->lights->h);
    }
#endif


    GLCall(glBindVertexArray(src->gl_vao));
    glbuf_bind(&src->ibo);
    //GLCall(glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, NULL));
    size_t count = glbuf_size(&src->ibo) / sizeof(float);
    size_t count_inst = glbuf_size(&src->vboi) / sizeof(struct ivert);
    GLCall(glDrawElementsInstanced(GL_TRIANGLES, count, GL_UNSIGNED_INT, NULL, count_inst));

    shader_unbind(shader);
    return 0;
}

int mesh_vert_push_back(struct mesh *dst, struct vert src){
    GLCall(glBindVertexArray(dst->gl_vao));
    return glbuf_append(&dst->vbo, &src, sizeof(struct vert));
}
int mesh_vert_push(struct mesh *dst, struct vert src, size_t i){
    GLCall(glBindVertexArray(dst->gl_vao));
    glbuf_insert(&dst->vbo, &src, sizeof(struct vert), i * sizeof(struct vert));
    struct tri *buf = malloc(glbuf_size(&dst->ibo));
    glbuf_get(&dst->ibo, buf, 0, glbuf_size(&dst->ibo));

    for(size_t i = 0;i < glbuf_size(&dst->ibo) / sizeof(struct tri);i++){
        if(buf[i].idxs[0] >= i)
            buf[i].idxs[0]++;
        if(buf[i].idxs[1] >= i)
            buf[i].idxs[1]++;
        if(buf[i].idxs[2] >= i)
            buf[i].idxs[2]++;
    }

    glbuf_set(&dst->ibo, buf, 0, glbuf_size(&dst->ibo));

    free(buf);
    return 0;
}
int mesh_vert_append(struct mesh *dst, struct vert *src, size_t n){
    GLCall(glBindVertexArray(dst->gl_vao));
    return glbuf_append(&dst->vbo, src, n * sizeof(struct vert));
}
int mesh_append(struct mesh *dst, const struct mesh *src){
    GLCall(glBindVertexArray(dst->gl_vao));

    struct vert src_vert;
    size_t src_verts_len = glbuf_size(&src->vbo) / sizeof(struct vert);
    struct vert *append_verts = malloc(sizeof(struct vert) * src_verts_len);
    for(size_t i = 0;i < src_verts_len;i++){
        glbuf_get(&src->vbo, &src_vert, sizeof(struct vert) * i, sizeof(struct vert));

        versor tmpq;
        vec3 tmpv3;

        glm_quat_inv(dst->rot, tmpq);

        // add original transforms and subtract old ones.
        glm_vec3_add(src_vert.pos, (float *)src->pos, src_vert.pos);
        glm_vec3_sub(src_vert.pos, dst->pos,          src_vert.pos);

        // rotate with src rotation and then inverse rotate with dst rotation
        glm_quat_rotatev((float *)src->rot, src_vert.pos, tmpv3);
        glm_quat_rotatev(tmpq, tmpv3, src_vert.pos);

        // rotate normals
        
        glm_quat_rotatev((float *)src->rot, src_vert.normal, tmpv3);
        glm_quat_rotatev(tmpq, tmpv3, src_vert.normal);

        append_verts[i] = src_vert;
    }

    glbuf_append(&dst->vbo, append_verts, sizeof(struct vert) * src_verts_len);

    free(append_verts);

    size_t src_tri_len = glbuf_size(&src->ibo) / sizeof(struct tri);
    size_t dst_tri_len = glbuf_size(&dst->ibo) / sizeof(struct tri);
    struct tri *append_tris = malloc(sizeof(struct tri) * src_tri_len);
    for(size_t i = 0;i < src_tri_len;i++){
        glbuf_get(&src->ibo, &append_tris[i], i * sizeof(struct tri), sizeof(struct tri));
        append_tris[i].idxs[0] += dst_tri_len;
        append_tris[i].idxs[1] += dst_tri_len;
        append_tris[i].idxs[2] += dst_tri_len;
    }

    glbuf_append(&dst->ibo, append_tris, sizeof(struct tri) * src_tri_len);

    free(append_tris);
    return 0;
}

int mesh_tri_push_back(struct mesh *dst, struct tri src){
    GLCall(glBindVertexArray(dst->gl_vao));
    return glbuf_append(&dst->ibo, &src, sizeof(struct tri));
}
int mesh_tri_append(struct mesh *dst, struct tri *src, size_t n){
    GLCall(glBindVertexArray(dst->gl_vao));
    return glbuf_append(&dst->ibo, src, sizeof(struct tri) * n);
}

int mesh_texture_push(struct mesh *dst, struct texture src){
    GLCall(glBindVertexArray(dst->gl_vao));
    return darray_push_back(&dst->textures, src);
}
int mesh_ivert_push_back(struct mesh *dst, struct ivert src){
    GLCall(glBindVertexArray(dst->gl_vao));
    return glbuf_append(&dst->vboi, &src, sizeof(struct ivert));
}
int mesh_iverts_clear(struct mesh *dst){
    GLCall(glBindVertexArray(dst->gl_vao));
    glbuf_resize(&dst->vboi, 0);
    return 0;
}
