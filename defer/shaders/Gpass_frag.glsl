#version 410

in vec4 vPosition;//view space
in vec4 vNormal;
in vec2 vTexCoord;
in vec4 vTangent;
in vec4 vBiTangent;
//in mat4 vTBN;

layout(location = 0) out vec3 gpassAlbedo;
layout(location = 1) out vec3 gpassPosition;
layout(location = 2) out vec3 gpassNormal;


uniform bool IsTexture = false;//debug this needs to be true
uniform sampler2D Diffuse;
uniform sampler2D NormalMap;

uniform vec3 Color = vec3(.25f,.25f,.25f);
uniform bool useNormalMap;

void main() 
{
	if (IsTexture)
	{
		gpassAlbedo = texture(Diffuse, vTexCoord).xyz;
	}
	else
	{
		gpassAlbedo = Color;
	}
	gpassPosition = vPosition.xyz;//view space
	if (useNormalMap)
	{
		mat3 TBN = mat3(\
			normalize(vTangent.xyz), \
			normalize(vBiTangent.xyz), \
			normalize(vNormal.xyz));
		vec4 normalSample = vec4(normalize(texture(NormalMap, vTexCoord).xyz * 2 - 1), 0);
		gpassNormal = (TBN * normalSample.xyz).xyz;
	}
	else
	{
		gpassNormal = vNormal.xyz;
	}
}
