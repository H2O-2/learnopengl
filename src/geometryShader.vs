#version 330 core
layout (location = 0) in vec3 aPos;
// layout (location = 1) in vec3 aColor;

// layout (location = 2) in vec2 aTexCoords;

layout (location = 1) in vec3 aNormal;

out VS_OUT {
    // vec3 color;

    // vec2 texCoords;

    vec4 normal;
} vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main() {
    // gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
    // vs_out.color = aColor;

    // vs_out.texCoords = aTexCoords;

    vs_out.normal = normalize(projection * vec4(mat3(transpose(inverse(view * model))) * aNormal, 0.0));
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}
