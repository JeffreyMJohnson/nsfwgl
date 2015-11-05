#pragma once
#include "../nsfwgl/nsfw.h"
#include "Light.h"
#include "Camera.h"

class LPassD : public nsfw::RenderPass
{
	nsfw::Asset<nsfw::ASSET::TEXTURE> position, normal;
	glm::vec3 ambientLight = vec3(0, 0, .2f);
	float specPower = 40;

	//glm::mat4 lightMatrix;

public:
	LPassD(const char *shaderName, const char *fboName) : RenderPass(shaderName, fboName), position("GPassPosition"), normal("GPassNormal") {}

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
		//glm::mat4 lightProjection = glm::ortho<float>(-10, 10, -10, 10, -10, 10);
		//glm::mat4 lightView = glm::lookAt(l.direction, glm::vec3(0), glm::vec3(0, 1, 0));
		//lightMatrix = lightProjection * lightView;

		//glm::mat4 textureSpaceOffset(
		//	0.5f, 0.0f, 0.0f, 0.0f,
		//	0.0f, 0.5f, 0.0f, 0.0f,
		//	0.0f, 0.0f, 0.5f, 0.0f,
		//	0.5f, 0.5f, 0.5f, 1.0f
		//	);
		//glm::mat4 lightMatrix = textureSpaceOffset * lightMatrix;


		//set frag shader uniforms
		//set the light properties
		setUniform("directional.Direction", nsfw::UNIFORM::TYPE::FLO3, glm::value_ptr(l.direction));
		setUniform("directional.Color", nsfw::UNIFORM::TYPE::FLO3, glm::value_ptr(l.color));


		setUniform("CameraPosition", nsfw::UNIFORM::TYPE::FLO3, glm::value_ptr(c.GetPosition()));

		setUniform("specPower", nsfw::UNIFORM::TYPE::FLO1, &specPower);
		setUniform("ambient", nsfw::UNIFORM::TYPE::FLO3, glm::value_ptr(ambientLight));
		setUniform("positionTexture", nsfw::UNIFORM::TEX2, position, 0);

		setUniform("normalTexture", nsfw::UNIFORM::TEX2, normal, 1);
		//setUniform("LightMatrix", nsfw::UNIFORM::MAT4, glm::value_ptr(lightMatrix));
		//setUniform("ShadowMap", nsfw::UNIFORM::TEX2, shadowMap, 2);


		unsigned quadVAOHandle = nsfw::Assets::instance().get<nsfw::ASSET::VAO>("Quad");
		unsigned quadNumtris = nsfw::Assets::instance().get<nsfw::ASSET::SIZE>("Quad");

		glBindVertexArray(quadVAOHandle);
		glDrawElements(GL_TRIANGLES, quadNumtris, GL_UNSIGNED_INT, 0);
	}
};