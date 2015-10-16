#pragma once

#include "../nsfwgl/nsfw.h"

#include "Camera.h"

class CPass : public nsfw::RenderPass
{
	nsfw::Asset<nsfw::ASSET::TEXTURE> albedo, position, normal, depth, light;

public:
											
	CPass(const char *shaderName, const char *fboName) 
						  : RenderPass(shaderName, fboName), albedo("GPassAlbedo"), position("GPassPosition"), // NAMES ARE FROM ASSET LIBRARY!
											    normal("GPassNormal"),depth("GPassDepth"), light("LPassColor")
												 {}


	void prep() 
	{
		glUseProgram(*shader);
		glClear(GL_COLOR_BUFFER_BIT);
		glBindFramebuffer(GL_FRAMEBUFFER, *fbo);

		//TODO_D("glUseProgram, glClear, glBindFrameBuffer, glViewPort, glEnable etc..."); 
	}
	void post() 
	{ 
		//TODO_D("Unset any gl settings"); 
		glUseProgram(0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindVertexArray(0);
	}


	void draw()
	{
		// Set uniforms for textures we're going to composite-> NAMES ARE FROM SHADER!
		setUniform("Albedo",	nsfw::UNIFORM::TEX2, albedo,   0);
		setUniform("Position",	nsfw::UNIFORM::TEX2, position, 1);
		setUniform("Normal",	nsfw::UNIFORM::TEX2, normal,   2);
		setUniform("Depth",		nsfw::UNIFORM::TEX2, depth,    3);
		setUniform("Light",		nsfw::UNIFORM::TEX2, light,    4);

		setUniform("TexelScalar", nsfw::UNIFORM::MAT4, glm::value_ptr(nsfw::Window::instance().getTexelAdjustmentMatrix()));

		unsigned quadVAOHandle = nsfw::Assets::instance().get<nsfw::ASSET::VAO>("Quad");
		unsigned quadNumtris   = nsfw::Assets::instance().get<nsfw::ASSET::SIZE>("Quad");

		glBindVertexArray(quadVAOHandle);
		glDrawArrays(GL_TRIANGLES, 0, quadNumtris);
		//TODO_D("GL BindVAO/DrawElements with quad size and vao");
	}
};