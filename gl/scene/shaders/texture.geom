#version 330
layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

uniform vec2 rect_size;
uniform mat4 proj_view;
uniform mat4 model;

out vec2 tex_coord;

void rectangle(vec4 position) {
    mat4 pvm = proj_view * model;
    gl_Position = pvm * (position + vec4(rect_size.x, 0.0, 0.0, 0.0));
    tex_coord = vec2(1.0, 0.0);
    EmitVertex();
    gl_Position = pvm * (position + vec4(rect_size.x, rect_size.y, 0.0, 0.0));
    tex_coord = vec2(1.0, 1.0);
    EmitVertex();
    gl_Position = pvm * (position + vec4(0.0, 0.0, 0.0, 0.0));
    tex_coord = vec2(0.0, 0.0);
    EmitVertex();
    gl_Position = pvm * (position + vec4(0.0, rect_size.y, 0.0, 0.0));
    tex_coord = vec2(0.0, 1.0);
    EmitVertex();
    EndPrimitive();
}

void main() {
    rectangle(gl_in[0].gl_Position);
}
