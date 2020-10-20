#version 330
layout (location = 0) in vec3 bottom_left;

uniform mat4 model;
uniform mat4 proj_view;

void main() {
  gl_Position = proj_view * model * vec4(bottom_left, 1.0);
}
