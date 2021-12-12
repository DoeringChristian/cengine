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
uniform float u_shadow_len;

uniform vec4 u_view_pos;

const float PI = 3.14159265359;


float shadow(vec3 pos);
float distribution_ggx(vec3 N, vec3 H, float roughness);
float geometry_schlick_ggx(float ndotv, float roughness);
float geometry_smith(vec3 N, vec3 V, vec3 L, float roughness);
vec3 fresnel_schlick(float costheta, vec3 F0);

void main (void){
    // ------------------------------------------------
    // Initialize the values from the deferred rederer textures.
    vec3 pos = vec3(texture(u_pos, frag_uv));

    if(length(vec3(u_light_pos) - pos) > u_shadow_len){
        o_color = vec4(0, 0, 0, 1);
        return;
    }

    vec3 normal = vec3(texture(u_normal, frag_uv));
    vec3 albedo = vec3(texture(u_albedo, frag_uv));
    float metallic = texture(u_mrao, frag_uv).r;
    float roughness = texture(u_mrao, frag_uv).g;
    float ao = texture(u_mrao, frag_uv).b;
    vec3 emission = texture(u_emission, frag_uv).rgb;
    float emission_strength = texture(u_emission, frag_uv).y;


    vec3 N = normalize(normal);
    vec3 V = normalize(vec3(u_view_pos) - pos);
    vec3 L = normalize(vec3(u_light_pos) - pos);
    vec3 H = normalize(V + L);
    float distance = length(vec3(u_light_pos) - pos);
    float attenuation = 1.0/(distance * distance);
    vec3 radiance = vec3(u_light_color) * attenuation;

    // -------------------------------------------------
    // Fresnel calculation:
    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);
    vec3 F = fresnel_schlick(max(dot(H, V), 0.0), F0);

    // -------------------------------------------------
    // Reflectance:
    float NDF = distribution_ggx(N, H, roughness);
    float G = geometry_smith(N, V, L, roughness);

    // -------------------------------------------------
    // Cook Torrance BRDF:
    vec3 numerator = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
    vec3 specular = numerator / denominator;

    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;

    kD *= 1.0 - metallic;

    float ndotl = max(dot(N, L), 0.0);

    vec3 lo = (kD * albedo / PI + specular) * radiance * ndotl * (1 - shadow(pos));
    o_color = vec4(lo, texture(u_albedo, frag_uv).a);

    // debug
    //o_color = vec4(texture(u_albedo, frag_uv).a, 0, 0, 1);
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
float shadow(vec3 pos){
    vec3 frag_to_light = pos - vec3(u_light_pos);
    float depth_closest = texture(u_shadow_depth, frag_to_light).r;
    depth_closest *= u_shadow_len;

    float depth_cur = length(frag_to_light);

    float bias = 0.05;

    return depth_cur - bias > depth_closest ? 1.0 : 0.0;
}
