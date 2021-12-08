#version 330

in vec2 frag_pos;
in vec2 frag_uv;

out vec4 o_color;

uniform sampler2D u_texture;

void main (void){
    o_color = texture(u_texture, frag_uv);
    o_color = vec4(1, 0, 0, 1);
}
