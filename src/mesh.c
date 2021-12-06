#include "mesh.h"

int mesh_init(struct mesh *dst, struct shader *shader, struct vert *verts, size_t verts_len, struct tri *tris, size_t tris_len){
    darray_init(&dst->textures, 2);
#if 0
    darray_init(&dst->tris, 100);
    darray_init(&dst->verts, 100);
    darray_init(&dst->iverts, 10);
#endif
    dst->shader = shader;

    // vertex array, vertex buffer and index buffer.

    GLCall(glGenVertexArrays(1, &dst->gl_vao));
    GLCall(glBindVertexArray(dst->gl_vao));

    gbuf_init(&dst->vbo, verts, verts_len * sizeof(struct vert), GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW);

    gbuf_init(&dst->ibo, tris, tris_len * sizeof(struct tri), GL_ELEMENT_ARRAY_BUFFER, GL_DYNAMIC_DRAW);

    gbuf_bind(&dst->vbo);

#if 0
    GLCall(glGenBuffers(1, &dst->gl_vbo));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, dst->gl_vbo));

    GLCall(glGenBuffers(1, &dst->gl_ibo));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, dst->gl_ibo));

    GLCall(glBindBuffer(GL_ARRAY_BUFFER, dst->gl_vbo));
#endif

    shader_attr_push_s(dst->shader, GL_FLOAT, 0, struct vert, pos);
    shader_attr_push_s(dst->shader, GL_FLOAT, 0, struct vert, normal);
    shader_attr_push_s(dst->shader, GL_FLOAT, 0, struct vert, color);
    shader_attr_push_s(dst->shader, GL_FLOAT, 0, struct vert, uv);

    gbuf_unbind(&dst->vbo);

    // instances
    
    gbuf_init(&dst->vboi, NULL, 0, GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW);

#if 0
    GLCall(glGenBuffers(1, &dst->gl_vboi));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, dst->gl_vboi));
#endif

    GLCall(glBindVertexArray(dst->gl_vao));
    gbuf_bind(&dst->vboi);

    shader_attr_push_mat4f_div_s(dst->shader, 0, struct ivert, trans);

    GLCall(glVertexAttribDivisor(dst->shader->attr_idx, 1));
    shader_attr_push_s(dst->shader, GL_FLOAT, 0, struct ivert, tex_idx_offset);

    GLCall(glVertexAttribDivisor(dst->shader->attr_idx, 1));
    shader_attr_push_s(dst->shader, GL_FLOAT, 0, struct ivert, light_tex_idx);

    gbuf_unbind(&dst->vboi);

    // set global rotation and translation.

    dst->rot[0] = 0.0;
    dst->rot[1] = 0.0;
    dst->rot[2] = 0.0;
    dst->rot[3] = 1.0;

    dst->pos[0] = 0;
    dst->pos[1] = 0;
    dst->pos[2] = 0;

    // init light texture
    float light[] = {
        1, 0, 0, 1,
        1, 1, 1, 1,
    };
    texture_init(&dst->lights, 0, 2, light);

    // init first instance
# if 0
    struct ivert i1;
    mat4_rotation_axis(i1.trans, (float []){0, 0, 1}, 0);
    mesh_ivert_push_back(dst, i1);
#endif

    return 0;
}
int mesh_init_quad(struct mesh *dst, struct shader *shader){

    struct vert verts[4] = {
        {
            .pos = {-1, -1, 0},
            .normal = {0, 0, 1},
            .color = {0, 0, 0, 1},
            .uv = {0, 0},
        },
        {
            .pos = {-1, 1, 0},
            .normal = {0, 0, 1},
            .color = {0, 0, 0, 1},
            .uv = {0, 1},
        },
        {
            .pos = {1, -1, 0},
            .normal = {0, 0, 1},
            .color = {0, 0, 0, 1},
            .uv = {1, 0},
        },
        {
            .pos = {1, 1, 0},
            .normal = {0, 0, 1},
            .color = {0, 0, 0, 1},
            .uv = {1, 1},
        },
    };

    struct tri tris[2] = {
        {
            0, 1, 2,
        },
        {
            2, 3, 1,
        },
    };

    mesh_init(dst, shader, verts, 4, tris, 2);

    return 0;
}

void mesh_free(struct mesh *dst){
    texture_free(&dst->lights);
    gbuf_free(&dst->vbo);
    gbuf_free(&dst->ibo);
    gbuf_free(&dst->vboi);
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

int mesh_draw(struct mesh *src){
    GLCall(glBindVertexArray(src->gl_vao));

    float mat_rot[16];
    float mat_trans[16];
    mat4_rotation_quat(mat_rot, src->rot);
    mat4_translation(mat_trans, mat_rot, src->pos);

    shader_uniform_mat4f(src->shader, "u_trans", mat_trans);

    size_t slot = 0;

    for(slot = 0;slot < darray_len(&src->textures) && slot < MAX_TEXTURES;slot++){
        texture_bind(&src->textures[slot], slot);
        char buf[100] = {0};
        snprintf(buf, 100, "u_sampler[%zu]", slot);
        shader_uniform_i(src->shader, buf, slot);
    }

    // add lights as texture
    texture_bind(&src->lights, slot);
    shader_uniform_i(src->shader, "u_lights", slot);
    shader_uniform_i(src->shader, "u_lights_w", src->lights.w);
    shader_uniform_i(src->shader, "u_lights_h", src->lights.h);


    GLCall(glBindVertexArray(src->gl_vao));
    gbuf_bind(&src->ibo);
    //GLCall(glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, NULL));
    size_t count = gbuf_size(&src->ibo) / sizeof(float);
    size_t count_inst = gbuf_size(&src->vboi) / sizeof(struct ivert);
    GLCall(glDrawElementsInstanced(GL_TRIANGLES, count, GL_UNSIGNED_INT, NULL, count_inst));

    return 0;
}

int mesh_vert_push_back(struct mesh *dst, struct vert src){
    GLCall(glBindVertexArray(dst->gl_vao));
    return gbuf_append(&dst->vbo, &src, sizeof(struct vert));
}
int mesh_vert_push(struct mesh *dst, struct vert src, size_t i){
    GLCall(glBindVertexArray(dst->gl_vao));
    gbuf_insert(&dst->vbo, &src, sizeof(struct vert), i * sizeof(struct vert));
    struct tri *buf = malloc(gbuf_size(&dst->ibo));
    gbuf_get(&dst->ibo, buf, 0, gbuf_size(&dst->ibo));

    for(size_t i = 0;i < gbuf_size(&dst->ibo) / sizeof(struct tri);i++){
        if(buf[i].idxs[0] >= i)
            buf[i].idxs[0]++;
        if(buf[i].idxs[1] >= i)
            buf[i].idxs[1]++;
        if(buf[i].idxs[2] >= i)
            buf[i].idxs[2]++;
    }

    gbuf_set(&dst->ibo, buf, 0, gbuf_size(&dst->ibo));

    free(buf);
    return 0;
}
int mesh_vert_append(struct mesh *dst, struct vert *src, size_t n){
    GLCall(glBindVertexArray(dst->gl_vao));
    return gbuf_append(&dst->vbo, src, n * sizeof(struct vert));
}
int mesh_append(struct mesh *dst, const struct mesh *src){
    GLCall(glBindVertexArray(dst->gl_vao));
#if 0
    size_t i = darray_len(&dst->verts);
    struct vert src_vert;
    for(size_t j = 0;j < darray_len(&src->verts);j++){
        src_vert = src->verts[j];
        vec3_subtract(src_vert.pos, src_vert.pos, dst->pos);

        float q[4];
        quat_multiply(q, dst->rot, (float *)src->rot);
        vec3_quat_rotate_inverse(src_vert.pos, src_vert.pos, q);
        darray_push_back(&dst->verts, src_vert);
    }
    //darray_append(&dst->verts, src->verts, darray_len(&src->verts));

    struct tri src_tri;
    for(size_t j = 0;j < darray_len(&src->tris);j++){
        src_tri = src->tris[j];
        src_tri.idxs[0] += i;
        src_tri.idxs[1] += i;
        src_tri.idxs[2] += i;
        darray_push_back(&dst->tris, src_tri);
    }
#endif
    return 0;
}

int mesh_tri_push_back(struct mesh *dst, struct tri src){
    GLCall(glBindVertexArray(dst->gl_vao));
    return gbuf_append(&dst->ibo, &src, sizeof(struct tri));
}
int mesh_tri_append(struct mesh *dst, struct tri *src, size_t n){
    GLCall(glBindVertexArray(dst->gl_vao));
    return gbuf_append(&dst->ibo, src, sizeof(struct tri) * n);
}

int mesh_texture_push(struct mesh *dst, struct texture src){
    GLCall(glBindVertexArray(dst->gl_vao));
    return darray_push_back(&dst->textures, src);
}
int mesh_ivert_push_back(struct mesh *dst, struct ivert src){
    GLCall(glBindVertexArray(dst->gl_vao));
    return gbuf_append(&dst->vboi, &src, sizeof(struct ivert));
}
int mesh_iverts_clear(struct mesh *dst){
    GLCall(glBindVertexArray(dst->gl_vao));
    gbuf_resize(&dst->vboi, 0);
    return 0;
}
int mesh_lvert_push_back(struct mesh *dst, struct lvert src){
    GLCall(glBindVertexArray(dst->gl_vao));
    texture_resize(&dst->lights, dst->lights.w + 1, dst->lights.h);
    texture_set(&dst->lights, dst->lights.w - 1, 0, src.pos);
    texture_set(&dst->lights, dst->lights.w - 1, 1, src.color);
    return 0;
}

