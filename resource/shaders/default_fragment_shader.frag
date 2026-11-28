#version 330 core
layout(location = 0) out vec4 FragColor;
uniform vec4 inputColor;

void main()
{
    FragColor = inputColor;
}