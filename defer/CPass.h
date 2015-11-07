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
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(*shader);
	}
	void post()
	{
		glUseProgram(0);
		glBindVertexArray(0);
	}


	void draw()
	{
		//frag shader uniforms
		setUniform("Albedo", nsfw::UNIFORM::TEX2, &albedo, 0);
		setUniform("Light", nsfw::UNIFORM::TEX2, &light, 1);

		unsigned quadVAOHandle = nsfw::Assets::instance().get<nsfw::ASSET::VAO>("Quad");
		unsigned quadNumtris = nsfw::Assets::instance().get<nsfw::ASSET::SIZE>("Quad");

		glBindVertexArray(quadVAOHandle);
		glDrawElements(GL_TRIANGLES, quadNumtris, GL_UNSIGNED_INT, 0);


	}

	void DrawDebugTexture(nsfw::Asset<nsfw::ASSET::TEXTURE>& debugTexture)
	{
		setUniform("DebugTexture", nsfw::UNIFORM::TEX2, &debugTexture, 2);
		bool bTrue = GL_TRUE;
		setUniform("OutputDebugTexture", nsfw::UNIFORM::BOOL, &bTrue);
		unsigned quadVAOHandle = nsfw::Assets::instance().get<nsfw::ASSET::VAO>("Quad");
		unsigned quadNumtris = nsfw::Assets::instance().get<nsfw::ASSET::SIZE>("Quad");

		glBindVertexArray(quadVAOHandle);
		glDrawElements(GL_TRIANGLES, quadNumtris, GL_UNSIGNED_INT, 0);
	}
};