#version 460 core

layout(location = 0) in vec2 inPos;
layout(location = 1) in vec3 inCol;
layout(location = 2) in vec2 texCoord;

out vec4 fColor;
out vec2 fTexCoord;

void main()
{
	gl_Position = vec4(inPos, 0.0, 1.0);
	fColor = vec4(inCol, 1.0);
	fTexCoord = texCoord;
}
