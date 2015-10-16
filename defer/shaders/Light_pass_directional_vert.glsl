#version 410

layout(location=0) in vec4 Position;
layout(location=1) in vec4 Color;
layout(location=2) in vec4 Normal;
layout(location=3) in vec2 TexCoord;

out vec4 vPosition;
out vec4 vNormal;

uniform mat4 Projection;
uniform mat4 View;

void main() 
{
	vPosition = Position;
	vNormal = Normal;
	gl_Position= Projection * View * Position;
}