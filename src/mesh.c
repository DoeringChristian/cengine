#include "mesh.h"

int mesh_init(struct mesh *dst, struct shader *shader){
    darray_init(&dst->tris, 100);
    darray_init(&dst->verts, 100);
    darray_init(&dst->textures, 2);
    darray_init(&dst->instances, 1);
    dst->shader = shader;

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

    GLCall(glGenBuffers(1, &dst->gl_vbo_instances));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, dst->gl_vbo_instances));


    shader_attr_push_mat4f_div_s(dst->shader, 0, struct instance, trans);

    GLCall(glVertexAttribDivisor(dst->shader->attr_idx, 1));
    shader_attr_push_s(dst->shader, GL_FLOAT, 0, struct instance, tex_idx_offset);

    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));

    dst->gl_vbo_size = 0;
    dst->gl_ibo_size = 0;
    dst->gl_vbo_instances_size = 0;

    memset(dst->pos, 0, sizeof(dst->pos));
    memset(dst->rot, 0, sizeof(dst->rot));

    // init first instance
    struct instance i1;
    mat4_rotation_axis(i1.trans, (float []){0, 0, 1}, 0);
    mesh_instance_push_back(dst, i1);

    return 0;
}
void mesh_free(struct mesh *dst){
    GLCall(glDeleteBuffers(1, &dst->gl_vbo_instances));
    GLCall(glDeleteBuffers(1, &dst->gl_vbo));
    GLCall(glDeleteBuffers(1, &dst->gl_ibo));
    GLCall(glDeleteVertexArrays(1, &dst->gl_vao));
    darray_free(&dst->verts);
    darray_free(&dst->tris);
    darray_free(&dst->textures);
    darray_free(&dst->instances);
}

int mesh_resize(struct mesh *dst){
    GLCall(glBindVertexArray(dst->gl_vbo));

    GLCall(glBindBuffer(GL_ARRAY_BUFFER, dst->gl_vbo));
    GLCall(glBufferData(GL_ARRAY_BUFFER, darray_size(&dst->verts), dst->verts, GL_DYNAMIC_DRAW));

    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, dst->gl_ibo));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, darray_size(&dst->tris), dst->tris, GL_DYNAMIC_DRAW));

    GLCall(glBindBuffer(GL_ARRAY_BUFFER, dst->gl_vbo_instances));
    GLCall(glBufferData(GL_ARRAY_BUFFER, darray_size(&dst->instances), dst->instances, GL_DYNAMIC_DRAW));

    dst->gl_vbo_size = darray_size(&dst->verts);
    dst->gl_ibo_size = darray_size(&dst->tris);
    dst->gl_vbo_instances_size = darray_size(&dst->instances);

    return 0;
}
int mesh_update(struct mesh *dst){

    if(dst->gl_vbo_size != darray_size(&dst->verts)
            || dst->gl_ibo_size != darray_size(&dst->tris) 
            || dst->gl_vbo_instances_size != darray_size(&dst->instances))
        mesh_resize(dst);

    GLCall(glBindVertexArray(dst->gl_vao));

    GLCall(glBindBuffer(GL_ARRAY_BUFFER, dst->gl_vbo));
    GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, dst->gl_vbo_size, dst->verts));

    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, dst->gl_ibo));
    GLCall(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, dst->gl_ibo_size, dst->tris));

    GLCall(glBindBuffer(GL_ARRAY_BUFFER, dst->gl_vbo_instances));
    GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, dst->gl_vbo_instances_size, dst->instances));

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
    GLCall(glDrawElementsInstanced(GL_TRIANGLES, darray_len(&src->tris) * 3, GL_UNSIGNED_INT, NULL, darray_len(&src->instances)));

    return 0;
}

int mesh_attr_push(struct mesh *dst, GLsizei num, GLenum type, GLint normalized){
    GLCall(glBindVertexArray(dst->gl_vao));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, dst->gl_ibo));
    //attr_arr_push(&dst->attr_arr, num, type, normalized);

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
int mesh_instance_push_back(struct mesh *dst, struct instance src){
    return darray_push_back(&dst->instances, src);
}
