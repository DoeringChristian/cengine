#version 330

in vec4 frag_pos;

uniform vec4 u_light_pos;
uniform float u_far;

void main (void){
    float light_dist = length(vec3(frag_pos) - vec3(u_light_pos));
    
    light_dist = light_dist/u_far;

    // why is this not working
    //light_dist = 0.5;
    //light_dist = u_light_pos.y;
    //light_dist = u_far / 200;
    //light_dist = 0.01;

    gl_FragDepth = light_dist;
}
