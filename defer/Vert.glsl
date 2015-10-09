#version 330

layout(location=0) in vec4 Position;
layout(location=1) in vec4 Normal;
layout(location=2) in vec4 Tangent;
layout(location=3) in vec2 TexCoord;

uniform mat4 ProjectionView;

void main() 
{
	gl_Position= ProjectionView * Position;
}