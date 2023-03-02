#version 460 core

in vec4 fColor;
in vec2 fTexCoord;
out vec4 FragColor;

layout(binding = 0) uniform sampler2D mainTexture;
uniform float k;

void main()
{
	FragColor = texture(mainTexture, fTexCoord) * (k * fColor);
}
