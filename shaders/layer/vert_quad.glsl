#version 330

// vertex data
layout(location = 0) in vec2 i_pos;

out vec2 frag_pos;
out vec2 frag_uv;

void main (void){
    gl_Position = vec4(i_pos, 0, 1);
    frag_pos = i_pos;
    frag_uv = i_uv;
}
