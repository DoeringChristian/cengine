#version 330

in vec3 frag_pos;

out vec4 o_color;

uniform samplerCube u_hdr;
uniform float u_roughness;
uniform int u_resolution;

const float PI = 3.14159265359;

float distribution_ggx(vec3 N, vec3 H, float roughness){
    float a = roughness * roughness;
    float a2 = a*a;
    float ndoth = max(dot(N, H), 0.0);
    float ndoth2 = ndoth * ndoth;

    float nom = a2;
    float denom = (ndoth2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom/denom;
}

float rad_inv_vdc(uint bits){
    bits = (bits << 16u) | (bits >> 16u);
    bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
    bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
    bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
    bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
    return float(bits) * 2.3283064365386963e-10; // / 0x100000000
}

vec2 hammersley(uint i, uint N){
    return vec2(float(i)/float(N), rad_inv_vdc(i));
}

vec3 importance_sample_ggx(vec2 Xi, vec3 N, float roughness){
    float a = roughness * roughness;

    float phi = 2.0 * PI * Xi.x;
    float costheta = sqrt((1.0 - Xi.y) / (1.0 + (a * a - 1.0) * Xi.y));
    float sintheta = sqrt(1.0 - costheta * costheta);

    vec3 H;
    H.x = cos(phi) * sintheta;
    H.y = sin(phi) * sintheta;
    H.z = costheta;

    vec3 up = abs(N.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
    vec3 tangent = normalize(cross(up, N));
    vec3 bitangent = cross(N, tangent);

    vec3 sample_vec = tangent * H.x + bitangent * H.y + N * H.z;
    return normalize(sample_vec);
}

void main (void){
    vec3 N = normalize(frag_pos);

    vec3 R = N;
    vec3 V = R;

    const uint SAMPLE_COUNT = 1024u;
    vec3 color = vec3(0.0);
    float weight_total;

    for(uint i = 0u;i < SAMPLE_COUNT;i++){
        vec2 Xi = hammersley(i, SAMPLE_COUNT);
        vec3 H = importance_sample_ggx(Xi, N, u_roughness);
        vec3 L = normalize(2.0 * dot(V, H) * H - V);

        float ndotl = max(dot(N, L), 0.0);
        if(ndotl > 0.0){
            float D = distribution_ggx(N, H, u_roughness);
            float ndoth = max(dot(N, H), 0.0);
            float hdotv = max(dot(H, V), 0.0);
            float pdf = D * ndoth / (4.0 * hdotv) + 0.001;
            
            float sa_texel = 5.0 * PI / (6.0 * u_resolution * u_resolution);
            float sa_sample = 1.0 / (float(SAMPLE_COUNT) * pdf + 0.0001);

            float mmlvl = u_roughness == 0.0 ? 0.0 : 0.5 * log2(sa_sample / sa_texel);
            color += textureLod(u_hdr, L, mmlvl).rgb * ndotl;
            weight_total += ndotl;
        }
    }
    color = color / weight_total;
    
    o_color = vec4(color, 1.0);

    // debug
    //o_color = vec4(1, 0, 0, 1);
}

