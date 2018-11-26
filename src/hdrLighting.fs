#version 330 core

out vec4 FragPos;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

struct Light {
    vec3 Position;
    vec3 Color;
};

uniform vec3 viewPos;
uniform Light lights[16];
uniform sampler2D diffuseTexture;

void main() {
    vec3 color = texture(diffuseTexture, fs_in.TexCoords).rgb;

    vec3 lighting = vec3(0.0);
    for (int i = 0; i < 16; ++i) {
        vec3 lightDir = normalize(lights[i].Position - fs_in.FragPos);
        float diff = max(dot(lightDir, fs_in.Normal), 0.0);
        vec3 diffuse = lights[i].Color * diff * color;
        // attenuation
        float fragDistance = length(fs_in.FragPos - lights[i].Position);
        diffuse *= 1.0 / (fragDistance * fragDistance);
        lighting += diffuse;
    }

    FragPos = vec4(lighting, 1.0);
}
