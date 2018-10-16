#version 330 core
out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
// use when calculating in world coordinate
// uniform vec3 viewPos;

in vec3 FragPos;
in vec3 LightPos;
in vec3 Normal;

void main() {
    float ambientStrength = 0.1;
    float specularStrength = 0.5;

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(LightPos - FragPos);
    vec3 viewDir = normalize(-FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float diff = max(dot(norm, lightDir), 0.0);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 128);
    FragColor = vec4((ambientStrength + diff + specularStrength * spec) * lightColor * objectColor, 1.0);
}
