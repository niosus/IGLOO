#version 330 core
out vec4 FragColor;

uniform vec4 ourColor;  // Set in OpenGL code.

void main()
{
  FragColor = ourColor;
}
