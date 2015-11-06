#pragma once
#include "../nsfwgl/nsfw.h"
#include "Light.h"
#include "Camera.h"

class LPassD : public nsfw::RenderPass
{
	nsfw::Asset<nsfw::ASSET::TEXTURE> position, normal, shadowMap;
	glm::vec3 ambientLight = vec3(0, 0, .2f);
	float specPower = 128;

	//glm::mat4 lightMatrix;

public:
	LPassD(const char *shaderName, const char *fboName) : RenderPass(shaderName, fboName), position("GPassPosition"), normal("GPassNormal"), shadowMap("ShadowMap") {}

	void prep()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, *fbo);
		glClear(GL_COLOR_BUFFER_BIT);

		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);

		glUseProgram(*shader);
	}
	void post()
	{
		glDisable(GL_BLEND);
		glUseProgram(0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}


	void draw(Camera &c, const LightD &l)
	{
		glm::mat4 textureSpaceOffset(
			0.5f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.5f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.5f, 0.0f,
			0.5f, 0.5f, 0.5f, 1.0f
			);

		//set frag shader uniforms
		//set the light properties
		setUniform("directional.Direction", nsfw::UNIFORM::TYPE::FLO3, glm::value_ptr(l.direction));
		setUniform("directional.Color", nsfw::UNIFORM::TYPE::FLO3, glm::value_ptr(l.color));
		setUniform("directional.projection", nsfw::UNIFORM::MAT4, glm::value_ptr(l.projection));
		setUniform("directional.view", nsfw::UNIFORM::MAT4, glm::value_ptr(l.view));
		setUniform("TextureSpaceOffset", nsfw::UNIFORM::MAT4, glm::value_ptr(textureSpaceOffset));


		setUniform("CameraPosition", nsfw::UNIFORM::TYPE::FLO3, glm::value_ptr(c.GetPosition()));
		setUniform("CameraView", nsfw::UNIFORM::MAT4, glm::value_ptr(c.GetView()));
		setUniform("specPower", nsfw::UNIFORM::TYPE::FLO1, &specPower);
		setUniform("ambient", nsfw::UNIFORM::TYPE::FLO3, glm::value_ptr(ambientLight));
		setUniform("positionTexture", nsfw::UNIFORM::TEX2, position, 0);

		setUniform("normalTexture", nsfw::UNIFORM::TEX2, normal, 1);
		setUniform("ShadowMap", nsfw::UNIFORM::TEX2, shadowMap, 2);


		unsigned quadVAOHandle = nsfw::Assets::instance().get<nsfw::ASSET::VAO>("Quad");
		unsigned quadNumtris = nsfw::Assets::instance().get<nsfw::ASSET::SIZE>("Quad");

		glBindVertexArray(quadVAOHandle);
		glDrawElements(GL_TRIANGLES, quadNumtris, GL_UNSIGNED_INT, 0);
	}
};