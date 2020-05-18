#version 330 core
uniform float dummy_value_dim_1;
uniform vec2 dummy_value_dim_2;
uniform vec3 dummy_value_dim_3;
uniform vec4 dummy_value_dim_4;

uniform int i_dummy_value_dim_1;
uniform ivec2 i_dummy_value_dim_2;
uniform ivec3 i_dummy_value_dim_3;
uniform ivec4 i_dummy_value_dim_4;

uniform vec2 vec_of_vec_2[2];
uniform vec3 vec_of_vec_3[2];

uniform mat2 matrix_2;
uniform mat3 matrix_3;
uniform mat2x3 matrix_2x3;

// This is a hack to make sure all the uniforms are compiled into the shader.
uniform int pick;

void main()
{
  if (pick == 0) {
    gl_Position = vec4(1.0, 1.0, 1.0, dummy_value_dim_1);
  }
  if (pick == 1) {
    gl_Position = vec4(1.0, 1.0, dummy_value_dim_2);
  }
  if (pick == 2) {
    gl_Position = vec4(1.0, dummy_value_dim_3);
  }
  if (pick == 3) {
    gl_Position = dummy_value_dim_4;
  }
  if (pick == 4) {
    gl_Position = vec4(1.0, 1.0, 1.0, i_dummy_value_dim_1);
  }
  if (pick == 5) {
    gl_Position = vec4(1.0, 1.0, i_dummy_value_dim_2);
  }
  if (pick == 6) {
    gl_Position = vec4(1.0, i_dummy_value_dim_3);
  }
  if (pick == 7) {
    gl_Position = i_dummy_value_dim_4;
  }
  if (pick == 8) {
    gl_Position = vec4(vec_of_vec_2[0], vec_of_vec_2[1]);
  }
  if (pick == 9) {
    gl_Position = vec4(vec_of_vec_3[0].xy, vec_of_vec_3[1].xy);
  }
  if (pick == 10) {
    gl_Position = vec4(matrix_2[0], matrix_2[1]);
  }
  if (pick == 11) {
    gl_Position = vec4(matrix_3[0].xy, matrix_3[1].xy);
  }
  if (pick == 12) {
    gl_Position = vec4(matrix_2x3[0], matrix_2x3[1]);
  }
}
