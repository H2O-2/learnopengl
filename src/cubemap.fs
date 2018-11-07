#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 Position;
// in vec2 TexCoords;

uniform vec3 cameraPos;
uniform samplerCube texture1;
// uniform sampler2D texture1;

void main() {
    // FragColor = texture(texture1, TexCoords);

    // reflection
    // vec3 I = normalize(Position - cameraPos);
    // vec3 R = reflect(I, normalize(Normal));

    // refraction
    float ratio = 1.00 / 1.52;
    vec3 I = normalize(Position - cameraPos);
    vec3 R = refract(I, normalize(Normal), ratio);

    FragColor = texture(texture1, R);
}
