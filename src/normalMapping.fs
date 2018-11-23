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
uniform bool blinn;

void main() {
    vec3 normal = texture(normalMap, fs_in.TexCoords).rgb;
    normal = normalize(normal * 2.0 - 1.0);

    vec3 color = texture(floorTexture, fs_in.TexCoords).rgb;

    // ambient
    vec3 ambient = 0.05 * color;

    // diffuse
    vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * color;

    // specular
    vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
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
