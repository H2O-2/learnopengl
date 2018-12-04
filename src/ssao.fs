#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D texNoise;
uniform mat4 projection;
uniform vec3 samples[64];
uniform int kernelSize;
uniform float radius;
uniform float bias;

const vec2 noiseScale = vec2(1280.0 / 4.0, 720.0 / 4.0);

void main() {
    vec3 fragPos = texture(gPosition, TexCoords).xyz;
    vec3 normal = texture(gNormal, TexCoords).xyz;
    vec3 randomVec = texture(texNoise, TexCoords * noiseScale).xyz;
}
