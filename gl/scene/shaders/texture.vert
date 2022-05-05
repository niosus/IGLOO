#version 330
layout (location = 0) in vec3 bottom_left;

void main() {
    gl_Position = vec4(bottom_left, 1.0);
}
