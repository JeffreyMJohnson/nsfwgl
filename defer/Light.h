#pragma once
#include <glm/glm.hpp>

struct LightD
{
	glm::vec3 color = vec3(1);
	float ambientIntensity = 1;
	glm::vec3 direction = vec3(0, 1, 0);
	float diffuseIntensity = 1;
	void update() {}
};

struct LightP
{
	glm::vec4 position = glm::vec4(0, 0, 0, 1);
	glm::vec3 color = glm::vec3(1);

	struct Attenuation
	{
		float kC = 1;//constant
		float kL = 1;//linear
		float kQ = 1;//quadratic
	};
	Attenuation attenuation;
};