#version 330 core
layout (triangles) in;
// layout (points, max_vertices = 1) out;
// layout (triangle_strip, max_vertices = 3) out;
layout (line_strip, max_vertices = 6) out;

in VS_OUT {
    // vec3 color;

    // vec2 texCoords;

    vec4 normal;
} gs_in[];
// out vec3 fColor;
// out vec2 TexCoords;

// uniform float curTime;

// void main() {
//     gl_Position = gl_in[0].gl_Position;
//     gl_PointSize = 100.0;
//     EmitVertex();
//     EndPrimitive();
// }

// void buildHouse(vec4 position) {
//     fColor = gs_in[0].color;
//     gl_Position = position + vec4(-0.2, -0.2, 0.0, 0.0);
//     EmitVertex();
//     gl_Position = position + vec4(0.2, -0.2, 0.0, 0.0);
//     EmitVertex();
//     gl_Position = position + vec4(-0.2, 0.2, 0.0, 0.0);
//     EmitVertex();
//     gl_Position = position + vec4(0.2, 0.2, 0.0, 0.0);
//     EmitVertex();
//     gl_Position = position + vec4(0.0, 0.4, 0.0, 0.0);
//     fColor = vec3(1.0, 1.0, 1.0);
//     EmitVertex();
//     EndPrimitive();
// }

// void main() {
//     buildHouse(gl_in[0].gl_Position);
// }

// explode
// vec3 getNormal() {
//     vec3 a = vec3(gl_in[0].gl_Position) - vec3(gl_in[1].gl_Position);
//     vec3 b = vec3(gl_in[2].gl_Position) - vec3(gl_in[1].gl_Position);

//     return normalize(cross(a, b));
// }

// vec4 explode(vec4 position, vec3 normal) {
//     float magnitude = 2.0;
//     vec3 direction = normal * ((sin(curTime) + 1.0) / 2.0) * magnitude;

//     return position + vec4(direction, 0.0);
// }

// void main() {
//     vec3 normal = getNormal();

//     gl_Position = explode(gl_in[0].gl_Position, normal);
//     TexCoords = gs_in[0].texCoords;
//     EmitVertex();
//     gl_Position = explode(gl_in[1].gl_Position, normal);
//     TexCoords = gs_in[1].texCoords;
//     EmitVertex();
//     gl_Position = explode(gl_in[2].gl_Position, normal);
//     TexCoords = gs_in[2].texCoords;
//     EmitVertex();
//     EndPrimitive();
// }

// visualize normal
void generateLine(int index) {
    float magnitude = 0.1;

    gl_Position = gl_in[index].gl_Position;
    EmitVertex();
    gl_Position = gl_in[index].gl_Position + gs_in[index].normal * magnitude;
    EmitVertex();
    EndPrimitive();
}

void main() {
    generateLine(0);
    generateLine(1);
    generateLine(2);
}
