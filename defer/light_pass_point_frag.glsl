#version 410

in vec2 vTexCoord;

out vec3 LightOutput;

struct Light
{
	vec4 position;
	vec3 normal;

	vec3 color;
	vec3 direction;

	struct Attenuation
	{
		float kC;//constant
		float kL;//linear
		float kQ;//quadratic
	};
	Attenuation attenuation;
};

float AttenuationFactor(vec3 cameraPosition, Light light)
{
	float distance = distance(CameraPosition, position);
	return 1 / (light.attenuation.kC + light.attenuation.kL * d + light.attenuation.kQ * d * d);
}

//direction in view-space
uniform Light directional;

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

	
	LightOutput = ambient + AttenuationFactor(CameraPosition, directional) * ((directional.color * diffuse) + (directional.color * specular));

}