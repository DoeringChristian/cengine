#include "mesh.h"

int mesh_init(struct mesh *dst, struct vert *verts, size_t verts_len, struct tri *tris, size_t tris_len){
    dst->name = NULL;
    dst->has_shadow = 1;
    dst->material = NULL;

    GLCall(glGenVertexArrays(1, &dst->gl_vao));
    GLCall(glBindVertexArray(dst->gl_vao));


    glbuf_init(&dst->vbo, verts, verts_len * sizeof(struct vert), GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW);

    glbuf_init(&dst->ibo, tris, tris_len * sizeof(struct tri), GL_ELEMENT_ARRAY_BUFFER, GL_DYNAMIC_DRAW);

    glbuf_bind(&dst->vbo);

    int idx = 0;
    vao_attr_push_inc(idx, GL_FLOAT, 0, struct vert, pos);
    vao_attr_push_inc(idx, GL_FLOAT, 0, struct vert, normal);
    vao_attr_push_inc(idx, GL_FLOAT, 0, struct vert, tangent);
    vao_attr_push_inc(idx, GL_FLOAT, 0, struct vert, color);
    vao_attr_push_inc(idx, GL_FLOAT, 0, struct vert, uv);

    glbuf_unbind(&dst->vbo);

    // instances
    
    glbuf_init(&dst->vboi, NULL, 0, GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW);


    GLCall(glBindVertexArray(dst->gl_vao));
    glbuf_bind(&dst->vboi);

    vao_attr_push_mat4f_div_inc(idx, GL_FLOAT, 0, struct ivert, trans);

    GLCall(glVertexAttribDivisor(idx, 1));
    //vao_attr_push_inc(idx, GL_FLOAT, 0, struct ivert, tex_idx_offset);
    vao_attr_push_inc(idx, GL_FLOAT, 0, struct ivert, tex_offset);


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
#if 0
int mesh_init_cmesh(struct mesh *dst, struct cmesh *src){
    return mesh_init(dst, src->verts, darray_len(&src->verts), src->tris, darray_len(&src->tris));
}
#endif

void mesh_free(struct mesh *dst){
    glbuf_free(&dst->vbo);
    glbuf_free(&dst->ibo);
    glbuf_free(&dst->vboi);
    GLCall(glDeleteVertexArrays(1, &dst->gl_vao));
#if 0
    darray_free(&dst->tris);
    darray_free(&dst->verts);
    darray_free(&dst->iverts);
#endif
    free(dst->name);
}

int mesh_draw(struct mesh *src){
    GLCall(glBindVertexArray(src->gl_vao));

    glbuf_bind(&src->ibo);

    size_t count = glbuf_size(&src->ibo) / sizeof(float);
    size_t count_inst = glbuf_size(&src->vboi) / sizeof(struct ivert);

    GLCall(glDrawElementsInstanced(GL_TRIANGLES, count, GL_UNSIGNED_INT, NULL, count_inst));

    glbuf_unbind(&src->ibo);
    
    GLCall(glBindVertexArray(0));
    return 0;
}

int mesh_render(struct mesh *src, struct cvert *camera, struct shader *shader){
    if(src->material == NULL)
        return -1;

    GLCall(glEnable(GL_DEPTH_TEST));

    shader_bind(shader);

    GLCall(glBindVertexArray(src->gl_vao));

    mat4 mat_tmp;
    
    glm_quat_mat4(src->rot, mat_tmp);
    glm_translate(mat_tmp, src->pos);

    shader_uniform_mat4f(shader, "u_model", (float *)mat_tmp);

    shader_uniform_mat4f(shader, "u_proj", (float *)camera->proj);

    shader_uniform_mat4f(shader, "u_view", (float *)camera->view);

    size_t slot = 0;

    if(src->material->albedo_map != NULL){
        shader_uniform_tex(shader, src->material->albedo_map, "u_albedo_map");
        shader_uniform_i(shader, "u_has_albedo", 1);
    }
    else
        shader_uniform_i(shader, "u_has_albedo", 0);

    if(src->material->normal_map != NULL){
        shader_uniform_tex(shader, src->material->normal_map, "u_normal_map");
        shader_uniform_i(shader, "u_has_normal", 1);
    }
    else
        shader_uniform_i(shader, "u_has_normal", 0);

    if(src->material->mrao_map != NULL){
        shader_uniform_tex(shader, src->material->mrao_map, "u_mrao_map");
        shader_uniform_i(shader, "u_has_mrao", 1);
    }
    else
        shader_uniform_i(shader, "u_has_mrao", 0);

    if(src->material->emission_map != NULL){
        shader_uniform_tex(shader, src->material->emission_map, "u_emission_map");
        shader_uniform_i(shader, "u_has_emission", 1);
    }
    else
        shader_uniform_i(shader, "u_has_emission", 0);

    shader_uniform_vec4f(shader, "u_albedo", src->material->albedo);
    shader_uniform_vec4f(shader, "u_mrao", src->material->mrao);
    shader_uniform_vec4f(shader, "u_emission", src->material->emission);
    shader_uniform_f(shader, "u_normal_scale", src->material->normal_scale);

    mesh_draw(src);

    shader_unbind(shader);
    return 0;
}

int mesh_render_depth(struct mesh *src, struct cvert *camera, struct shader *shader, struct light *light){
    if(!src->has_shadow)
        return 0;

    GLCall(glEnable(GL_DEPTH_TEST));

    shader_bind(shader);

    mat4 mat_tmp;
    
    glm_quat_mat4(src->rot, mat_tmp);
    glm_translate(mat_tmp, src->pos);

    shader_uniform_vec4f(shader, "u_light_pos", light->pos);

    shader_uniform_f(shader, "u_far", camera->far);

    shader_uniform_mat4f(shader, "u_model", (float *)mat_tmp);

    shader_uniform_mat4f(shader, "u_proj", (float *)camera->proj);

    shader_uniform_mat4f(shader, "u_view", (float *)camera->view);

    mesh_draw(src);

    shader_unbind(shader);
    return 0;
}

int mesh_set_to(struct mesh *dst, struct vert *verts, size_t verts_len, struct tri *tris, size_t tris_len, struct ivert *iverts, size_t iverts_len){
    if(verts != NULL){
        glbuf_resize(&dst->vbo, sizeof(struct vert) * verts_len);
        glbuf_set(&dst->vbo, verts, 0, sizeof(struct vert) * verts_len);
    }
    if(tris != NULL){
        glbuf_resize(&dst->ibo, sizeof(struct tri) * tris_len);
        glbuf_set(&dst->ibo, tris, 0, sizeof(struct tri) * tris_len);
    }
    if(iverts != NULL){
        glbuf_resize(&dst->vboi, sizeof(struct ivert) * iverts_len);
        glbuf_set(&dst->vboi, iverts, 0, sizeof(struct ivert) * iverts_len);
    }
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
int mesh_vert_set(struct mesh *dst, struct vert src, size_t i){
    return glbuf_set(&dst->vbo, &src, i * sizeof(struct vert), sizeof(struct vert));
}
int mesh_vert_setn(struct mesh *dst, struct vert *src, size_t n, size_t i){
    return glbuf_set(&dst->vbo, &src, i * sizeof(struct vert), n * sizeof(struct vert));
}
int mesh_vert_set_tri(struct mesh *dst, struct vert *src, struct tri target){
    mesh_vert_set(dst, src[0], target.idxs[0]);
    mesh_vert_set(dst, src[1], target.idxs[1]);
    mesh_vert_set(dst, src[2], target.idxs[2]);
    return 0;
}
struct vert mesh_vert_get(struct mesh *src, size_t i){
    struct vert dst;
    glbuf_get(&src->vbo, &dst, i * sizeof(struct vert), sizeof(struct vert));
    return dst;
}
size_t mesh_vert_count(struct mesh *src){
    return glbuf_size(&src->vbo) / sizeof(struct vert);
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

    mesh_vert_append(dst, append_verts, src_verts_len);
    //glbuf_append(&dst->vbo, append_verts, sizeof(struct vert) * src_verts_len);

    free(append_verts);

    size_t src_tris_len = glbuf_size(&src->ibo) / sizeof(struct tri);
    size_t dst_vert_len = glbuf_size(&dst->vbo) / sizeof(struct vert);
    struct tri *append_tris = malloc(sizeof(struct tri) * src_tris_len);
    for(size_t i = 0;i < src_tris_len;i++){
        glbuf_get(&src->ibo, &append_tris[i], i * sizeof(struct tri), sizeof(struct tri));
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
    GLCall(glBindVertexArray(dst->gl_vao));
    return glbuf_append(&dst->ibo, &src, sizeof(struct tri));
}
int mesh_tri_append(struct mesh *dst, struct tri *src, size_t n){
    GLCall(glBindVertexArray(dst->gl_vao));
    return glbuf_append(&dst->ibo, src, sizeof(struct tri) * n);
}
int mesh_tri_set(struct mesh *dst, struct tri src, size_t i){
    if(i < glbuf_size(&dst->ibo) / sizeof(struct tri))
        glbuf_set(&dst->ibo, &src, sizeof(struct tri) * i, sizeof(struct tri));
    else
        return 1;
    return 0;
}
int mesh_tri_get_verts(struct mesh *src, struct tri tri, struct vert *dst){
    if(tri.idxs[0] < glbuf_size(&src->vbo) / sizeof(struct vert))
        glbuf_get(&src->vbo, &dst[0], sizeof(struct vert) * tri.idxs[0], sizeof(struct vert));
    if(tri.idxs[1] < glbuf_size(&src->vbo) / sizeof(struct vert))
        glbuf_get(&src->vbo, &dst[1], sizeof(struct vert) * tri.idxs[1], sizeof(struct vert));
    if(tri.idxs[1] < glbuf_size(&src->vbo) / sizeof(struct vert))
        glbuf_get(&src->vbo, &dst[2], sizeof(struct vert) * tri.idxs[2], sizeof(struct vert));
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
    GLCall(glBindVertexArray(dst->gl_vao));
    return glbuf_append(&dst->vboi, &src, sizeof(struct ivert));
}
int mesh_ivert_set(struct mesh *dst, struct ivert src, int i){
    GLCall(glBindVertexArray(dst->gl_vao));
    return glbuf_set(&dst->vboi, &src, sizeof(struct ivert) * i, sizeof(struct ivert));
}
struct ivert mesh_ivert_get(struct mesh *src, int i){
    struct ivert dst;
    GLCall(glBindVertexArray(src->gl_vao));
    glbuf_get(&src->vboi, &dst, sizeof(struct ivert) * i, sizeof(struct ivert));
    return dst;
}
int mesh_iverts_clear(struct mesh *dst){
    GLCall(glBindVertexArray(dst->gl_vao));
    return glbuf_resize(&dst->vboi, 0);
}
struct tri mesh_tri_get(struct mesh *src, size_t i){
    struct tri dst;
    glbuf_get(&src->ibo, &dst, sizeof(struct tri) * i, sizeof(struct tri));
    return dst;
}
size_t mesh_tri_count(struct mesh *src){
    return glbuf_size(&src->ibo) / sizeof(struct tri);
}
int mesh_tri_get_verts_i(struct mesh *src, size_t i, struct vert *dst){
    struct tri tri = mesh_tri_get(src, i);
    if(tri.idxs[0] < glbuf_size(&src->vbo) / sizeof(struct vert))
        glbuf_get(&src->vbo, &dst[0], sizeof(struct vert) * tri.idxs[0], sizeof(struct vert));
    if(tri.idxs[1] < glbuf_size(&src->vbo) / sizeof(struct vert))
        glbuf_get(&src->vbo, &dst[1], sizeof(struct vert) * tri.idxs[0], sizeof(struct vert));
    if(tri.idxs[1] < glbuf_size(&src->vbo) / sizeof(struct vert))
        glbuf_get(&src->vbo, &dst[2], sizeof(struct vert) * tri.idxs[0], sizeof(struct vert));
    return 0;
}
int mesh_cull_from_normal(struct mesh *dst){
    vec3 a, b;
    vec3 norm_sum;
    vec3 norm_gen;
    struct vert verts[3];
    struct tri tmptri;
    for(size_t i = 0;i < mesh_tri_count(dst);i++){
        mesh_tri_get_verts_i(dst, i, verts);
        tmptri = mesh_tri_get(dst, i);

        glm_vec3_sub(verts[1].pos, verts[0].pos, a);
        glm_vec3_sub(verts[2].pos, verts[0].pos, a);
        glm_cross(b, a, norm_gen);

        glm_vec3_add(verts[0].normal, verts[1].normal, norm_sum);
        glm_vec3_add(norm_sum, verts[2].normal, norm_sum);

        if(glm_vec3_dot(norm_gen, norm_sum) < 0){
            int tmp = tmptri.idxs[1];
            tmptri.idxs[1] = tmptri.idxs[2];
            tmptri.idxs[2] = tmp;
            mesh_tri_set(dst, tmptri, i);
        }
    }
}
int mesh_normal_from_cull(struct mesh *dst){
#if 0
    vec3 a, b;
    vec3 norm_sum;
    vec3 norm_gen;
    struct vert verts[3];
    struct tri tmptri;
    for(size_t i = 0;i < mesh_tri_count(dst);i++){
        mesh_tri_get_verts_i(dst, i, verts);
        tmptri = mesh_tri_get(dst, i);

        glm_vec3_sub(verts[1].pos, verts[0].pos, a);
        glm_vec3_sub(verts[2].pos, verts[0].pos, a);
        glm_cross(b, a, norm_gen);

        glm_vec3_add(verts[0].normal, verts[1].normal, norm_sum);
        glm_vec3_add(norm_sum, verts[2].normal, norm_sum);

    }
#endif
}
int mesh_gen_tangent(struct mesh *dst){
    struct tri tmptri;
    struct vert tmpverts[3];
    for(size_t i = 0;i < mesh_tri_count(dst);i++){
        tmptri = mesh_tri_get(dst, i);
        mesh_tri_get_verts(dst, tmptri, tmpverts);

        vec3 edge1, edge2;
        glm_vec3_sub(tmpverts[1].pos, tmpverts[0].pos, edge1);
        glm_vec3_sub(tmpverts[2].pos, tmpverts[0].pos, edge2);

        vec2 duv1, duv2;
        glm_vec2_sub(tmpverts[1].uv, tmpverts[0].uv, duv1);
        glm_vec2_sub(tmpverts[2].uv, tmpverts[0].uv, duv2);

        vec3 tangent, bitangent;
        // tangent
        float f = 1.0f / (duv1[0] * duv2[1] - duv2[0] * duv1[1]);
        tangent[0] = f * (duv2[1] * edge1[0] - duv1[1] * edge2[0]);
        tangent[1] = f * (duv2[1] * edge1[1] - duv1[1] * edge2[1]);
        tangent[2] = f * (duv2[1] * edge1[2] - duv1[1] * edge2[2]);

        // bitangent
        bitangent[0] = f * (-duv2[0] * edge1[0] - duv1[0] * edge2[0]);
        bitangent[1] = f * (-duv2[0] * edge1[1] - duv1[0] * edge2[1]);
        bitangent[2] = f * (-duv2[0] * edge1[2] - duv1[0] * edge2[2]);

        glm_vec3_copy(tangent, tmpverts[0].tangent);
        glm_vec3_copy(tangent, tmpverts[1].tangent);
        glm_vec3_copy(tangent, tmpverts[2].tangent);

        //glm_vec3_print(tangent, stdout);

        mesh_vert_set_tri(dst, tmpverts, tmptri);
    }
    return 0;
}

int mesh_name_set(struct mesh *dst, const char *name){
    if(dst->name != NULL)
        free(dst->name);
    size_t name_size = strlen(name)+1;
    dst->name = malloc(name_size);
    memcpy(dst->name, name, name_size);
    return 0;
}
int mesh_material_set(struct mesh *dst, struct material *src){
    dst->material = src;
    return 0;
}

// ------------------------------------------------------------
// mesh2:

int mesh2_init(struct mesh2 *dst, struct vert2 *verts, size_t verts_len, int *idxs, size_t idxs_len){
    GLCall(glGenVertexArrays(1, &dst->gl_vao));
    GLCall(glBindVertexArray(dst->gl_vao));

    glbuf_init(&dst->vbo, verts, verts_len * sizeof(struct vert2), GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW);

    glbuf_init(&dst->ibo, idxs, idxs_len * sizeof(int), GL_ELEMENT_ARRAY_BUFFER, GL_DYNAMIC_DRAW);

    glbuf_bind(&dst->vbo);

    int idx = 0;
    vao_attr_push_inc(idx, GL_FLOAT, 0, struct vert2, pos);

    glbuf_unbind(&dst->vbo);

    GLCall(glBindVertexArray(0));

    return 0;
}
void mesh2_free(struct mesh2 *dst){
    glbuf_free(&dst->vbo);
    glbuf_free(&dst->ibo);

    GLCall(glDeleteVertexArrays(1, &dst->gl_vao));
}

int mesh2_draw(struct mesh2 *dst){
    GLCall(glBindVertexArray(dst->gl_vao));

    glbuf_bind(&dst->ibo);

    GLCall(glDrawElements(GL_TRIANGLES, glbuf_size(&dst->ibo), GL_UNSIGNED_INT, NULL));

    glbuf_unbind(&dst->ibo);

    GLCall(glBindVertexArray(0));

    return 0;
}

// ------------------------------------------------------------
// mesh2:

int mesh3_init(struct mesh3 *dst, struct vert3 *verts, size_t verts_len, int *idxs, size_t idxs_len){
    GLCall(glGenVertexArrays(1, &dst->gl_vao));
    GLCall(glBindVertexArray(dst->gl_vao));

    glbuf_init(&dst->vbo, verts, verts_len * sizeof(struct vert3), GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW);

    glbuf_init(&dst->ibo, idxs, idxs_len * sizeof(int), GL_ELEMENT_ARRAY_BUFFER, GL_DYNAMIC_DRAW);

    glbuf_bind(&dst->vbo);

    int idx = 0;
    vao_attr_push_inc(idx, GL_FLOAT, 0, struct vert3, pos);

    glbuf_unbind(&dst->vbo);

    GLCall(glBindVertexArray(0));

    return 0;
}
void mesh3_free(struct mesh3 *dst){
    glbuf_free(&dst->vbo);
    glbuf_free(&dst->ibo);

    GLCall(glDeleteVertexArrays(1, &dst->gl_vao));
}

int mesh3_draw(struct mesh3 *dst){
    GLCall(glBindVertexArray(dst->gl_vao));

    glbuf_bind(&dst->ibo);

    GLCall(glDrawElements(GL_TRIANGLES, glbuf_size(&dst->ibo), GL_UNSIGNED_INT, NULL));

    glbuf_unbind(&dst->ibo);

    GLCall(glBindVertexArray(0));

    return 0;
}
