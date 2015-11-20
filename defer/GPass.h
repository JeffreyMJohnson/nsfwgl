#pragma once

#include "../nsfwgl/nsfw.h"

#include "Camera.h"
#include "Geometry.h"
#include "ParticleEmitter.h"

class GPass : public nsfw::RenderPass
{
public:
	GPass(const char *shaderName, const char *fboName) : RenderPass(shaderName, fboName) {}
	
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

	void Draw(Camera& camera, const Geometry &geometry, const Particle& particle)
	{
		setUniform("View", nsfw::UNIFORM::TYPE::MAT4, glm::value_ptr(camera.GetView()));
		setUniform("Projection", nsfw::UNIFORM::TYPE::MAT4, glm::value_ptr(camera.GetProjection()));
		setUniform("ParticleColor", nsfw::UNIFORM::TYPE::FLO4, glm::value_ptr(particle.color));
		bool usingParticle = true;
		setUniform("IsParticle", nsfw::UNIFORM::BOOL, &usingParticle);
		glBindVertexArray(*geometry.mesh);
		glDrawElements(GL_TRIANGLES, *geometry.tris, GL_UNSIGNED_INT, 0);
	}

	void draw(Camera &c, const Geometry &g)
	{
		//vert shader uniforms
		setUniform("Projection", nsfw::UNIFORM::TYPE::MAT4, glm::value_ptr(c.GetProjection()));
		setUniform("View", nsfw::UNIFORM::TYPE::MAT4, glm::value_ptr(c.GetView()));
		setUniform("Model", nsfw::UNIFORM::TYPE::MAT4, glm::value_ptr(g.transform));

		//frag shader uniforms
		bool usingTexture = false;
		if (g.diffuse.name != "")
		{
			setUniform("Diffuse", nsfw::UNIFORM::TEX2, g.diffuse, 0);
			usingTexture = true;
		}
		//HACKHACK
		setUniform("IsTexture", nsfw::UNIFORM::BOOL, &usingTexture);
		

		glBindVertexArray(*g.mesh);
		glDrawElements(GL_TRIANGLES, *g.tris, GL_UNSIGNED_INT, 0);
	}
};