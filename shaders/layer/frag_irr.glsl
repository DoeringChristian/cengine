#version 330

in vec3 frag_pos;

out vec4 o_color;

uniform samplerCube u_hdr;

const float PI = 3.14159265359;

void main (void){
    vec3 normal = normalize(frag_pos);
    vec3 irradiance = vec3(0.0);

    vec3 up = vec3(0, 1, 0);
    vec3 right = normalize(cross(up, normal));
    up = normalize(cross(normal, right));

    float sample_delta = 0.025;
    float sample_nr = 0.0;
    for(float phi = 0.0; phi < 2.0 * PI; phi += sample_delta){
        for(float theta = 0.0; theta < 0.5 * PI; theta += sample_delta){
            vec3 tangent_sample = vec3(sin(theta) * cos(phi), sin(theta) * sin(phi), cos(theta));
            vec3 sample_vec = tangent_sample.x * right + tangent_sample.y * up + tangent_sample.z * normal;

            irradiance += texture(u_hdr, sample_vec).rgb * cos(theta) * sin(theta);

            sample_nr++;
        }
    }
    irradiance = PI * irradiance * (1.0 / float(sample_nr));

    o_color = vec4(irradiance, 1.0);
}

