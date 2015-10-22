#pragma once

#include "../nsfwgl/nsfw.h"

#include "Camera.h"
#include "Geometry.h"

class GPass : public nsfw::RenderPass
{
public:
	void prep()
	{
		
		glBindFramebuffer(GL_FRAMEBUFFER, *fbo);
		glEnable(GL_DEPTH_TEST);
		glClearColor(.25f, .25f, .25f, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(*shader);


		//TODO_D("glUseProgram, glClear, glBindFrameBuffer, glViewPort, glEnable etc..."); 
	}
	void post()
	{
		glDisable(GL_DEPTH_TEST);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glUseProgram(0);
		glBindVertexArray(0);
		//TODO_D("Unset any gl settings"); 
	}

	GPass(const char *shaderName, const char *fboName) : RenderPass(shaderName, fboName) {}

	void draw(Camera &c, const Geometry &g)
	{
		c.GetProjection();

		setUniform("Projection", nsfw::UNIFORM::TYPE::MAT4, glm::value_ptr(c.GetProjection()));
		setUniform("ProjectionView", nsfw::UNIFORM::TYPE::MAT4, glm::value_ptr(c.GetViewProjection()));
		setUniform("View", nsfw::UNIFORM::TYPE::MAT4, glm::value_ptr(c.GetView()));
		setUniform("Model", nsfw::UNIFORM::TYPE::MAT4, glm::value_ptr(g.transform));


		setUniform("Diffuse", nsfw::UNIFORM::TEX2, g.diffuse, 0);
		//setUniform("NormalMap", nsfw::UNIFORM::TEX2, g.normal, 1);
		//setUniform("Specular", nsfw::UNIFORM::TEX2, g.specular, 2);

		//setUniform("SpecularPower", nsfw::UNIFORM::FLO1, (void*)&g.specPower);

		//nsfw::Assets::instance().get(g.mesh);
		//nsfw::Assets::instance().get(g.tris);

	   // GLuint vao = *g.mesh;
		//GLuint size = *g.tris;


		glBindVertexArray(*g.mesh);
		glDrawElements(GL_TRIANGLES, *g.tris, GL_UNSIGNED_INT, 0);
	}
};