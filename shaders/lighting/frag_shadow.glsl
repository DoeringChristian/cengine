#version 330

in vec4 frag_pos;

uniform vec4 u_light_pos;
uniform float u_far;

void main (void){
    float light_dist = length(vec3(frag_pos) - vec3(u_light_pos));
    
    light_dist = light_dist/u_far;

    //light_dist = 0.1;

    gl_FragDepth = light_dist;
}
