#version 330

in vec4 frag_pos;
in vec3 frag_normal;
in vec4 frag_color;
in vec2 frag_uv;

layout (location = 0) out vec4 o_pos;
layout (location = 1) out vec4 o_normal;
layout (location = 2) out vec4 o_color;

//uniform sampler2D u_sampler[10];
uniform sampler2D u_albedo;
uniform sampler2D u_normal;
uniform sampler2D u_spec;

void main (void){
    o_pos = frag_pos;

    o_normal = vec4(normalize(frag_normal), 1);

    o_color = vec4(vec3(texture(u_albedo, frag_uv)), 1);
}
