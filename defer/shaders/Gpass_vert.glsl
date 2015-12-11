#version 410

layout(location = 0) in vec4 Position;
layout(location = 1) in vec4 Normal;
layout(location = 2) in vec4 Tangent;
layout(location = 3) in vec2 TexCoord;

//view-space normal and position
out vec4 vPosition;
out vec4 vNormal;
out vec2 vTexCoord;
out vec4 vTangent;
out vec4 vBiTangent;
//out mat4 vTBN;

uniform mat4 Projection;
uniform mat4 View;
uniform mat4 Model;

uniform sampler2D NormalMap;

void main()
{
	vPosition = View * Model * Position;
	vNormal = normalize(View * Model * Normal);
	vTangent = normalize(View * Model * Tangent);
	vBiTangent = vec4(cross(vNormal.xyz, vTangent.xyz),0);

	vTexCoord = TexCoord;

	gl_Position = Projection * View * Model * Position;
}