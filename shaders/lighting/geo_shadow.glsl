#version 330

layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;

uniform mat4 u_view[6];
uniform mat4 u_proj;

out vec4 frag_pos;

void main (void){
    for(int face = 0; face < 6; face++){
        gl_Layer = face;
        for(int i = 0;i < 3;i++){
            frag_pos = gl_in[i].gl_Position;
            gl_Position = u_proj * u_view[face] * frag_pos;
            EmitVertex();
        }
        EndPrimitive();
    }
}

