#version 410

layout(location = 0) in vec4 Position;
layout(location = 1) in vec4 Color;

out vec4 vColor;

uniform mat4 View;
uniform mat4 Projection;

void main()
{
	vColor = Color;
	gl_Position = Projection * View * Position;
}