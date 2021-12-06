#version 330

in vec4 frag_pos;
in vec4 frag_normal;
in vec4 frag_color;
in vec2 frag_uv;

out vec4 o_color;

uniform sampler2D u_sampler[10];
uniform sampler2D u_lights;
uniform int u_lights_w;
uniform int u_lights_h;

void main (void){
    vec3 diff_color_sum = vec3(0, 0, 0);
    for(int i = 0;i < u_lights_w;i++){
        vec3 light_pos = vec3(texture(u_lights, vec2(i / u_lights_w, 0 / u_lights_h)));
        vec4 light_color = vec4(texture(u_lights, vec2(i / u_lights_w, 1 / u_lights_h)));

        vec3 dir = light_pos - vec3(frag_pos);
        float light_strength = light_color[3];
        float diff = max(dot(vec3(frag_normal), dir), 0);
        vec3 diff_color = diff * vec3(light_color) * light_strength;
        //diff_color_sum += diff_color;
        diff_color_sum += vec3(light_color);
    }
    //o_color = frag_color;
    o_color = texture(u_sampler[0], frag_uv) * vec4(diff_color_sum, 1.0);
    //o_color = vec4(texture(u_lights, vec2(0 / u_lights_w, 1 / u_lights_h)));
    //o_color = vec4(diff_color_sum, 1.0);
    //o_color = texture(u_lights, vec2(0, 0));
}
