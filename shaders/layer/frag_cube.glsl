#version 330

in vec2 frag_pos;
in vec2 frag_uv;

out vec4 o_color;

uniform samplerCube u_texture;

void main (void){
    o_color = vec4(0, 0, 0, 1);
    if(frag_uv.x > 0.0 && frag_uv.x < 0.25 && frag_uv.y < 0.5 && frag_uv.y > 0.25){
        o_color = vec4(texture(u_texture, 4*vec3(frag_uv.x-0.0, frag_uv.y-0.25, -1)).r, 0, 0, 1);
    }
    else if(frag_uv.x > 0.25 && frag_uv.x < 0.5 && frag_uv.y < 0.5 && frag_uv.y > 0.25){
        o_color = vec4(texture(u_texture, 4*vec3(1, frag_uv.x-0.25, frag_uv.y-0.25)).r, 0, 0, 1);
    }
    else if(frag_uv.x > 0.5 && frag_uv.x < 0.75 && frag_uv.y < 0.5 && frag_uv.y > 0.25){
        o_color = vec4(texture(u_texture, 4*vec3(frag_uv.x-0.5, frag_uv.y-0.25, 1)).r, 0, 0, 1);
    }
    else if(frag_uv.x > 0.75 && frag_uv.x < 1.0 && frag_uv.y < 0.5 && frag_uv.y > 0.25){
        o_color = vec4(texture(u_texture, 4*vec3(-1, frag_uv.x-0.75, frag_uv.y-0.25)).r, 0, 0, 1);
    }
    else if(frag_uv.x > 0.5 && frag_uv.x < 0.75 && frag_uv.y < 0.25 && frag_uv.y > 0.0){
        o_color = vec4(texture(u_texture, 4*vec3(frag_uv.x-0.5, -1, frag_uv.y)).r, 0, 0, 1);
    }
    else if(frag_uv.x > 0.5 && frag_uv.x < 0.75 && frag_uv.y < 0.75 && frag_uv.y > 0.5){
        o_color = vec4(texture(u_texture, 4*vec3(frag_uv.x-0.5, 1, frag_uv.y-0.5)).r, 0, 0, 1);
    }
    else
        o_color = vec4(0, 0, 0, 1);
}
