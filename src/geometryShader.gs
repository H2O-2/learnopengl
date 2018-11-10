#version 330 core
layout (points) in;
layout (points, max_vertices = 1) out;

void main() {
    gl_Position = gl_in[0].gl_Position;
    gl_PointSize = 100.0;
    EmitVertex();
    EndPrimitive();
}
