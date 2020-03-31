#version 430

layout (points) in;
layout (line_strip, max_vertices = 6) out;

uniform mat4 proj_view;
uniform mat4 model;

out vec4 pt_color;

mat4 MVP = proj_view * model;

void emitLine(vec4 position, vec3 direction) {
  gl_Position = MVP * position;
  pt_color = vec4(direction, 1);
  EmitVertex();
  gl_Position = MVP * (position + vec4(direction, 0));
  pt_color = vec4(direction, 1);
  EmitVertex();
  EndPrimitive();
}

void emitCoordinateSystem(vec4 position) {
  emitLine(position, vec3(1, 0, 0));
  emitLine(position, vec3(0, 1, 0));
  emitLine(position, vec3(0, 0, 1));
}

void main() {
  emitCoordinateSystem(gl_in[0].gl_Position);
}
