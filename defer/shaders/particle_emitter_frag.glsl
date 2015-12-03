#version 410

in vec4 Color;

out vec4 FragColor;

uniform vec3 ParticleColor = vec3(1);

void main() 
{
	FragColor = ParticleColor;
}
