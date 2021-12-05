#version 330

in vec4 frag_color;
in vec2 frag_uv;

out vec4 o_color;

uniform sampler2D u_sampler[10];

void main (void){
    //o_color = frag_color;
    o_color = texture(u_sampler[0], frag_uv);
}
