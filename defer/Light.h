#pragma once
#include <glm/glm.hpp>

struct LightD
{
	glm::vec3 color = vec3(1);
	float ambientIntensity = 1;
	glm::vec3 direction = vec3(0,1,0);
	float diffuseIntensity = 1;
	void update() {}
};

struct LightP
{
	glm::vec4 position;
	glm::vec3 normal;

	glm::vec3 color;
	glm::vec3 direction;

	struct Attenuation
	{
		float kC;//constant
		float kL;//linear
		float kQ;//quadratic
	};
	Attenuation attenuation;
};