#version 330

in vec2 frag_pos;
in vec2 frag_uv;

out vec4 o_color;

uniform sampler2D u_pos;
uniform sampler2D u_normal;
uniform sampler2D u_color;
uniform sampler2D u_light_prev;
uniform samplerCube u_shadow;

uniform vec4 u_light_pos;
uniform vec4 u_light_color;

uniform float u_shadow_len;

vec4 light_pos;
vec4 debug_out;

float shadow(vec3 pos){
    vec3 frag_to_light = pos - vec3(light_pos);
    float depth_closest = texture(u_shadow, frag_to_light).r;
    depth_closest *= u_shadow_len;

    debug_out = vec4(vec3(depth_closest), 1);

    float depth_cur = length(frag_to_light);

    float bias = 0.05;

    return depth_cur - bias > depth_closest ? 1.0 : 0.0;
}

void main (void){
    //o_color = texture(u_color, frag_uv);
    vec3 pos = vec3(texture(u_pos, frag_uv));
    vec3 normal = vec3(texture(u_normal, frag_uv));
    vec3 color = vec3(texture(u_color, frag_uv));
    float spec_strength = texture(u_color, frag_uv).a;

    // transform light pos to screen space
    light_pos = u_light_pos;

    vec3 light_dir = normalize(vec3(light_pos) - pos);

    // assume the camera is located at (0, 0, 0)
    vec3 view_dir = pos;
    vec3 light_dir_reflected = reflect(vec3(light_dir), normal);

    float spec = pow(max(dot(view_dir, light_dir_reflected), 0.0), 32);
    vec3 specular = spec_strength * spec * vec3(u_light_color) * u_light_color.a;

    vec3 diffuse = max(dot(normal, light_dir), 0) * vec3(u_light_color) * u_light_color.a;
    o_color = vec4((1 - shadow(pos)) * (specular + diffuse) * color, 1);

    // add light from previous passes.
    o_color += texture(u_light_prev, frag_uv);

    //o_color = debug_out;
    //o_color = vec4(texture(u_shadow, vec3(frag_uv.x, -1, frag_uv.y)).r, 0, 0, 1);
    //o_color = vec4(texture(u_shadow, pos - vec3(u_light_pos)).r, 0, 0, 1);
}
