#version 410

in vec2 vTexCoord;

out vec3 LightOutput;

struct Attenuation
{
	float kC;//constant
	float kL;//linear
	float kQ;//quadratic
};
struct Light
{
	vec3 Position;
	vec3 Color;
	Attenuation attenuation;
};

vec3 GetLightDirection(vec3 lightPosition, vec3 vertexPosition)
{
	return normalize(lightPosition - vertexPosition);
}

float AttenuationFactor(vec3 cameraPosition, Light light)
{
	float distance = distance(cameraPosition, light.Position);
	return 1 / (light.attenuation.kC + light.attenuation.kL * distance + light.attenuation.kQ * distance * distance);
}

//direction in view-space
uniform Light pointLight;

uniform vec3 CameraPosition;
uniform mat4 View;
uniform float specPower;

uniform vec3 ambient;

uniform sampler2D positionTexture;
uniform sampler2D normalTexture;

void main()
{
	vec3 normal = normalize(texture(normalTexture, vTexCoord).xyz);
	vec3 position = texture(positionTexture, vTexCoord).xyz;
	vec3 lightViewPosition = (View * vec4(pointLight.Position, 1)).xyz;

	//compute diffuse lighting
	vec3 L = normalize(lightViewPosition - position);
	float diffuseLight = max(dot(normal, L), 0);
	vec3 diffuseResult = pointLight.Color * diffuseLight;

	//compute specular lighting
	vec3 V = normalize(CameraPosition - position);
	vec3 H = normalize(L + V);
	float specularLight = pow(max(dot(normal, H), 0), specPower);
	if (diffuseLight <= 0)
	{
		specularLight = 0;
	}
	vec3 specularResult = pointLight.Color * specularLight;

	//calc attenuation
	float distance = distance(position, lightViewPosition);
	float attFactor = 1.0f / (distance * distance);
	LightOutput = ambient + attFactor * (diffuseLight + specularLight);
}