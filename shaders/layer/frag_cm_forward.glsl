#version 330

in vec3 frag_pos;

out vec4 o_color;

uniform samplerCube u_cubemap;

void main (void){
    o_color = texture(u_cubemap, frag_pos);
    //o_color = vec4(0.1, 0, 0, 1);
}
