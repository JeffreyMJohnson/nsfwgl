#version 410

in vec2 vTexCoord;

out vec3 LightOutput;

//direction in view-space
uniform vec3 LightDirection;
uniform vec3 LightColor;
uniform vec3 CameraPosition;
uniform float specPower;
uniform vec3 ambient;

uniform sampler2D positionTexture;
uniform sampler2D normalTexture;

void main()
{
	vec3 normal = normalize(texture(normalTexture, vTexCoord).xyz);
	vec3 position = texture(positionTexture, vTexCoord).xyz;

	float diffuse = max(0, dot(normal, LightDirection));
	vec3 eye = normalize(CameraPosition - position.xyz);
	vec3 reflect = reflect(-LightDirection, normal.xyz);
	float specular = max(0, dot(eye, reflect));
	specular = pow(specular, specPower);

	LightOutput = vec3(ambient + LightColor * diffuse + LightColor * specular);
}