#version 410

//in vec4 vPosition;//MVP coords
in vec4 vPosition;//world space
in vec4 vNormal;
in vec2 vTexCoord;

layout(location = 0) out vec3 gpassAlbedo;
layout(location = 1) out vec3 gpassPosition;
layout(location = 2) out vec3 gpassNormal;


uniform bool isTexture = true;
uniform sampler2D Diffuse;

void main() 
{
	if (isTexture)
	{
		gpassAlbedo = texture(Diffuse, vTexCoord).xyz;
	}
	else
	{
		gpassAlbedo = vec3(.25f,.25f,.25f);
	}
	
	gpassPosition = vPosition.xyz;//MVP coords
	gpassNormal = vNormal.xyz;
}
