#version 430

layout (location = 0) in vec4 point;

void main() {
    gl_Position = point;
}
