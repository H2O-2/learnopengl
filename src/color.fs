#version 330 core
out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;

in vec3 FragPos;
in vec3 Normal;

void main() {
    float ambientStrength = 0.1;

    // diffuse
    vec3 normal = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    FragColor = vec4((ambientStrength + diff) * lightColor * objectColor, 1.0);
}
