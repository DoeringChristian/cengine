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

    float axis[3] = {0, 1, 0};
    float quat[4];
    quat_from_axis_angle(quat, axis, 1);
    memcpy(mesh.rot, quat, sizeof(quat));

    struct texture tex1;
    texture_load(&tex1, "res/img/test02.jpg");

    mesh_texture_push(&mesh, tex1);

    renderer_mesh_push(&win.renderer, &mesh);

    mesh_update(&mesh);

    window_main(&win);

    window_free(&win);
}

