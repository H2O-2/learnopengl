#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoords;
out vec3 WorldPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    TexCoords = aTexCoord;
    WorldPos = vec3(model * vec4(aPosition, 1.0));
    Normal = mat3(model) * aNormal; // since there will be no non-uniform scaling we simply multiply 3x3 model matrix
    gl_Position = projection * view * model * vec4(aPosition, 1.0);
}
