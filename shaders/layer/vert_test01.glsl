#version 330

// vertex data
layout(location = 0) in vec3 i_pos;

out vec3 frag_pos;

uniform mat4 u_view;
uniform mat4 u_proj;

void main (void){
    vec3 pos = i_pos * 2 - 1;

    mat4 view = mat4(mat3(u_view));

    frag_pos = i_pos;

    gl_Position = u_proj * view * vec4(pos, 1.0);
}
