#version 330 core

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
uniform sampler2D ssao;
uniform Light light;

void main() {
    vec3 Position = texture(gPosition, TexCoords).rgb;
    vec3 Normal = texture(gNormal, TexCoords).rgb;
    vec3 Albedo = texture(gAlbedoSpec, TexCoords).rgb;
    float Specular = texture(gAlbedoSpec, TexCoords).a;
    float ssaoOcculusion = texture(ssao, TexCoords).r;

    // ambient
    // vec3 lighting = Albedo * ssaoOcculusion * 0.3;
    vec3 lighting = Albedo * ssaoOcculusion;

    // vec3 viewDir = normalize(-Position);
    // vec3 lightDir = normalize(light.Position - Position);
    // vec3 diffuse = max(dot(lightDir, Normal), 0.0) * Albedo * light.Color;

    // vec3 halfwayDir = normalize(lightDir + viewDir);
    // float spec = pow(max(dot(halfwayDir, Normal), 0.0), 16.0);
    // vec3 specular = spec * Specular * light.Color;

    // float dist = length(light.Position - Position);
    // float attenuation = 1.0 / (1.0 + light.linear * dist + light.quad * dist);
    // diffuse *= attenuation;
    // specular *= attenuation;

    // lighting += diffuse + specular;

    FragColor = vec4(lighting, 1.0);
}
