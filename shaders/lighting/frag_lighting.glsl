#version 330

in vec2 frag_pos;
in vec2 frag_uv;

out vec4 o_color;

uniform sampler2D u_pos;
uniform sampler2D u_normal;
uniform sampler2D u_color;

uniform vec4 u_light_pos;
uniform vec4 u_light_color;

void main (void){
    //o_color = texture(u_color, frag_uv);
    vec3 pos = vec3(texture(u_pos, frag_uv));
    vec3 normal = vec3(texture(u_normal, frag_uv));
    vec3 color = vec3(texture(u_color, frag_uv));

    vec3 light_dir = normalize(vec3(u_light_pos) - pos);
    vec3 diffuse = max(dot(normal, light_dir), 0) * color * vec3(u_light_color);
    o_color = vec4(diffuse, 1);
}
