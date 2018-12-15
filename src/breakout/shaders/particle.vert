#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;
out vec4 ParticleColor;

uniform mat4 projection;
uniform vec2 offset;
uniform vec4 color;
uniform float scale;

void main() {
    float scale = 10.0;
    TexCoords = aTexCoords;
    ParticleColor = color;
    gl_Position = projection * vec4((aPos * scale) + offset, 0.0, 1.0);
}
