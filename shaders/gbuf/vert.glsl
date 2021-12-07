#version 330

// vertex data
layout(location = 0) in vec2 i_pos;
layout(location = 1) in vec2 i_uv;

out vec2 frag_pos;
out vec2 frag_uv;

void main (void){
    gl_Position = vec4(i_pos, 0, 1);
    frag_uv = i_uv;
}
