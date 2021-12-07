#version 330

in vec4 frag_pos;
in vec4 frag_normal;
in vec4 frag_color;
in vec2 frag_uv;

layout (location = 0) out vec4 o_pos;
layout (location = 1) out vec4 o_normal;
layout (location = 2) out vec4 o_color;

uniform sampler2D u_sampler[10];

void main (void){
    o_pos = frag_pos;

    o_normal = normalize(frag_normal);

    o_color = texture(u_sampler[0], frag_uv);
}
