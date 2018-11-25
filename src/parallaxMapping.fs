#version 330 core

in VS_OUT {
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} fs_in;

out vec4 FragColor;

uniform sampler2D floorTexture;
uniform sampler2D normalMap;
uniform sampler2D depthMap;
uniform float height_scale;
uniform bool blinn;

vec2 parallaxMapping(vec2 texCoords, vec3 viewDir);

void main() {
    vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    vec2 texCoords = parallaxMapping(fs_in.TexCoords, viewDir);

    if (texCoords.x > 1.0 || texCoords.y > 1.0 || texCoords.x < 0.0 || texCoords.y < 0.0)
        discard;

    vec3 normal = texture(normalMap, texCoords).rgb;
    normal = normalize(normal * 2.0 - 1.0);

    vec3 color = texture(floorTexture, texCoords).rgb;

    // ambient
    vec3 ambient = 0.05 * color;

    // diffuse
    vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;

    // specular
    float spec = 0.0;
    if (blinn) {
        vec3 halfwayDir = normalize(lightDir + viewDir);
        spec = pow(max(dot(halfwayDir, normal), 0.0), 32.0);
    } else {
        vec3 reflectDir = reflect(-lightDir, normal);
        spec = pow(max(dot(reflectDir, normal), 0.0), 8.0);
    }

    vec3 specular = vec3(0.3) * spec;

    FragColor = vec4(ambient + diffuse + specular, 1.0);
}

vec2 parallaxMapping(vec2 texCoords, vec3 viewDir) {
    // float height = texture(depthMap, texCoords).r;
    // vec2 p = viewDir.xy / viewDir.z * (height * height_scale);
    // return texCoords - p;

    // Steep Parallax Mapping
    const float minlayerNum = 8.0;
    const float maxlayerNum = 32.0;
    float layerNum = mix(maxlayerNum, minlayerNum, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));
    float layerDepth = 1.0 / layerNum;
    float currentLayerDepth = 0.0;
    vec2 P = viewDir.xy * height_scale;
    vec2 deltaTexCoords = P / layerNum;

    vec2 currentTexCoords = texCoords;
    float currentDepthMapValue = texture(depthMap, currentTexCoords).r;

    while (currentLayerDepth < currentDepthMapValue) {
        currentTexCoords -= deltaTexCoords;
        currentDepthMapValue = texture(depthMap, currentTexCoords).r;
        currentLayerDepth += layerDepth;
    }

    // return currentTexCoords;

    // Parallax Occlusion Mapping
    vec2 prevTexCoords = currentTexCoords + deltaTexCoords;
    float afterDepth = currentDepthMapValue - currentLayerDepth;
    float beforeDepth = texture(depthMap, prevTexCoords).r - (currentLayerDepth - layerDepth);
    float weight = afterDepth / (afterDepth - beforeDepth);
    vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1 - weight);

    return finalTexCoords;
}
