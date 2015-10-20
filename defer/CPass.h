#pragma once

#include "../nsfwgl/nsfw.h"

#include "Camera.h"

class CPass : public nsfw::RenderPass
{
	nsfw::Asset<nsfw::ASSET::TEXTURE> albedo, depth, light;

public:

	CPass(const char *shaderName, const char *fboName)
		: RenderPass(shaderName, fboName), albedo("GPassAlbedo"), depth("GPassDepth"), light("LPassColor")
	{}


	void prep()
	{

		glClearColor(.1f, .5f, .1f, 1);
		//glClear(GL_COLOR_BUFFER_BIT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(*shader);


		//TODO_D("glUseProgram, glClear, glBindFrameBuffer, glViewPort, glEnable etc..."); 
	}
	void post()
	{
		//TODO_D("Unset any gl settings"); 
		//glUseProgram(0);
		//glBindVertexArray(0);
	}


	void draw(Camera &c, const Geometry &g)
	{
		// Set uniforms for textures we're going to composite-> NAMES ARE FROM SHADER!
		//setUniform("Albedo", nsfw::UNIFORM::TEX2, &albedo, 0);

		//setUniform("Depth", nsfw::UNIFORM::TEX2, &depth, 1);
		//setUniform("Light", nsfw::UNIFORM::TEX2, &light, 1);
		setUniform("Projection", nsfw::UNIFORM::TYPE::MAT4, glm::value_ptr(c.GetProjection()));
		setUniform("View", nsfw::UNIFORM::TYPE::MAT4, glm::value_ptr(c.GetView()));
		setUniform("Model", nsfw::UNIFORM::TYPE::MAT4, glm::value_ptr(g.transform));

		setUniform("Diffuse", nsfw::UNIFORM::TEX2, &g.diffuse, 0);
		//setUniform("TexelScalar", nsfw::UNIFORM::MAT4, glm::value_ptr(nsfw::Window::instance().getTexelAdjustmentMatrix()));

		//unsigned quadVAOHandle = nsfw::Assets::instance().get<nsfw::ASSET::VAO>("Quad");
		//unsigned quadNumtris = nsfw::Assets::instance().get<nsfw::ASSET::SIZE>("Quad");

		//glBindVertexArray(quadVAOHandle);
		//glDrawArrays(GL_TRIANGLES, 0, quadNumtris);

		glBindVertexArray(*g.mesh);
		glDrawElements(GL_TRIANGLES, *g.tris, GL_UNSIGNED_INT, 0);

		//TODO_D("GL BindVAO/DrawElements with quad size and vao");


	}
};