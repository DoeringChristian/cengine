#version 330

in vec2 frag_pos;
in vec2 frag_uv;

out vec4 o_color;

uniform sampler2D u_texture;
uniform float u_gamma;

void main (void){
    o_color = texture(u_texture, frag_uv);
    o_color.rgb = pow(o_color.rgb, vec3(1.0/u_gamma));
}
