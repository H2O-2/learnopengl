#version 330 core

out float FragColor;

in vec2 TexCoords;

uniform sampler2D ssaoInput;

void main() {
    vec2 texelSize = 1.0 / textureSize(ssaoInput, 0);
    float result = 0.0;
    for (int i = -2; i < 2; ++i) {
        for (int j = -2; j < 2; ++j) {
            result += texture(ssaoInput, TexCoords + vec2(float(i), float(j)) * texelSize).r;
        }
    }

    FragColor = result / 16.0;
}
