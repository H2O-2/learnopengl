#version 330 core

#define NR_LIGHT 4

const float PI = 3.14159265359;

out vec4 FragColor;
in vec2 TexCoords;
in vec3 WorldPos;
in vec3 Normal;

uniform vec3 camPos;

uniform vec3 albedo;
uniform float metallic;
uniform float roughness;
uniform float ao;

uniform vec3 lightPosns[NR_LIGHT];
uniform vec3 lightColors[NR_LIGHT];

float distributionGGX(vec3 norm, vec3 halfwayDir, float roughness) {
    float a2 = roughness * roughness;
    float NdotH = max(dot(norm, halfwayDir), 0.0);
    float denom = (NdotH * NdotH) * (a2 - 1.0) + 1.0;
    return a2 / max(PI * denom * denom, 0.001);
}

float geometrySchlickGGX(float Ndot, float roughness) {
    float k = (roughness + 1.0) * (roughness + 1.0) / 8.0;
    return Ndot / (Ndot * (1 - k) + k);
}

float geometrySmith(vec3 norm, vec3 viewDir, vec3 lightDir, float roughness) {
    return geometrySchlickGGX(max(dot(norm, viewDir), 0.0), roughness) * geometrySchlickGGX(max(dot(norm, lightDir), 0.0), roughness);
}

vec3 fresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

void main() {
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(camPos - WorldPos);

    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);

    vec3 Lo = vec3(0.0); // Radiance of a point
    for (int i = 0; i < NR_LIGHT; ++i) {
        vec3 lightDir = normalize(lightPosns[i] - WorldPos);
        vec3 halfwayDir = normalize(lightDir + viewDir);

        // Radiance from each light
        float lightDist = length(lightPosns[i] - WorldPos);
        float attenuation = 1.0 / (lightDist * lightDist); // Use Inverse-square Law
        vec3 radiance = lightColors[i] * attenuation;

        // Cook-Torrance BRDF
        float D = distributionGGX(norm, halfwayDir, roughness);
        float G = geometrySmith(norm, viewDir, lightDir, roughness);
        vec3 F = fresnelSchlick(max(dot(halfwayDir, viewDir), 0.0), F0);
        // Specular part of BRDF
        vec3 brdfSpecular = D * G * F / max(4.0 * max(dot(lightDir, norm), 0.0) * max(dot(viewDir, norm), 0.0), 0.001); // add max(value, 0.001) to prevent divide by zero

        // Diffuse part of BRDF
        vec3 kD = vec3(1.0) - F; // Calculate the amount of light getting refracted according to Law of Energy Reservation. F is the specular contribution (kS)
        kD *= 1.0 - metallic; // nullify kD if material is metal since metal do not refract light
        vec3 brdfDiffuse = kD * albedo / PI;

        // Final BRDF
        Lo += (brdfDiffuse + brdfSpecular) * radiance * max(dot(norm, lightDir), 0.0);
    }

    vec3 ambient = vec3(0.03) * albedo * ao;
    vec3 color = ambient + Lo;
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2));

    FragColor = vec4(color, 1.0);
}

