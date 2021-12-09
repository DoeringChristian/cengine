#version 330

// vertex data
layout(location = 0) in vec3 i_pos;
layout(location = 1) in vec3 i_normal;
layout(location = 2) in vec4 i_color;
layout(location = 3) in vec2 i_uv;

// instance data
layout(location = 4) in mat4 i_instance;
layout(location = 8) in float i_tex_idx_offset;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_proj;

out vec4 frag_pos;

void main (void){
    frag_pos = i_instance * u_model * vec4(i_pos, 1);
    gl_Position = u_proj * u_view * frag_pos;
}
