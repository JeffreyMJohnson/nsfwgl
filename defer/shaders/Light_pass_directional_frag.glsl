#version 410

in vec2 vTexCoord;

out vec3 LightOutput;

//direction in view-space
uniform vec3 LightDirection = vec3(0,1,0);
uniform vec3 LightColor = vec3(1,1,0);
//uniform mat4 TexelScalar;

//uniform vec3 cameraPosition = vec3(10,10,10);
//uniform float specPower = 128;

uniform sampler2D positionTexture;
uniform sampler2D normalTexture;


void main() 
{
	vec3 normal = normalize(texture(normalTexture, vTexCoord).xyz);
	vec3 position = texture(positionTexture, vTexCoord).xyz;

	float diffuse = max(0, dot( normal, -LightDirection)); 

	LightOutput = LightColor * diffuse;


	//vec3 eye = normalize(cameraPosition - vPosition.xyz);
	//vec3 reflect = reflect(-lightDirection, vNormal.xyz);
	//float specular = max(0, dot(eye,reflect));
	//specular = pow(specular, specPower);
	//FragColor = vec4(diffuse * lightColor + lightColor * specular, 1);
}