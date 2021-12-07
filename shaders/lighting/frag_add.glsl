#version 330

in vec2 frag_pos;
in vec2 frag_uv;

out vec4 o_color;

uniform sampler2D u_texture1;
uniform sampler2D u_texture2;

void main (void){
    o_color = texture(u_texture1, frag_uv) + texture(u_texture2, frag_uv);
}
