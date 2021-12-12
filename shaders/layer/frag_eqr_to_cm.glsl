#version 330

in vec3 frag_pos;

out vec4 o_color;

uniform sampler2D u_texture;

const vec2 inv_atan = vec2(0.1591, 0.3183);
vec2 sphere_sample(vec3 v){
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= inv_atan;
    uv += 0.5;
    return uv;
}
void main (void){
    vec2 uv = sphere_sample(normalize(frag_pos));
    vec3 color = texture(u_texture, uv).rgb;

    o_color = vec4(color, 1.0);
    //o_color = vec4(uv, 0, 1);
}
