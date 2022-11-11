#version 330 core

in vec4 fColor;
out vec4 FragColor;

uniform float k;

void main()
{
	FragColor = k * fColor;
}
