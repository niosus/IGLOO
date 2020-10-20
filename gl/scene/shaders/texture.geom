#version 330
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

uniform vec2 rect_size;

out vec2 tex_coord;

out vec4 vertexColor;  // Linked to the variable in vertex shader.

void rectangle(vec4 position) {
    gl_Position = position + vec4(rect_size.x, 0.0, 0.0, 0.0);
    tex_coord = vec2(1.0, 0.0);
    EmitVertex();
    gl_Position = position + vec4(rect_size.x, rect_size.y, 0.0, 0.0);
    tex_coord = vec2(1.0, 1.0);
    EmitVertex();
    gl_Position = position + vec4(0.0, 0.0, 0.0, 0.0);
    tex_coord = vec2(0.0, 0.0);
    EmitVertex();
    gl_Position = position + vec4(0.0, rect_size.y, 0.0, 0.0);
    tex_coord = vec2(0.0, 1.0);
    EmitVertex();
    EndPrimitive();
}

void main() {
    rectangle(gl_in[0].gl_Position);
}
