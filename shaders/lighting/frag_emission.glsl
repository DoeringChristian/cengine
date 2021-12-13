#version 330

in vec2 frag_pos;
in vec2 frag_uv;

out vec4 o_color;

uniform sampler2D u_pos;
uniform sampler2D u_normal;
uniform sampler2D u_albedo;
uniform sampler2D u_mrao;
uniform sampler2D u_emission;
uniform samplerCube u_shadow_depth;

uniform vec4 u_light_pos;
uniform vec4 u_light_color;
uniform float u_shadow_len;

uniform vec4 u_view_pos;

const float PI = 3.14159265359;

void main(void){
    vec3 emission = texture(u_emission, frag_uv).rgb;
    float emission_strength = texture(u_emission, frag_uv).a;

    o_color = vec4(emission * emission_strength, 1.0);
}
