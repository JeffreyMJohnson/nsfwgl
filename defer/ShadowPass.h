#pragma once
#include "../nsfwgl/nsfw.h"

class ShadowPass : public nsfw::RenderPass
{
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
		//glViewport(0, 0, 1280, 720);
	}

	void draw(LightD& light, Geometry& geometry)
	{
		setUniform("LightMatrix", nsfw::UNIFORM::MAT4, glm::value_ptr(light.projection * light.view));
		setUniform("Model", nsfw::UNIFORM::MAT4, glm::value_ptr(geometry.transform));

		//draw shadow-casting geometry
		glBindVertexArray(*geometry.mesh);
		glDrawElements(GL_TRIANGLES, *geometry.tris, GL_UNSIGNED_INT, 0);
		
	}

};