#pragma once

#include "../nsfwgl/nsfw.h"
#include "Light.h"
#include "Camera.h"

class LPassP : public nsfw::RenderPass
{
	nsfw::Asset<nsfw::ASSET::TEXTURE> position, specular, normal;
	glm::vec3 ambientLight = vec3(0, 0, .2f);
	float specPower = 40;

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

	void draw(Camera &c, const LightP &l)
	{
		//set frag shader uniforms
		//set the light properties
		setUniform("directional.direction", nsfw::UNIFORM::TYPE::FLO3, glm::value_ptr(l.direction));
		setUniform("directional.color", nsfw::UNIFORM::TYPE::FLO3, glm::value_ptr(l.color));
		setUniform("directional.position", nsfw::UNIFORM::TYPE::FLO3, glm::value_ptr(l.position));
		setUniform("attenuation.kC", nsfw::UNIFORM::TYPE::FLO1, &l.attenuation.kC);
		setUniform("attenuation.kL", nsfw::UNIFORM::TYPE::FLO1, &l.attenuation.kL);
		setUniform("attenuation.kQ", nsfw::UNIFORM::TYPE::FLO1, &l.attenuation.kQ);


		setUniform("CameraPosition", nsfw::UNIFORM::TYPE::FLO3, glm::value_ptr(c.GetPosition()));

		setUniform("specPower", nsfw::UNIFORM::TYPE::FLO1, &specPower);
		setUniform("ambient", nsfw::UNIFORM::TYPE::FLO3, glm::value_ptr(ambientLight));
		setUniform("positionTexture", nsfw::UNIFORM::TEX2, position, 0);

		setUniform("normalTexture", nsfw::UNIFORM::TEX2, normal, 1);

		unsigned quadVAOHandle = nsfw::Assets::instance().get<nsfw::ASSET::VAO>("Quad");
		unsigned quadNumtris = nsfw::Assets::instance().get<nsfw::ASSET::SIZE>("Quad");

		glBindVertexArray(quadVAOHandle);
		glDrawElements(GL_TRIANGLES, quadNumtris, GL_UNSIGNED_INT, 0);
	}


};