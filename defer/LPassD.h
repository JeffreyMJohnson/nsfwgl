#pragma once
#include "../nsfwgl/nsfw.h"
#include "Light.h"
#include "Camera.h"

class LPassD : public nsfw::RenderPass
{
	nsfw::Asset<nsfw::ASSET::TEXTURE> position, normal, ShadowMap;
	glm::vec3 ambientLight = vec3(0 );
	float specPower = 1;
	glm::mat4 textureSpaceOffset = glm::mat4(0.5f, 0.0f, 0.0f, 0.0f,
											 0.0f, 0.5f, 0.0f, 0.0f,
											 0.0f, 0.0f, 0.5f, 0.0f,
											 0.5f, 0.5f, 0.5f, 1.0f);

public:
	bool useLighting = true;

	LPassD(const char *shaderName, const char *fboName) : RenderPass(shaderName, fboName), position("GPassPosition"), normal("GPassNormal"), ShadowMap("ShadowMap") {}

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
		setUniform("Directional.Direction", nsfw::UNIFORM::TYPE::FLO3, glm::value_ptr(l.direction));
		setUniform("Directional.Color", nsfw::UNIFORM::TYPE::FLO3, glm::value_ptr(l.color));
		setUniform("Directional.Projection", nsfw::UNIFORM::MAT4, glm::value_ptr(l.projection));
		setUniform("Directional.View", nsfw::UNIFORM::MAT4, glm::value_ptr(l.view));
		setUniform("TextureSpaceOffset", nsfw::UNIFORM::MAT4, glm::value_ptr(textureSpaceOffset));


		setUniform("CameraPosition", nsfw::UNIFORM::TYPE::FLO3, glm::value_ptr(c.GetPosition()));
		setUniform("CameraView", nsfw::UNIFORM::MAT4, glm::value_ptr(c.GetView()));

		setUniform("SpecPower", nsfw::UNIFORM::TYPE::FLO1, &specPower);
		setUniform("AmbientColor", nsfw::UNIFORM::TYPE::FLO3, glm::value_ptr(ambientLight));
		setUniform("PositionMap", nsfw::UNIFORM::TEX2, position, 0);

		setUniform("NormalMap", nsfw::UNIFORM::TEX2, normal, 1);
		setUniform("ShadowMap", nsfw::UNIFORM::TEX2, ShadowMap, 2);

		setUniform("UseLighting", nsfw::UNIFORM::BOOL, &useLighting);

		unsigned quadVAOHandle = nsfw::Assets::instance().get<nsfw::ASSET::VAO>("Quad");
		unsigned quadNumtris = nsfw::Assets::instance().get<nsfw::ASSET::SIZE>("Quad");

		glBindVertexArray(quadVAOHandle);
		glDrawElements(GL_TRIANGLES, quadNumtris, GL_UNSIGNED_INT, 0);
	}
};