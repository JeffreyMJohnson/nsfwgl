#version 410

in vec2 vTexCoord;

out vec3 LightOutput;

//direction in view-space
uniform vec3 LightDirection = vec3(0, 0, 1);
uniform vec3 LightColor = vec3(1, 1, 0);
uniform vec3 CameraPosition = vec3(0, 0, 10);
uniform float specPower = 128;

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

	vec3 ambient = vec3(0, 0, .1f);

	LightOutput = vec3(ambient + LightColor * diffuse + LightColor * specular);
}