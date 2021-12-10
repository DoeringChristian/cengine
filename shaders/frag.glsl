#version 330

in VS_OUT{
    vec3 pos;
    vec4 color;
    vec2 uv;
    mat3 tbn;
} fs_in;

layout (location = 0) out vec4 o_pos;
layout (location = 1) out vec4 o_normal;
layout (location = 2) out vec4 o_color;

//uniform sampler2D u_sampler[10];
uniform sampler2D u_albedo;
uniform sampler2D u_normal;
uniform sampler2D u_spec;

uniform bool u_has_albedo;
uniform bool u_has_normal;
uniform bool u_has_spec;

void main (void){
    o_pos = vec4(fs_in.pos, 1);

    o_normal = texture(u_normal, fs_in.uv);
    o_normal = o_normal * 2.0 - 1.0;
    o_normal = vec4(fs_in.tbn * vec3(o_normal), 1);
    o_normal = vec4(vec3(normalize(o_normal)), 1);


    if(!u_has_normal)
        o_normal = vec4(fs_in.tbn[2], 1);

    o_color = vec4(vec3(texture(u_albedo, fs_in.uv)), 1.0);
}
