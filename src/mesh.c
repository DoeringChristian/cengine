#include "mesh.h"

int mesh_init(struct mesh *dst, struct vert *verts, size_t verts_len, struct tri *tris, size_t tris_len){
    darray_init(&dst->verts, 10);
    darray_init(&dst->tris, 10);
    darray_init(&dst->iverts, 1);
    dst->type = MESH_STATIC;

    GLCall(glGenVertexArrays(1, &dst->gl_vao));
    GLCall(glBindVertexArray(dst->gl_vao));

    if(dst->type == MESH_DYNAMIC){
        if(verts != NULL)
            darray_append(&dst->verts, verts, verts_len);
        if(tris != NULL)
            darray_append(&dst->tris, tris, tris_len);
    }

    glbuf_init(&dst->vbo, verts, verts_len * sizeof(struct vert), GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW);

    glbuf_init(&dst->ibo, tris, tris_len * sizeof(struct tri), GL_ELEMENT_ARRAY_BUFFER, GL_DYNAMIC_DRAW);

    glbuf_bind(&dst->vbo);

    int idx = 0;
    vao_attr_push_inc(idx, GL_FLOAT, 0, struct vert, pos);
    vao_attr_push_inc(idx, GL_FLOAT, 0, struct vert, normal);
    vao_attr_push_inc(idx, GL_FLOAT, 0, struct vert, color);
    vao_attr_push_inc(idx, GL_FLOAT, 0, struct vert, uv);

    glbuf_unbind(&dst->vbo);

    // instances
    
    glbuf_init(&dst->vboi, NULL, 0, GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW);


    GLCall(glBindVertexArray(dst->gl_vao));
    glbuf_bind(&dst->vboi);

    vao_attr_push_mat4f_div_inc(idx, GL_FLOAT, 0, struct ivert, trans);

    GLCall(glVertexAttribDivisor(idx, 1));
    vao_attr_push_inc(idx, GL_FLOAT, 0, struct ivert, tex_idx_offset);


    glbuf_unbind(&dst->vboi);

    // set global rotation and translation.

    dst->rot[0] = 0.0;
    dst->rot[1] = 0.0;
    dst->rot[2] = 0.0;
    dst->rot[3] = 1.0;

    dst->pos[0] = 0;
    dst->pos[1] = 0;
    dst->pos[2] = 0;

    dst->tex_albedo = NULL;
    dst->tex_normal = NULL;
    dst->tex_spec = NULL;

    return 0;
}
int mesh_init_cmesh(struct mesh *dst, struct cmesh *src){
    return mesh_init(dst, src->verts, darray_len(&src->verts), src->tris, darray_len(&src->tris));
}

void mesh_free(struct mesh *dst){
    glbuf_free(&dst->vbo);
    glbuf_free(&dst->ibo);
    glbuf_free(&dst->vboi);
    GLCall(glDeleteVertexArrays(1, &dst->gl_vao));
    darray_free(&dst->tris);
    darray_free(&dst->verts);
    darray_free(&dst->iverts);
}

int mesh_draw(struct mesh *src, struct cvert *camera, struct shader *shader){
    if(src->type == MESH_DYNAMIC)
        mesh_push(src);

    shader_bind(shader);

    GLCall(glBindVertexArray(src->gl_vao));

    mat4 mat_tmp;
    
    glm_quat_mat4(src->rot, mat_tmp);
    glm_translate(mat_tmp, src->pos);

    shader_uniform_mat4f(shader, "u_model", (float *)mat_tmp);

    shader_uniform_mat4f(shader, "u_proj", (float *)camera->proj);

    shader_uniform_mat4f(shader, "u_view", (float *)camera->view);

    size_t slot = 0;

    if(src->tex_albedo != NULL){
        texture_bind(src->tex_albedo, 0);
        shader_uniform_i(shader, "u_albedo", 0);
    }

    if(src->tex_normal != NULL){
        texture_bind(src->tex_normal, 1);
        shader_uniform_i(shader, "u_normal", 1);
    }

    if(src->tex_spec != NULL){
        texture_bind(src->tex_spec, 2);
        shader_uniform_i(shader, "u_spec", 2);
    }

#if 0
    for(slot = 0;slot < darray_len(&src->textures) && slot < MAX_TEXTURES;slot++){
        texture_bind(&src->textures[slot], slot);
        char buf[100] = {0};
        snprintf(buf, 100, "u_sampler[%zu]", slot);
        shader_uniform_i(shader, buf, slot);
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

int mesh_draw_depth(struct mesh *src, struct cvert *camera, struct shader *shader, struct light *light){
    if(src->type == MESH_DYNAMIC)
        mesh_push(src);

    shader_bind(shader);

    GLCall(glBindVertexArray(src->gl_vao));

    mat4 mat_tmp;
    
    glm_quat_mat4(src->rot, mat_tmp);
    glm_translate(mat_tmp, src->pos);

    shader_uniform_vec4f(shader, "u_light_pos", light->pos);

    shader_uniform_f(shader, "u_far", camera->far);

    shader_uniform_mat4f(shader, "u_model", (float *)mat_tmp);

    shader_uniform_mat4f(shader, "u_proj", (float *)camera->proj);

    shader_uniform_mat4f(shader, "u_view", (float *)camera->view);

    glbuf_bind(&src->ibo);

    size_t count = glbuf_size(&src->ibo) / sizeof(float);
    size_t count_inst = glbuf_size(&src->vboi) / sizeof(struct ivert);
    GLCall(glDrawElementsInstanced(GL_TRIANGLES, count, GL_UNSIGNED_INT, NULL, count_inst));

    GLCall(glBindVertexArray(0));

    shader_unbind(shader);
    return 0;
}

int mesh_pull(struct mesh *dst){
    size_t vbo_size = glbuf_size(&dst->vbo);
    size_t ibo_size = glbuf_size(&dst->ibo);
    size_t vboi_size = glbuf_size(&dst->vboi);
    if(vboi_size != darray_size(&dst->verts)){
        darray_free(&dst->verts);
        darray_init(&dst->verts, vbo_size / sizeof(struct vert));
        struct vert *buf = malloc(vbo_size);
        darray_append(&dst->verts, buf, vbo_size / sizeof(struct vert));
        free(buf);
    }
    if(ibo_size != darray_size(&dst->tris)){
        darray_free(&dst->tris);
        darray_init(&dst->tris, ibo_size / sizeof(struct tri));
        struct tri *buf = malloc(ibo_size);
        darray_append(&dst->tris, buf, ibo_size / sizeof(struct tri));
        free(buf);
    }
    if(vboi_size != darray_size(&dst->iverts)){
        darray_free(&dst->iverts);
        darray_init(&dst->iverts, vboi_size / sizeof(struct ivert));
        struct ivert *buf = malloc(vboi_size);
        darray_append(&dst->iverts, buf, vboi_size / sizeof(struct ivert));
        free(buf);
    }
    return 0;
}
int mesh_push(struct mesh *src){
    size_t vbo_size = glbuf_size(&src->vbo);
    size_t ibo_size = glbuf_size(&src->ibo);
    size_t vboi_size = glbuf_size(&src->vboi);
    if(vbo_size != darray_size(&src->verts)){
        glbuf_resize(&src->vbo, darray_size(&src->verts));
        glbuf_set(&src->vbo, src->verts, 0, darray_size(&src->verts));
    }
    if(ibo_size != darray_size(&src->tris)){
        glbuf_resize(&src->ibo, darray_size(&src->tris));
        glbuf_set(&src->ibo, src->tris, 0, darray_size(&src->tris));
    }
    if(vboi_size != darray_size(&src->iverts)){
        glbuf_resize(&src->vboi, darray_size(&src->iverts));
        glbuf_set(&src->vboi, src->iverts, 0, darray_size(&src->iverts));
    }
    return 0;
}

int mesh_vert_push_back(struct mesh *dst, struct vert src){
    if(dst->type == MESH_DYNAMIC){
        return darray_push_back(&dst->verts, src);
    }
    else if(dst->type == MESH_STATIC){
        GLCall(glBindVertexArray(dst->gl_vao));
        return glbuf_append(&dst->vbo, &src, sizeof(struct vert));
    }
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
    if(dst->type == MESH_DYNAMIC){
        return darray_append(&dst->verts, src, n);
    }
    else if(dst->type == MESH_STATIC){
        GLCall(glBindVertexArray(dst->gl_vao));
        return glbuf_append(&dst->vbo, src, n * sizeof(struct vert));
    }
    return 1;
}
int mesh_vert_set(struct mesh *dst, struct vert src, size_t i){
    if(dst->type == MESH_DYNAMIC){
        if(i <= darray_len(&dst->verts))
            dst->verts[i] = src;
        return 0;
    }
    else if(dst->type == MESH_STATIC){
        return glbuf_set(&dst->vbo, &src, i * sizeof(struct vert), sizeof(struct vert));
    }
    return 1;
}
int mesh_vert_setn(struct mesh *dst, struct vert *src, size_t n, size_t i){
    if(dst->type == MESH_DYNAMIC){
        if(i <= darray_len(&dst->verts))
            for(size_t j = 0;j < n;j++)
                dst->verts[i+j] = src[j];
        return 0;
    }
    else if(dst->type == MESH_STATIC){
        return glbuf_set(&dst->vbo, &src, i * sizeof(struct vert), n * sizeof(struct vert));
    }
    return 1;
}
int mesh_append(struct mesh *dst, const struct mesh *src){
    GLCall(glBindVertexArray(dst->gl_vao));

    struct vert src_vert;
    size_t src_verts_len = glbuf_size(&src->vbo) / sizeof(struct vert);
    struct vert *append_verts = malloc(sizeof(struct vert) * src_verts_len);
    for(size_t i = 0;i < src_verts_len;i++){
        if(dst->type == MESH_STATIC)
            glbuf_get(&src->vbo, &src_vert, sizeof(struct vert) * i, sizeof(struct vert));
        else if(dst->type == MESH_DYNAMIC)
            src_vert = dst->verts[i];
        else
            assert(0);

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

    mesh_vert_append(dst, append_verts, src_verts_len);
    //glbuf_append(&dst->vbo, append_verts, sizeof(struct vert) * src_verts_len);

    free(append_verts);

    size_t src_tris_len = glbuf_size(&src->ibo) / sizeof(struct tri);
    size_t dst_vert_len = glbuf_size(&dst->vbo) / sizeof(struct vert);
    struct tri *append_tris = malloc(sizeof(struct tri) * src_tris_len);
    for(size_t i = 0;i < src_tris_len;i++){
        if(dst->type == MESH_STATIC)
            glbuf_get(&src->ibo, &append_tris[i], i * sizeof(struct tri), sizeof(struct tri));
        else if(dst->type == MESH_STATIC)
            append_tris[i] = src->tris[i];
        append_tris[i].idxs[0] += dst_vert_len;
        append_tris[i].idxs[1] += dst_vert_len;
        append_tris[i].idxs[2] += dst_vert_len;
    }

    mesh_tri_append(dst, append_tris, src_tris_len);
    //glbuf_append(&dst->ibo, append_tris, sizeof(struct tri) * src_tri_len);

    free(append_tris);
    return 0;
}

int mesh_tri_push_back(struct mesh *dst, struct tri src){
    if(dst->type == MESH_STATIC){
        GLCall(glBindVertexArray(dst->gl_vao));
        return glbuf_append(&dst->ibo, &src, sizeof(struct tri));
    }
    else if(dst->type == MESH_DYNAMIC){
        darray_push_back(&dst->tris, src);
        return 0;
    }
    else
        assert(0);
    return 0;
}
int mesh_tri_append(struct mesh *dst, struct tri *src, size_t n){
    if(dst->type == MESH_STATIC){
        GLCall(glBindVertexArray(dst->gl_vao));
        return glbuf_append(&dst->ibo, src, sizeof(struct tri) * n);
    }
    else if(dst->type == MESH_DYNAMIC){
        darray_append(&dst->tris, src, n);
        return 0;
    }
    else
        assert(0);
    return 0;
}

void mesh_texture_albedo_set(struct mesh *dst, struct texture *src){
    dst->tex_albedo = src;
}
void mesh_texture_normal_set(struct mesh *dst, struct texture *src){
    dst->tex_normal = src;
}
void mesh_texture_spec_set(struct mesh *dst, struct texture *src){
    dst->tex_spec = src;
}
int mesh_ivert_push_back(struct mesh *dst, struct ivert src){
    if(dst->type == MESH_STATIC){
        GLCall(glBindVertexArray(dst->gl_vao));
        return glbuf_append(&dst->vboi, &src, sizeof(struct ivert));
    }
    else if(dst->type == MESH_DYNAMIC){
        darray_push_back(&dst->iverts, src);
        return 0;
    }
    else
        assert(0);
    return 0;
}
int mesh_ivert_set(struct mesh *dst, struct ivert src, int i){
    if(dst->type == MESH_STATIC){
        GLCall(glBindVertexArray(dst->gl_vao));
        return glbuf_set(&dst->vboi, &src, sizeof(struct ivert) * i, sizeof(struct ivert));
    }
    else if(dst->type == MESH_DYNAMIC){
        dst->iverts[i] = src;
    }
    else
        assert(0);
    return 0;
}
int mesh_ivert_get(struct mesh *src, struct ivert *dst, int i){
    if(src->type == MESH_STATIC){
        GLCall(glBindVertexArray(src->gl_vao));
        return glbuf_get(&src->vboi, dst, sizeof(struct ivert) * i, sizeof(struct ivert));
    }
    else if(src->type == MESH_DYNAMIC){
        *dst = src->iverts[i];
    }
    else
        assert(0);
    return 0;
}
int mesh_iverts_clear(struct mesh *dst){
    if(dst->type == MESH_STATIC){
        GLCall(glBindVertexArray(dst->gl_vao));
        return glbuf_resize(&dst->vboi, 0);
    }
    else if(dst->type == MESH_DYNAMIC){
        darray_free(&dst->iverts);
        darray_init(&dst->iverts, 10);
        return 0;
    }
    else
        assert(0);
    return 0;
}
