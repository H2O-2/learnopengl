#version 330 core

// in vec3 fColor;

// struct Material {
//     sampler2D texture_diffuse1;
//     sampler2D texture_specular1;
// };

// in vec2 TexCoords;

out vec4 FragColor;

// uniform Material material;

void main() {
    // FragColor = vec4(0.0, 1.0, 0.0, 1.0);

    // FragColor = vec4(fColor, 1.0);

    // FragColor = texture(material.texture_diffuse1, TexCoords);

    FragColor = vec4(1.0, 1.0, 0.0, 1.0);
}
