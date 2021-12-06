#include "mesh.h"

int mesh_init(struct mesh *dst, struct shader *shader){
    darray_init(&dst->tris, 100);
    darray_init(&dst->verts, 100);
    darray_init(&dst->textures, 2);
    darray_init(&dst->iverts, 10);
    dst->shader = shader;

    // vertex array, vertex buffer and index buffer.

    GLCall(glGenVertexArrays(1, &dst->gl_vao));
    GLCall(glBindVertexArray(dst->gl_vao));

    GLCall(glGenBuffers(1, &dst->gl_vbo));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, dst->gl_vbo));

    GLCall(glGenBuffers(1, &dst->gl_ibo));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, dst->gl_ibo));

    GLCall(glBindBuffer(GL_ARRAY_BUFFER, dst->gl_vbo));

    shader_attr_push_s(dst->shader, GL_FLOAT, 0, struct vert, pos);
    shader_attr_push_s(dst->shader, GL_FLOAT, 0, struct vert, normal);
    shader_attr_push_s(dst->shader, GL_FLOAT, 0, struct vert, color);
    shader_attr_push_s(dst->shader, GL_FLOAT, 0, struct vert, uv);

    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));


    // instances

    GLCall(glGenBuffers(1, &dst->gl_vboi));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, dst->gl_vboi));

    shader_attr_push_mat4f_div_s(dst->shader, 0, struct ivert, trans);

    GLCall(glVertexAttribDivisor(dst->shader->attr_idx, 1));
    shader_attr_push_s(dst->shader, GL_FLOAT, 0, struct ivert, tex_idx_offset);

    GLCall(glVertexAttribDivisor(dst->shader->attr_idx, 1));
    shader_attr_push_s(dst->shader, GL_FLOAT, 0, struct ivert, light_tex_idx);

    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));

    // set global rotation and translation.

    dst->gl_vbo_size = 0;
    dst->gl_ibo_size = 0;
    dst->gl_vboi_size = 0;

    dst->rot[0] = 0.0;
    dst->rot[1] = 0.0;
    dst->rot[2] = 0.0;
    dst->rot[3] = 1.0;

    dst->pos[0] = 0;
    dst->pos[1] = 0;
    dst->pos[2] = 0;

    // init first instance
# if 0
    struct ivert i1;
    mat4_rotation_axis(i1.trans, (float []){0, 0, 1}, 0);
    mesh_ivert_push_back(dst, i1);
#endif

    return 0;
}
void mesh_free(struct mesh *dst){
    GLCall(glDeleteBuffers(1, &dst->gl_vboi));
    GLCall(glDeleteBuffers(1, &dst->gl_vbo));
    GLCall(glDeleteBuffers(1, &dst->gl_ibo));
    GLCall(glDeleteVertexArrays(1, &dst->gl_vao));
    darray_free(&dst->verts);
    darray_free(&dst->tris);
    darray_free(&dst->textures);
    darray_free(&dst->iverts);
}

int mesh_push(struct mesh *dst){
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

    return 0;
}

int mesh_draw(struct mesh *src){
    GLCall(glBindVertexArray(src->gl_vao));

    float mat_rot[16];
    float mat_trans[16];
    mat4_rotation_quat(mat_rot, src->rot);
    mat4_translation(mat_trans, mat_rot, src->pos);

    shader_uniform_mat4f(src->shader, "u_trans", mat_trans);

    for(size_t i = 0;i < darray_len(&src->textures) && i < MAX_TEXTURES;i++){
        texture_bind(&src->textures[i], i);
        char buf[100] = {0};
        snprintf(buf, 100, "u_sampler[%zu]", i);
        shader_uniform_i(src->shader, buf, i);
    }


    //GLCall(glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, NULL));
    GLCall(glDrawElementsInstanced(GL_TRIANGLES, darray_len(&src->tris) * 3, GL_UNSIGNED_INT, NULL, darray_len(&src->iverts)));

    return 0;
}

int mesh_vert_push_back(struct mesh *dst, struct vert src){
    return darray_push_back(&dst->verts, src);
}
int mesh_vert_push(struct mesh *dst, struct vert src, size_t i){
    darray_push(&dst->verts, src, i);
    for(size_t i = 0;i < darray_len(&dst->tris);i++){
        if(dst->tris[i].idxs[0] >= i)
            dst->tris[i].idxs[0]++;
        if(dst->tris[i].idxs[1] >= i)
            dst->tris[i].idxs[1]++;
        if(dst->tris[i].idxs[2] >= i)
            dst->tris[i].idxs[2]++;
    }
    return 0;
}
int mesh_append(struct mesh *dst, const struct mesh *src){
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
    return 0;
}

int mesh_tri_push_back(struct mesh *dst, struct tri src){
    return darray_push_back(&dst->tris, src);
}

int mesh_texture_push(struct mesh *dst, struct texture src){
    return darray_push_back(&dst->textures, src);
}
int mesh_ivert_push_back(struct mesh *dst, struct ivert src){
    return darray_push_back(&dst->iverts, src);
}
int mesh_iverts_clear(struct mesh *dst){
    darray_free(&dst->iverts);
    darray_init(&dst->iverts, 10);
    return 0;
}

