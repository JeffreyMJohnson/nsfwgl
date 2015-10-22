#version 410

in vec4 vPosition;
in vec4 vNormal;
in vec2 vTexCoord;

layout(location = 0) out vec3 gpassAlbedo;
layout(location = 1) out vec3 gpassPosition;
layout(location = 2) out vec3 gpassNormal;



uniform sampler2D Diffuse;
//uniform sampler2D NormalMap;
//uniform sampler2D Specular;

//uniform float SpecularPower;

void main() 
{
	//float diffuse = max(0, dot( normalize(vNormal.xyz), lightDirection ) ); 
	//vec3 eye = normalize(cameraPosition - vPosition.xyz);
	//vec3 reflect = reflect(-lightDirection, vNormal.xyz);
	//float specular = max(0, dot(eye,reflect));
	//specular = pow(specular, specPower);
	//float d = max(0, dot( normalize(vNormal.xyz), vec3(0,1,0)));

	gpassAlbedo = texture(Diffuse, vTexCoord).xyz;
	gpassPosition = vPosition.xyz;
	gpassNormal = vNormal.xyz;
	//gpassNormal = texture(NormalMap, vTexCoord).xyz;
}
