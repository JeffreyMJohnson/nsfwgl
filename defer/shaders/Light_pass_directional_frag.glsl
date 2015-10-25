#version 410

in vec2 vTexCoord;

out vec3 LightOutput;

struct DirectionalLight
{
	vec3 color;
	float ambientIntensity;
	vec3 direction;
	float diffuseIntensity;
};

//direction in view-space
uniform DirectionalLight directional;

uniform vec3 CameraPosition;
uniform float specPower;

uniform vec3 ambient;

uniform sampler2D positionTexture;
uniform sampler2D normalTexture;

void main()
{
	vec3 normal = normalize(texture(normalTexture, vTexCoord).xyz);
	vec3 position = texture(positionTexture, vTexCoord).xyz;

	float diffuse = max(0, dot(normal, -directional.direction));
	vec3 eye = normalize(position.xyz - CameraPosition);
	vec3 reflect = reflect(-directional.direction, normal.xyz);
	float specular = max(0, dot(eye, reflect));
	specular = pow(specular, specPower);

	LightOutput = ambient + (directional.color * diffuse) + (directional.color * specular);

}