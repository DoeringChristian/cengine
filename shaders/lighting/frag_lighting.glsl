#version 330

in vec2 frag_pos;
in vec2 frag_uv;

out vec4 o_color;

uniform sampler2D u_pos;
uniform sampler2D u_normal;
uniform sampler2D u_color;
uniform samplerCube u_shadow;

uniform vec4 u_light_pos;
uniform vec4 u_light_color;

uniform float u_far;

float shadow(vec3 pos){
    vec3 frag_to_light = pos - vec3(u_light_pos);
    float depth_closest = texture(u_shadow, frag_to_light).r;
    depth_closest *= u_far;
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

    vec3 light_dir = normalize(vec3(u_light_pos) - pos);

    // assume the camera is located at (0, 0, 0)
    vec3 view_dir = pos;
    vec3 light_dir_reflected = reflect(vec3(light_dir), normal);

    float spec = pow(max(dot(view_dir, light_dir_reflected), 0.0), 32);
    vec3 specular = spec_strength * spec * vec3(u_light_color);

    vec3 diffuse = max(dot(normal, light_dir), 0) * vec3(u_light_color);
    o_color = vec4((specular + diffuse) * color * (1- shadow(pos)), 1);
    o_color = vec4(texture(u_shadow, pos - vec3(u_light_pos)).r, 0, 0,1);
    //o_color = vec4(texture(u_shadow, pos).xyz, 1);
    //o_color = texture(u_shadow, pos - vec3(u_light_pos));
    //o_color = vec4(shadow(pos), 0, 0, 1);
    //o_color = vec4(vec3(texture(u_shadow, vec3(u_light_pos) - pos)), 1);
    //o_color = vec4(1, 0, 0, 1);
    //o_color = vec4(normal, 1);
    // ned to remove mabient light form light pass.
    //o_color = vec4((specular + diffuse + 0.1) * color, 1);
    //o_color = vec4(dot(normal, light_dir), -dot(normal, light_dir), 0, 1);
    //o_color = vec4(color, 1);
}
