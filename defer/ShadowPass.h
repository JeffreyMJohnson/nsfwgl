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
		setUniform("LightMatrix", nsfw::UNIFORM::MAT4, glm::value_ptr(light.projection * light.view));
		setUniform("Model", nsfw::UNIFORM::MAT4, glm::value_ptr(geometry.transform));

		//draw shadow-casting geometry
		glBindVertexArray(*geometry.mesh);
		glDrawElements(GL_TRIANGLES, *geometry.tris, GL_UNSIGNED_INT, 0);
		
	}

	//void Draw(LightD& light, ParticleEmitter& emitter)
	//{
	//	setUniform("LightMatrix", nsfw::UNIFORM::MAT4, glm::value_ptr(light.projection * light.view));
	//	for (int i = 0; i < emitter.mFirstDead; i++)
	//	{
	//		Particle* particle = &emitter.mParticles[i];

	//		mat4 modelTransform = glm::translate(particle->position) * glm::scale(glm::vec3(particle->size, particle->size, 1));
	//		setUniform("Model", nsfw::UNIFORM::TYPE::MAT4, glm::value_ptr(modelTransform));
	//		glBindVertexArray(*emitter.mesh);
	//		glDrawElements(GL_TRIANGLES, *emitter.tris, GL_UNSIGNED_INT, 0);
	//	}
	//}

};