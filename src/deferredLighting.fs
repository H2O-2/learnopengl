#version 330 core

const int NR_LIGHTS = 32;

out vec4 FragColor;

in vec2 TexCoords;

struct Light {
    vec3 Position;
    vec3 Color;
    float linear;
    float quad;
};

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
uniform vec3 viewPos;
uniform Light lights[NR_LIGHTS];

void main() {
    vec3 Position = texture(gPosition, TexCoords).rgb;
    vec3 Normal = texture(gNormal, TexCoords).rgb;
    vec3 Albedo = texture(gAlbedoSpec, TexCoords).rgb;
    float Specular = texture(gAlbedoSpec, TexCoords).a;

    // ambient
    vec3 lighting = Albedo * 0.1;

    vec3 viewDir = normalize(viewPos - Position);
    for (int i = 0; i < NR_LIGHTS; ++i) {
        vec3 lightDir = normalize(lights[i].Position - Position);
        vec3 diffuse = max(dot(lightDir, Normal), 0.0) * Albedo * lights[i].Color;

        vec3 halfwayDir = normalize(lightDir + viewDir);
        float spec = pow(max(dot(halfwayDir, Normal), 0.0), 16.0);
        vec3 specular = spec * Specular * lights[i].Color;

        float dist = length(lights[i].Position - Position);
        float attenuation = 1.0 / (1.0 + lights[i].linear * dist + lights[i].quad * dist);
        diffuse *= attenuation;
        specular *= attenuation;

        lighting += diffuse + specular;
    }

    FragColor = vec4(lighting, 1.0);
}
