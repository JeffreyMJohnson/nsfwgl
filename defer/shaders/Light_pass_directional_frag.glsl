#version 410

in vec4 vPosition;
in vec4 vNormal;

out vec4 FragColor;

uniform vec3 LightDirection = vec3(0,1,0);
uniform vec3 LightColor = vec3(1,1,0);
uniform mat4 TexelScalar;

uniform vec3 cameraPosition = vec3(10,10,10);
uniform float specPower = 128;


void main() 
{
	float diffuse = max(0, dot( normalize(vNormal.xyz), lightDirection ) ); 
	vec3 eye = normalize(cameraPosition - vPosition.xyz);
	vec3 reflect = reflect(-lightDirection, vNormal.xyz);
	float specular = max(0, dot(eye,reflect));
	specular = pow(specular, specPower);
	FragColor = vec4(diffuse * lightColor + lightColor * specular, 1);
}