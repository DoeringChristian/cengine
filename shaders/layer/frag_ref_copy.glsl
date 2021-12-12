#version 330

in vec3 frag_pos;

out vec4 o_color;

uniform samplerCube u_cubemap;
uniform int u_mm;

void main (void){
    o_color = textureLod(u_cubemap, frag_pos, u_mm);
    //o_color = vec4(0.1, 0, 0, 1);
}
