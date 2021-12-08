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
    float spec_strength = texture(u_color, frag_uv).a;

    vec3 light_dir = normalize(vec3(u_light_pos) - pos);

    // assume the camera is located at (0, 0, 0)
    vec3 view_dir = pos;
    vec3 light_dir_reflected = reflect(vec3(light_dir), normal);

    float spec = pow(max(dot(view_dir, light_dir_reflected), 0.0), 32);
    vec3 specular = spec_strength * spec * vec3(u_light_color);

    vec3 diffuse = max(dot(normal, light_dir), 0) * vec3(u_light_color);
    o_color = vec4((specular + diffuse) * color, 1);
    //o_color = vec4(normal, 1);
    // ned to remove mabient light form light pass.
    //o_color = vec4((specular + diffuse + 0.1) * color, 1);
    //o_color = vec4(dot(normal, light_dir), -dot(normal, light_dir), 0, 1);
    //o_color = vec4(color, 1);
}
