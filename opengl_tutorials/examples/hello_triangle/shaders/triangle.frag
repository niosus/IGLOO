#version 330 core
out vec4 FragColor;

in vec4 vertexColor; // Linked to the variable in vertex shader.

void main()
{
  FragColor = vertexColor;
}
