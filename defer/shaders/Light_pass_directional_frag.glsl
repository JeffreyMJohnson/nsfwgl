#version 410

in vec2 vTexCoord;

out vec3 LightOutput;

struct DirectionalLight
{
	vec3 Color;
	vec3 Direction;
};

//direction in view-space
uniform DirectionalLight directional;

uniform vec3 CameraPosition;
uniform float specPower;

uniform vec3 ambient;

uniform sampler2D positionTexture;//view space
uniform sampler2D normalTexture;

//uniform mat4 LightMatrix;//light View Projection for shadows
//uniform sampler2D ShadowMap;

void main()
{
	vec3 normal = normalize(texture(normalTexture, vTexCoord).xyz);
	vec3 position = texture(positionTexture, vTexCoord).xyz;//view space

	//not in correct space, need to multiply by inverse view
	//vec3 shadowCoord = (LightMatrix * vec4(position, 1)).xyz;


	//compute diffuse lighting
	vec3 lightDirection = directional.Direction;
	float diffuseLight = max(dot(normal, lightDirection), 0);
	//if (texture(ShadowMap, shadowCoord.xy).r < shadowCoord.z)
	//{
	//	diffuseLight = 0;
	//}
	vec3 diffuseResult = directional.Color * diffuseLight;

	//compute specular lighting
	vec3 viewPointDirection = normalize(CameraPosition - position);
	vec3 halfVector = normalize(lightDirection + viewPointDirection);
	float specularLight = pow(max(dot(normal, halfVector), 0), specPower);
	if (diffuseLight <= 0)
	{
		specularLight = 0;
	}
	vec3 specularResult = directional.Color * specularLight;

	//think this should be specularResult?
	LightOutput = ambient + (diffuseLight + specularLight);

}