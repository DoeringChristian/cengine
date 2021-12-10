#version 330

in vec2 frag_pos;
in vec2 frag_uv;

out vec4 o_color;

uniform sampler2D u_texture;

void main (void){
    vec4 color = texture(u_texture, frag_uv);
    o_color = length(color.xyz) > 1 ? color : vec4(0, 0, 0, 1);
}
