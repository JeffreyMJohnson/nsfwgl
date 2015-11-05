#pragma once
#include "../nsfwgl/nsfw.h"

class ShadowPass : public nsfw::RenderPass
{
	glm::mat4 lightMatrix;

public:
	ShadowPass(const char *shaderName, const char *fboName) :
		RenderPass(shaderName, fboName) {}
	void prep()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, *fbo);
		glEnable(GL_DEPTH_TEST);
		//glViewport(0, 0, 1024, 1024);
		glClear(GL_DEPTH_BUFFER_BIT);
		glUseProgram(*shader);


	}

	void post()
	{
		glDisable(GL_DEPTH_TEST);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glUseProgram(0);
	}

	void draw(LightD& light, Geometry& geometry)
	{
		setUniform("Model", nsfw::UNIFORM::TYPE::MAT4, glm::value_ptr(geometry.transform));
		glm::mat4 lightProjection = glm::ortho<float>(-10, 10, -10, 10, -10, 10);
		glm::mat4 lightView = glm::lookAt(light.direction, glm::vec3(0), glm::vec3(0, 1, 0));
		lightMatrix = lightProjection * lightView;

		setUniform("LightMatrix", nsfw::UNIFORM::MAT4, glm::value_ptr(lightMatrix));

		//draw shadow-casting geometry
		glBindVertexArray(*geometry.mesh);
		glDrawElements(GL_TRIANGLES, *geometry.tris, GL_UNSIGNED_INT, 0);
	}

};