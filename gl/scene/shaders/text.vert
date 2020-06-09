#version 430
layout (location = 0) in vec2 char_pos;
layout (location = 1) in vec2 texture_pos;

uniform mat4 proj_view;
uniform mat4 model;
uniform vec3 anchor;

out vec2 tex_coord;

void main() {
  gl_Position = proj_view * model * vec4(anchor, 1) + vec4(char_pos, 0, 0);
  tex_coord = texture_pos;
}
