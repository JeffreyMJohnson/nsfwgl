#version 410

layout(location = 0) in vec4 Position;
layout(location = 1) in vec4 Normal;
layout(location = 2) in vec4 Tangent;
layout(location = 3) in vec2 TexCoord;

uniform mat4 LightMatrix;
uniform mat4 Model;

void main()
{
	gl_Position = LightMatrix * Model * Position;
}