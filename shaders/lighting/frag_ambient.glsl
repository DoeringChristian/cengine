#version 330

in vec2 frag_pos;
in vec2 frag_uv;

out vec4 o_color;

uniform sampler2D u_pos;
uniform sampler2D u_normal;
uniform sampler2D u_albedo;
uniform sampler2D u_mrao;
uniform sampler2D u_emission;

uniform samplerCube u_irr;
uniform samplerCube u_ref;
uniform sampler2D u_brdf;

uniform int u_ref_lod_max;

uniform vec4 u_view_pos;

const float PI = 3.14159265359;


float shadow(vec3 pos);
float distribution_ggx(vec3 N, vec3 H, float roughness);
float geometry_schlick_ggx(float ndotv, float roughness);
float geometry_smith(vec3 N, vec3 V, vec3 L, float roughness);
vec3 fresnel_schlick(float costheta, vec3 F0);
vec3 fresnel_schlick_roughness(float costheta, vec3 F0, float roughness);

void main (void){
    // ------------------------------------------------
    // Initialize the values from the deferred rederer textures.
    vec3 pos = vec3(texture(u_pos, frag_uv));
    vec3 normal = vec3(texture(u_normal, frag_uv));
    vec3 albedo = vec3(texture(u_albedo, frag_uv));
    float metallic = texture(u_mrao, frag_uv).r;
    float roughness = texture(u_mrao, frag_uv).g;
    float ao = texture(u_mrao, frag_uv).b;


    vec3 N = normalize(normal);
    vec3 V = normalize(vec3(u_view_pos) - pos);
    vec3 R = reflect(-V, N);
    
    // -------------------------------------------------
    // Fresnel calculation:
    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);
   

    vec3 kS = fresnel_schlick(max(dot(N, V), 0.0), F0);
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - metallic;
    vec3 irradiance = texture(u_irr, N).rgb;
    vec3 diffuse = irradiance * albedo;


    vec3 ref_color = textureLod(u_ref, R, roughness * u_ref_lod_max).rgb;
    vec3 F = fresnel_schlick_roughness(max(dot(N, V), 0.0), F0, roughness);
    vec2 brdf_env = texture(u_brdf, vec2(max(dot(N, V), 0.0), roughness)).rg;
    vec3 specular = ref_color * (F * brdf_env.x + brdf_env.y);

    vec3 ambient = (kD * diffuse + specular) * ao;

    o_color = vec4(ambient, texture(u_albedo, frag_uv).a);

    // TODO: need to transform u_ref

    // debug
    //o_color = vec4(texture(u_albedo, frag_uv).a, texture(u_albedo, frag_uv).a);
    //o_color = vec4(kD * specular, 1);
    //o_color = vec4(V, 1);
    //o_color = vec4(V, 1);



    // debug
    //o_color = vec4(texture(u_albedo, frag_uv).a, 0, 0, 1);
    //o_color = vec4(texture(u_albedo, frag_uv).a > 0.9 ? 1 : 0, 0, 0, 1);
    //o_color = vec4(vec3(length(vec3(u_view_pos) - pos)/100), 1);
}

float distribution_ggx(vec3 N, vec3 H, float roughness){
    float a = roughness * roughness;
    float a2 = a*a;
    float ndoth = max(dot(N, H), 0.0);
    float ndoth2 = ndoth * ndoth;

    float num = a2;
    float denom = (ndoth2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return num / denom;
}
float geometry_schlick_ggx(float ndotv, float roughness){
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float num = ndotv;
    float denom = ndotv * (1.0 - k) + k;

    return num / denom;
}
float geometry_smith(vec3 N, vec3 V, vec3 L, float roughness){
    float ndotv = max(dot(N, V), 0.0);
    float ndotl = max(dot(N, L), 0.0);
    float ggx2 = geometry_schlick_ggx(ndotv, roughness);
    float ggx1 = geometry_schlick_ggx(ndotl, roughness);

    return ggx1 * ggx2;
}
vec3 fresnel_schlick(float costheta, vec3 F0){
    return F0 + (1.0 - F0) * pow(clamp(1.0 - costheta, 0.0, 1.0), 5.0);
}
vec3 fresnel_schlick_roughness(float costheta, vec3 F0, float roughness){
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(clamp(1.0 - costheta, 0.0, 1.0), 5.0);
}
#if 0
float shadow(vec3 pos){
    vec3 frag_to_light = pos - vec3(u_light_pos);
    float depth_closest = texture(u_shadow_depth, frag_to_light).r;
    depth_closest *= u_shadow_len;

    float depth_cur = length(frag_to_light);

    float bias = 0.05;

    return depth_cur - bias > depth_closest ? 1.0 : 0.0;
}
#endif
