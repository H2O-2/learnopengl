#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D hdrBuffer;
uniform float exposure;
uniform bool hdr;

void main() {
    vec3 hdrColor = texture(hdrBuffer, TexCoords).rgb;

    if (hdr) {
        const float gamma = 2.2;
        // Reinhard tone mapping
        // vec3 mapped = hdrColor / (hdrColor + vec3(1.0));

        // exposure tone mapping
        vec3 mapped = vec3(1.0) - exp(-hdrColor * exposure);
        mapped = pow(mapped, vec3(1.0 / gamma));

        FragColor = vec4(mapped, 1.0);
    } else {
        FragColor = vec4(hdrColor, 1.0);
    }
}
