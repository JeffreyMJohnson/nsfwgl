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
		glClearColor(0,0,0,0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(*shader);
	}
	void post()
	{
		glDisable(GL_DEPTH_TEST);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glUseProgram(0);
		glBindVertexArray(0);
	}

	GPass(const char *shaderName, const char *fboName) : RenderPass(shaderName, fboName) {}

	void draw(Camera &c, const Geometry &g)
	{
		c.GetProjection();

		//vert shader uniforms
		setUniform("Projection", nsfw::UNIFORM::TYPE::MAT4, glm::value_ptr(c.GetProjection()));
		setUniform("View", nsfw::UNIFORM::TYPE::MAT4, glm::value_ptr(c.GetView()));
		setUniform("Model", nsfw::UNIFORM::TYPE::MAT4, glm::value_ptr(g.transform));

		//frag shader uniforms
		if (g.diffuse.name == "")
		{
			int b = 0;
			setUniform("isTexture", nsfw::UNIFORM::BOOL, &b);
		}
		else
		{
			setUniform("Diffuse", nsfw::UNIFORM::TEX2, g.diffuse, 0);
		}

		//HACKHACK
		setUniform("isObj", nsfw::UNIFORM::BOOL, &g.isObjNormals);
		

		glBindVertexArray(*g.mesh);
		glDrawElements(GL_TRIANGLES, *g.tris, GL_UNSIGNED_INT, 0);
	}
};