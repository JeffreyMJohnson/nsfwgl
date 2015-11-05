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

//direction in view-space
uniform Light pointLight;

uniform vec3 CameraPosition;
uniform mat4 View;
uniform float specPower;

uniform vec3 ambient;
uniform sampler2D positionTexture;//view space
uniform sampler2D normalTexture;

void main()
{
	vec3 normal = normalize(texture(normalTexture, vTexCoord).xyz);
	vec3 position = texture(positionTexture, vTexCoord).xyz;//view space
	vec3 lightViewPosition = (View * vec4(pointLight.Position, 1)).xyz;

	//compute diffuse lighting
	vec3 lightDirection = normalize(lightViewPosition - position);
	float d = max(dot(normal, lightDirection), 0); //lambertian term
	
	//compute specular lighting
	vec3 CamViewPosition = (View * vec4(CameraPosition, 1)).xyz;
	vec3 E = normalize(CamViewPosition - position); //Eye vector
	vec3 R = reflect(-lightDirection, normal);//reflection vector
	float s = pow(max(dot(E, R), 0), specPower);//specular

	//calc attenuation
	float distance = distance(position, lightViewPosition);
	float attFactor = 1.0f / (distance * distance);

	LightOutput = attFactor * ((pointLight.Color * d) + (pointLight.Color * s));
}