#version 410

layout(location = 0) in vec4 Position;
layout(location = 1) in vec4 Normal;
layout(location = 2) in vec4 Tangent;
layout(location = 3) in vec2 TexCoord;

//view-space normal and position
out vec4 vPosition;
out vec4 vNormal;
out vec2 vTexCoord;
out mat4 vTBN;

uniform mat4 Projection;
uniform mat4 View;
uniform mat4 Model;

uniform sampler2D NormalMap;

void main()
{
	vPosition = View * Model * Position;
	vNormal = normalize(View * Model * Normal);
vec4 biNormal = vec4(normalize(cross(Normal.xyz, Tangent.xyz)), 0);
		vTBN = mat4(Tangent, biNormal, Normal, vec4(0, 0, 0, 1));
		vTBN = View * Model * vTBN;

	vTexCoord = TexCoord;

	gl_Position = Projection * View * Model * Position;
}