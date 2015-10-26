#version 410

layout(location=0) in vec4 Position;
layout(location=1) in vec4 Normal;
layout(location=2) in vec4 Tangent;
layout(location=3) in vec2 TexCoord;

//view-space normal and position
out vec4 vPosition;
out vec4 vNormal;
out vec2 vTexCoord;

uniform mat4 Projection;
uniform mat4 View;
uniform mat4 Model;

//HACKHACK
uniform bool isObj = false;

void main() 
{
	
	//HACKHACK
	if (isObj)
	{
		vNormal = Normal;
		vPosition = Position;
	}
	else
	{
		vNormal = normalize(Model * View * Normal);
		vPosition = Model * View * Position;
	}
	vTexCoord = TexCoord;
	gl_Position = Model * Projection * View * Position;
}