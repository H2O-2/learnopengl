#version 330 core

out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
} fs_in;

uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

float ShadowCalculation(vec4 fragPosLightSpace, vec3 lightDir, vec3 normal) {
    // perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to texture coordinates
    projCoords = projCoords * 0.5 + 0.5;

    // float closestDepth = texture(shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;

    if (currentDepth > 1.0) {
        return 0.0;
    }

    // shadow acne
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    // float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;

    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for (int x = -1; x < 2; ++x) {
        for (int y = -1; y < 2; ++y) {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }

    shadow /= 9.0;

    return shadow;
}

void main() {
    vec3 color = texture(diffuseTexture, fs_in.TexCoords).rgb;
    vec3 ambient = 0.3 * color;
    vec3 lightColor = vec3(0.3);

    // diffuse
    vec3 lightDir = normalize(lightPos - fs_in.FragPos);
    vec3 normal = normalize(fs_in.Normal);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;

    // specular
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(halfwayDir, normal), 0.0), 64.0);
    vec3 specular = spec * lightColor;

    // shadow
    float shadow = ShadowCalculation(fs_in.FragPosLightSpace, lightDir, normal);
    vec3 lighting = (ambient + (1 - shadow) * (diffuse + specular)) * color;

    FragColor = vec4(lighting, 1.0);
}
