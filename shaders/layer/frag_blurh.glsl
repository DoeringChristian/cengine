#version 330

in vec2 frag_pos;
in vec2 frag_uv;

out vec4 o_color;

uniform sampler2D u_texture;

float weights[5] = float[] (0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);

void main (void){
    vec2 tex_offset = 1.0/textureSize(u_texture, 0);
    //o_color = texture(u_texture, frag_uv).rgb * weights[0];
    
    for(int i = -5;i <= 5;i++){
        o_color += texture(u_texture, frag_uv + vec2(tex_offset.x * i, 0.0)).rgb * weights[i];
    }
}
