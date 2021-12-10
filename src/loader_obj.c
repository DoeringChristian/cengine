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

    char line[500];

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
#if 0
            printf("tmpv: (pos: %f %f %f, normal: %f %f %f)\n", tmpv.pos[0], tmpv.pos[1], tmpv.pos[2], tmpv.normal[0], tmpv.normal[1], tmpv.normal[2]);
            printf("idxs[6]-1: %i\n", idxs[6]-1);
#endif
            darray_push_back(&verts, tmpv);

            glm_vec3_copy(v[idxs[1]-1].pos, tmpv.pos);
            glm_vec3_copy(vn[idxs[7]-1].normal, tmpv.normal);
            glm_vec2_copy(vt[idxs[4]-1].uv, tmpv.uv);
            darray_push_back(&verts, tmpv);

            glm_vec3_copy(v[idxs[2]-1].pos, tmpv.pos);
            glm_vec3_copy(vn[idxs[8]-1].normal, tmpv.normal);
            glm_vec2_copy(vt[idxs[5]-1].uv, tmpv.uv);
            darray_push_back(&verts, tmpv);

#if 0
            glm_vec3_copy(vn[idxs[3]].normal, v[idxs[0]].normal);
            glm_vec3_copy(vn[idxs[4]].normal, v[idxs[1]].normal);
            glm_vec3_copy(vn[idxs[5]].normal, v[idxs[2]].normal);
            glm_vec2_copy(vt[idxs[6]].uv, v[idxs[0]].uv);
            glm_vec2_copy(vt[idxs[7]].uv, v[idxs[1]].uv);
            glm_vec2_copy(vt[idxs[8]].uv, v[idxs[2]].uv);
#endif
        }
        //printf("%s\n", line);
    }


    mesh_init(dst, verts, darray_len(&verts), tris, darray_len(&tris));

    darray_free(&verts);
    darray_free(&v);
    darray_free(&tris);
    darray_free(&vn);
    darray_free(&vt);

    fclose(fp);
    return 0;
}
