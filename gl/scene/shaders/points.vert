#version 330

layout (location = 0) in vec3 point;
layout (location = 1) in float intensity;

uniform mat4 proj_view;
uniform mat4 model;
uniform vec3 color;

out vec4 pt_color;

void main() {
    gl_Position = proj_view * model * vec4(point, 1.0);
    pt_color = vec4(intensity * color.xyz, 1.0);
}
