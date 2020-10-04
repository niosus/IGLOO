#version 330
layout (location = 0) out vec4 result_color;

in vec2 tex_coord;

uniform sampler2D source;

void main() {
    result_color = texture(source, tex_coord);
}
