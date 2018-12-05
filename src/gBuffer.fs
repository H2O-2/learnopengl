#version 330 core

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;
layout (location = 3) out vec3 specular;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_out;

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
};
uniform Material material;

void main() {
    gPosition = fs_out.FragPos;
    gNormal = fs_out.Normal;
    // color
    gAlbedoSpec.rgb = texture(material.texture_diffuse1, fs_out.TexCoords).rgb;
    // specular
    gAlbedoSpec.a = texture(material.texture_specular1, fs_out.TexCoords).r;
}
