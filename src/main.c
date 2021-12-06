#include <stdint.h>
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <assert.h>
#include <SDL2/SDL.h>
#include "window.h"
#include "mesh.h"

int main(){
    struct window win;
    struct mesh mesh;

    window_init(&win, 0, 0, 800, 600, "test");

    mesh_init(&mesh, &win.renderer.shader);

    mesh_vert_push_back(&mesh, (struct vert){.pos = {0, 0, 0}, .color = {1, 0, 0, 1}, .uv = {0, 0}});
    mesh_vert_push_back(&mesh, (struct vert){.pos = {0, 1, 0}, .color = {0, 1, 0, 1}, .uv = {0, 1}});
    mesh_vert_push_back(&mesh, (struct vert){.pos = {1, 0, 0}, .color = {0, 0, 1, 1}, .uv = {1, 0}});

    mesh_tri_push_back(&mesh, tri(0, 1, 2));

#if 0
    float axis[3] = {0, 1, 0};
    float quat[4];
    quat_from_axis_angle(quat, axis, 0);
    memcpy(mesh.rot, quat, sizeof(quat));
#endif

    //instance

#if 0
    struct ivert i2;
    mat4_rotation_axis(i2.trans, (float []){1, 0, 1}, -1);
    i2.tex_idx_offset = 0;

    mesh_ivert_push_back(&mesh, i2);
#endif

    struct ivert iv1;
    mat4_rotation_axis(iv1.trans, (float []){1, 0, 1}, -1);
    iv1.tex_idx_offset = 0;

    struct instance i1;
    instance_init(&i1, &mesh);
    i1.ivert = iv1;
    renderer_instance_push(&win.renderer, &i1);

    struct ivert iv2;
    mat4_rotation_axis(iv2.trans, (float []){1, 0, 0}, -1);
    iv2.tex_idx_offset = 0;

    struct instance i2;
    instance_init(&i2, &mesh);
    i2.ivert = iv2;
    renderer_instance_push(&win.renderer, &i2);

    // texture
    struct texture tex1;
    texture_load(&tex1, "res/img/ca-env-0105.jpg");

    mesh_texture_push(&mesh, tex1);

    renderer_mesh_push(&win.renderer, &mesh);

    renderer_push(&win.renderer);
    //mesh_push(&mesh);

    window_main(&win);

    window_free(&win);
}

