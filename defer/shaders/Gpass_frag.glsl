#version 410

in vec4 vPosition;
in vec4 vNormal;
in vec2 vTexCoord;

layout(location = 0) out vec3 gpassAlbedo;
layout(location = 1) out vec3 gpassPosition;
layout(location = 2) out vec3 gpassNormal;



uniform sampler2D Diffuse;

void main() 
{
	gpassAlbedo = texture(Diffuse, vTexCoord).xyz;
	gpassPosition = vPosition.xyz;
	gpassNormal = vNormal.xyz;
}
