#version 330

// vertex data
layout(location = 0) in vec3 i_pos;
layout(location = 1) in vec3 i_normal;
layout(location = 2) in vec4 i_color;
layout(location = 3) in vec2 i_uv;

// instance data
layout(location = 4) in mat4 i_trans;
layout(location = 8) in float i_tex_idx_offset;

out vec4 frag_pos;
out vec3 frag_normal;
out vec4 frag_color;
out vec2 frag_uv;

// global mesh transforms
uniform mat4 u_trans;
uniform mat4 u_proj;

void main (void){
    mat4 mv_matrix = u_proj * i_trans * u_trans;
    mat3 norm_matrix = transpose(inverse(mat3(mv_matrix)));
    frag_pos = mv_matrix * vec4(i_pos, 1.0);
    gl_Position = mv_matrix * vec4(i_pos, 1.0);
    frag_color = i_color;
    //frag_normal = vec4(mat3(i_trans) * mat3(u_trans) * i_normal, 1.0);
    frag_normal = normalize(norm_matrix * i_normal);
    frag_uv = i_uv;
}
