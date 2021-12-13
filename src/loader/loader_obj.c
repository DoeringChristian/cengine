#include "loader_obj.h"

static ssize_t readline(FILE *fp, char *buf, size_t buf_size){
    ssize_t i = 0;
    int c = 0;
    for(i = 0;i < buf_size;i++){
        c = getc(fp);
        if(c == EOF)
            break;
        else if(c == '\n')
            break;
        buf[i] = c;
    }
    if(i < buf_size - 1){
        buf[i] = 0;
    }
    return i;
}

int mesh_load_obj(struct mesh *dst, const char *path){
    FILE *fp = fopen(path, "r");

    darray(struct vert) verts;
    darray(struct tri) tris;
    darray(struct vert) v;
    darray(struct vert) vn;
    darray(struct vert) vt;

    darray_init(&verts, 100);
    darray_init(&tris, 100);
    darray_init(&v, 100);
    darray_init(&vn, 100);
    darray_init(&vt, 100);

    char line[500] = {0};
    char name_buf[500];

    float px, py, pz;
    int idxs[16];
    struct vert tmpv;
    struct tri tmptri;
    for(;readline(fp, line, 500) > 0;){
        if(sscanf(line, "v %f %f %f", &px, &py, &pz) == 3){
            tmpv.pos[0] = px;
            tmpv.pos[1] = py;
            tmpv.pos[2] = pz;
            darray_push_back(&v, tmpv);
        }
        else if(sscanf(line, "vn %f %f %f", &px, &py, &pz) == 3){
            tmpv.normal[0] = px;
            tmpv.normal[1] = py;
            tmpv.normal[2] = pz;
            darray_push_back(&vn, tmpv);
        }
        else if(sscanf(line, "vt %f %f", &px, &py) == 2){
            tmpv.uv[0] = px;
            tmpv.uv[1] = py;
            darray_push_back(&vt, tmpv);
        }
        else if(sscanf(line, "f %i %i %i", &idxs[0], &idxs[1], &idxs[2]) == 3){
            tmptri.idxs[0] = idxs[0];
            tmptri.idxs[1] = idxs[1];
            tmptri.idxs[2] = idxs[2];
            darray_push_back(&tris, tmptri);
        }
        else if(sscanf(line, "f %i//%i %i//%i %i//%i", &idxs[0], &idxs[3], &idxs[1], &idxs[4], &idxs[2], &idxs[5]) == 6){
            tmptri.idxs[0] = idxs[0];
            tmptri.idxs[1] = idxs[1];
            tmptri.idxs[2] = idxs[2];
            darray_push_back(&tris, tmptri);
            glm_vec3_copy(vn[idxs[3]].normal, v[idxs[0]].normal);
            glm_vec3_copy(vn[idxs[4]].normal, v[idxs[1]].normal);
            glm_vec3_copy(vn[idxs[5]].normal, v[idxs[2]].normal);
        }
        else if(sscanf(line, "f %i/%i/%i %i/%i/%i %i/%i/%i", &idxs[0], &idxs[3], &idxs[6], &idxs[1], &idxs[4], &idxs[7], &idxs[2], &idxs[5], &idxs[8]) == 9){
            tmptri.idxs[0] = darray_len(&verts) +0;
            tmptri.idxs[1] = darray_len(&verts) +1;
            tmptri.idxs[2] = darray_len(&verts) +2;
            darray_push_back(&tris, tmptri);

            glm_vec3_copy(v[idxs[0]-1].pos, tmpv.pos);
            glm_vec3_copy(vn[idxs[6]-1].normal, tmpv.normal);
            glm_vec2_copy(vt[idxs[3]-1].uv, tmpv.uv);
            darray_push_back(&verts, tmpv);

            glm_vec3_copy(v[idxs[1]-1].pos, tmpv.pos);
            glm_vec3_copy(vn[idxs[7]-1].normal, tmpv.normal);
            glm_vec2_copy(vt[idxs[4]-1].uv, tmpv.uv);
            darray_push_back(&verts, tmpv);

            glm_vec3_copy(v[idxs[2]-1].pos, tmpv.pos);
            glm_vec3_copy(vn[idxs[8]-1].normal, tmpv.normal);
            glm_vec2_copy(vt[idxs[5]-1].uv, tmpv.uv);
            darray_push_back(&verts, tmpv);

        }
        else if(sscanf(line, "o %500s", name_buf) == 1){

        }
        //printf("%s\n", line);
    }


    mesh_init(dst, verts, darray_len(&verts), tris, darray_len(&tris));

    if(strlen(name_buf) > 0)
        mesh_name_set(dst, name_buf);

    darray_free(&verts);
    darray_free(&v);
    darray_free(&tris);
    darray_free(&vn);
    darray_free(&vt);

    fclose(fp);
    return 0;
}
int container_load_mtl(struct container *dst, const char *path){
    FILE *fp = fopen(path, "r");

    char buf[500] = {0};
    char buf_mtl_name[500] = {0};
    char buf_dir[256] = {0};
    struct material *mat_cur = NULL;
    struct texture *tex_cur = NULL;
    vec3 color_tmp;

    for(;readline(fp, buf, 500) > 0;){
        if(sscanf(buf, "newmtl %500s", buf_mtl_name) == 1){
            mat_cur = malloc(sizeof(struct material));
            material_init(mat_cur);
            darray_push_back(&dst->materials, mat_cur);
        }
        else if(sscanf(buf, "Kd %f %f %f", &color_tmp[0], &color_tmp[1], &color_tmp[2]) == 3){
            if(mat_cur != NULL)
                glm_vec3_copy(color_tmp, mat_cur->albedo);
        }
        else if(sscanf(buf, "Ks %f %f %f", &color_tmp[0], &color_tmp[1], &color_tmp[2]) == 3){
            if(mat_cur != NULL)
                glm_vec3_copy(color_tmp, mat_cur->mrao);
        }
        else if(sscanf(buf, "map_Kd %s", buf_mtl_name) == 1){
            if(mat_cur != NULL){
                char tmp[256] = {0};
                char *slash_last = strrchr(path, '/');
                memcpy(buf_dir, path, slash_last - path);
                sprintf(tmp, "%s/%s", buf_dir, buf_mtl_name);
                tex_cur = malloc(sizeof(struct texture));
                texture_load(tex_cur, tmp);
                darray_push_back(&dst->textures, tex_cur);
                material_albedo_map_set(mat_cur, tex_cur);
                tex_cur = NULL;
            }
        }
        else if(sscanf(buf, "map_Bump %s", buf_mtl_name) == 1){
            if(mat_cur != NULL){
                char tmp[256] = {0};
                char *slash_last = strrchr(path, '/');
                memcpy(buf_dir, path, slash_last - path);
                sprintf(tmp, "%s/%s", buf_dir, buf_mtl_name);
                tex_cur = malloc(sizeof(struct texture));
                texture_load(tex_cur, tmp);
                darray_push_back(&dst->textures, tex_cur);
                material_normal_map_set(mat_cur, tex_cur, 1);
                tex_cur = NULL;
            }
        }
        else{

        }
    }

    return 0;
}
int container_load_obj(struct container *dst, const char *path){

    struct mesh *tmp_mesh = malloc(sizeof(struct mesh));
    mesh_load_obj(tmp_mesh, path);
    darray_push_back(&dst->meshes, tmp_mesh);

    FILE *fp = fopen(path, "r");

    char buf[500] = {0};
    char buf_mtllib[500] = {0};
    char buf_usemtl[500] = {0};
    char buf_dir[256] = {0};

    for(;readline(fp, buf, 500) > 0;){
        if(sscanf(buf, "mtllib %500s", buf_mtllib) == 1){
            char tmp[256] = {0};
            char *slash_last = strrchr(path, '/');
            memcpy(buf_dir, path, slash_last - path);
            sprintf(tmp, "%s/%s", buf_dir, buf_mtllib);
            container_load_mtl(dst, tmp);
        }
        else if(sscanf(buf, "usemtl %500s", buf_usemtl) == 1){

        }
        else{

        }
    }

    fclose(fp);
    return 0;
}
