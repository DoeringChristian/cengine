#version 330

// vertex data
layout(location = 0) in vec3 i_pos;

out vec3 frag_pos;

uniform mat4 u_view;
uniform mat4 u_proj;
uniform mat4 u_cview;

void main (void){
    vec3 pos = i_pos;
    mat4 view = mat4(mat3(u_view));
    mat4 cview = mat4(mat3(u_cview));

    frag_pos = vec3(vec4(pos, 1));


    gl_Position = (u_proj * view * inverse(cview) * vec4(pos, 1.0));
    //gl_Position = vec4(pos * 0.9, 1.0);
}
