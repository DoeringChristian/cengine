#version 330

in VS_OUT{
    vec3 pos;
    vec4 color;
    vec2 uv;
    mat3 tbn;
} fs_in;

layout (location = 0) out vec4 o_pos;
layout (location = 1) out vec4 o_normal;
layout (location = 2) out vec4 o_albedo;
layout (location = 3) out vec4 o_mrao;
layout (location = 4) out vec4 o_emission;

uniform sampler2D u_albedo_map;
uniform sampler2D u_normal_map;
uniform sampler2D u_mrao_map;
uniform sampler2D u_emission_map;

uniform bool u_has_albedo;
uniform bool u_has_normal;
uniform bool u_has_mrao;
uniform bool u_has_emission;

uniform vec4 u_albedo;
uniform vec4 u_mrao;
uniform vec4 u_emission;

uniform float u_normal_scale;

void main (void){
    o_pos = vec4(fs_in.pos, 1);

    o_normal = texture(u_normal_map, fs_in.uv);
    o_normal = o_normal * 2.0 - 1.0;
    o_normal.xy *= u_normal_scale;
    o_normal = vec4(fs_in.tbn * vec3(o_normal), 1);
    o_normal = vec4(vec3(normalize(o_normal)), 1);

    o_albedo = vec4(vec3(texture(u_albedo_map, fs_in.uv)), 1.0);

    o_mrao = vec4(texture(u_mrao_map, fs_in.uv));

    o_emission = texture(u_emission_map, fs_in.uv) * u_emission.a;

    if(!u_has_normal)
        o_normal = vec4(fs_in.tbn[2], 1);

    if(!u_has_albedo)
        o_albedo = u_albedo;
    if(!u_has_mrao)
        o_mrao = u_mrao;
    if(!u_has_emission)
        o_emission = u_emission;
}
