#version 410

in vec4 vPosition;//view space
in vec4 vNormal;
in vec2 vTexCoord;

layout(location = 0) out vec3 gpassAlbedo;
layout(location = 1) out vec3 gpassPosition;
layout(location = 2) out vec3 gpassNormal;


uniform bool IsTexture = true;
uniform bool IsParticle = false;
uniform sampler2D Diffuse;
uniform vec4 ParticleColor = vec4(0);

void main() 
{
	if (IsTexture)
	{
		gpassAlbedo = texture(Diffuse, vTexCoord).xyz;
	}
	else if (IsParticle)
	{
		gpassAlbedo = ParticleColor.xyz;
	}
	else
	{
		gpassAlbedo = vec3(.25f,.25f,.25f);
	}
	
	gpassPosition = vPosition.xyz;//view space
	gpassNormal = vNormal.xyz;
}
