#pragma once

#include <glm\glm.hpp>
#include "../nsfwgl/Assets.h"
#include "Keyboard.h"

using namespace nsfw::ASSET;

struct Geometry
{
	glm::mat4 transform;

	nsfw::Asset<VAO> mesh;
	nsfw::Asset<nsfw::ASSET::SIZE> tris;

	nsfw::Asset<TEXTURE> diffuse;
	nsfw::Asset<TEXTURE> normal;
	nsfw::Asset<TEXTURE> specular;

	float specPower;

	void update() 
	{
		if (Keyboard::IsKeyPressed(Keyboard::KEY_RIGHT) || Keyboard::IsKeyRepeat(Keyboard::KEY_RIGHT))
		{
			transform = glm::rotate(transform, -1.0f, glm::vec3(0,0,1));
		}
		if (Keyboard::IsKeyPressed(Keyboard::KEY_LEFT) || Keyboard::IsKeyRepeat(Keyboard::KEY_LEFT))
		{
			transform = glm::rotate(transform, 1.0f, glm::vec3(0, 0, 1));
		}
	}
};