#version 330

// vertex data
layout(location = 0) in vec3 i_pos;
layout(location = 1) in vec3 i_normal;
layout(location = 2) in vec4 i_color;
layout(location = 3) in vec2 i_uv;

// instance data
layout(location = 4) in mat4 i_trans;
layout(location = 8) in float i_tex_idx_offset;

out vec4 frag_color;
out vec2 frag_uv;

// global mesh transforms
uniform mat4 u_trans;

void main (void){
    gl_Position = i_trans * u_trans * vec4(i_pos, 1.0);
    frag_color = i_color;
    frag_uv = i_uv;
}
