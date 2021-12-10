#version 330

// vertex data
layout(location = 0) in vec3 i_pos;
layout(location = 1) in vec3 i_normal;
layout(location = 2) in vec3 i_tangent;
layout(location = 3) in vec4 i_color;
layout(location = 4) in vec2 i_uv;

// instance data
layout(location = 5) in mat4 i_instance;
layout(location = 9) in float i_tex_idx_offset;

#if 0
out vec4 frag_pos;
out vec3 frag_normal;
out vec4 frag_color;
out vec2 frag_uv;
#endif

out VS_OUT{
    vec3 pos;
    vec4 color;
    vec2 uv;
    mat3 tbn;
} vs_out;

// global mesh transforms
uniform mat4 u_model;
uniform mat4 u_proj;
uniform mat4 u_view;

void main (void){
    mat4 m_matrix = i_instance * u_model;
    mat4 pvm_matrix = u_proj * u_view * m_matrix;
    mat3 norm_matrix = transpose(inverse(mat3(m_matrix)));

    gl_Position = pvm_matrix * vec4(i_pos, 1.0);

    // apply only instance and model to fragment position
    vs_out.pos = vec3(m_matrix * vec4(i_pos, 1.0));

    // forward the color
    vs_out.color = i_color;

    // set the fragment normal 
    //frag_normal = normalize(norm_matrix * i_normal);

    vs_out.uv = i_uv;
    //frag_uv = i_uv;

    vec3 normal = normalize(mat3(m_matrix) * i_normal);
    vec3 tangent = normalize(mat3(m_matrix) * i_tangent);
    vec3 bitangent = cross(normal, tangent);
    vs_out.tbn = mat3(tangent, bitangent, normal);
}
