#version 330

in vec3 frag_pos;

out vec4 o_color;

uniform sampler3D u_texture;

void main (void){
    vec3 frag_color = texture(u_texture, frag_pos).xyz;

    frag_color = frag_color / (frag_color + vec3(1.0));
    frag_color = pow(frag_color, vec3(1.0 / 2.2));

    o_color = vec4(frag_color, 1.0);
    o_color = vec4(1, 0, 0, 1);
}
