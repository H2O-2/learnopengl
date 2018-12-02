#version 330 core

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_out;

uniform sampler2D textureDiffuse;
uniform sampler2D textureSpecular;

void main() {
    gPosition = fs_out.FragPos;
    gNormal = fs_out.Normal;
    // color
    gAlbedoSpec.rgb = texture(textureDiffuse, fs_out.TexCoords).rgb;
    // specular
    gAlbedoSpec.a = texture(textureSpecular, fs_out.TexCoords).r;
}
