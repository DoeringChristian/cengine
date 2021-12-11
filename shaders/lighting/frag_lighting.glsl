#version 330

in vec2 frag_pos;
in vec2 frag_uv;

out vec4 o_color;

uniform sampler2D u_pos;
uniform sampler2D u_normal;
uniform sampler2D u_albedo;
uniform sampler2D u_mrao;
uniform sampler2D u_emission;
uniform samplerCube u_shadow_depth;

uniform vec4 u_light_pos;
uniform vec4 u_light_color;
uniform float u_light_c1;
uniform float u_light_c2;

uniform float u_shadow_len;

uniform vec4 u_view_pos;

float shadow(vec3 pos){
    vec3 frag_to_light = pos - vec3(u_light_pos);
    float depth_closest = texture(u_shadow_depth, frag_to_light).r;
    depth_closest *= u_shadow_len;

    float depth_cur = length(frag_to_light);

    float bias = 0.05;

    return depth_cur - bias > depth_closest ? 1.0 : 0.0;
}

float attenuation(float dist){
    return 1.0/(1.0 + u_light_c2 * dist + u_light_c2 * dist * dist);
}

void main (void){
    //o_color = texture(u_color, frag_uv);
    vec3 pos = vec3(texture(u_pos, frag_uv));
    vec3 normal = vec3(texture(u_normal, frag_uv));
    vec3 albedo = vec3(texture(u_albedo, frag_uv));
    float metallic = texture(u_mrao, frag_uv).x;
    float roughness = texture(u_mrao, frag_uv).y;
    float ao = texture(u_mrao, frag_uv).z;
    vec3 emission = texture(u_emission, frag_uv).rgb;
    float emission_strength = texture(u_emission, frag_uv).y;

    float spec_strength = texture(u_albedo, frag_uv).a;

    vec3 light_dir = normalize(vec3(u_light_pos) - pos);
    float light_dist = length(vec3(u_light_pos) - pos);

    vec3 view_dir = normalize(vec3(u_view_pos) - pos);
    vec3 light_dir_reflected = reflect(-vec3(light_dir), normal);

    // ---------------------------------------------------
    // specular calculations

    float spec = pow(max(dot(view_dir, light_dir_reflected), 0.0), 32);
    vec3 specular = spec_strength * spec * vec3(u_light_color) * u_light_color.a;

    // ---------------------------------------------------
    // difuse calculations
    
    vec3 diffuse = max(dot(normal, light_dir), 0) * vec3(u_light_color) * u_light_color.a;

    // ---------------------------------------------------
    // output calculation

    o_color = vec4((1 - shadow(pos)) * (specular + diffuse) * albedo * attenuation(light_dist), 1.0);
    o_color += vec4(emission * emission_strength, 0);
    //o_color = vec4((1 - shadow(pos)) * (specular + diffuse) * color * attenuation(light_dist) * 2, 1.0);
}
