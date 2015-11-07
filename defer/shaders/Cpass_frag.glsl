#version 410

in vec2 vTexCoord;

out vec4 FragColor;

uniform sampler2D Albedo;
uniform sampler2D Light;

uniform bool OutputDebugTexture = false;
uniform sampler2D DebugTexture;

void main() 
{
	vec3 light = texture(Light, vTexCoord).rgb;
	vec3 albedo = texture(Albedo, vTexCoord).rgb;
	
	if (OutputDebugTexture)
	{
		FragColor = texture(DebugTexture, vTexCoord);
	}
	else
	{
		FragColor = vec4(albedo * light, 1);
	}
	
}