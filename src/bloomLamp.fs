#version 330 core

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

uniform vec3 lightColor;

void main() {
    FragColor = vec4(lightColor, 1.0);
    // float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    float brightness = dot(FragColor.rgb, vec3(0.6666));
    if (brightness > 0.7) {
        BrightColor = vec4(FragColor.rgb, 1.0);
    } else {
        BrightColor = vec4(0.5, 0.0, 0.5, 1.0);
    }
}
