#version 410

in vec2 vTexCoord;

out vec4 FragColor;

uniform sampler2D Albedo;
uniform sampler2D Light;

uniform sampler2D Position;
uniform sampler2D Normal;
uniform sampler2D Depth;

void main() 
{
	vec3 light = texture(Light, vTexCoord).rgb;
	vec3 albedo = texture(Albedo, vTexCoord).rgb;


	vec3 position = texture(Position, vTexCoord).rgb;
	vec3 normal = texture(Normal, vTexCoord).rgb;
	vec3 depth = texture(Depth, vTexCoord).rgb;
	
	FragColor = vec4(albedo * light, 1);
}