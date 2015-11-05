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
uniform mat4 CameraView;
uniform float specPower;

uniform vec3 ambient;

uniform sampler2D positionTexture;//view space
uniform sampler2D normalTexture;

void main()
{
	vec3 normal = normalize(texture(normalTexture, vTexCoord).xyz);
	vec3 position = texture(positionTexture, vTexCoord).xyz;//view space

	//compute diffuse lighting
	vec3 lightDirection = directional.Direction;
	float d = max(dot(normal, lightDirection), 0); //lambertian term

	//compute specular lighting
	vec3 CamViewPosition = (CameraView * vec4(CameraPosition, 1)).xyz;
	vec3 E = normalize(CamViewPosition - position); //Eye vector
	vec3 R = reflect(-lightDirection, normal);//reflection vector
	float s = pow(max(dot(E, R), 0), specPower);//specular

	LightOutput = directional.Color * d + directional.Color * s;

}