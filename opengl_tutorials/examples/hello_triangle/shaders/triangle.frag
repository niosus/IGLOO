#version 330 core
out vec4 FragColor;

in vec3 vertexColor;  // Linked to the variable in vertex shader.

void main()
{
  FragColor = vec4(vertexColor, 1.0);
}
