#version 330

layout (location = 0) out vec4 fragColor;

in vec4 pt_color;

void main() {
    fragColor = pt_color;
}
