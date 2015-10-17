#version 410

in vec2 vTexCoord;

out vec4 FragColor;

uniform sampler2D Albedo;
uniform sampler2D Light;

void main() 
{
	vec3 light = texture(Light, vTexCoord).rgb;
	vec3 albedo = texture(Albedo, vTexCoord).rgb;
	
	FragColor = vec4(albedo * light, 1);
}