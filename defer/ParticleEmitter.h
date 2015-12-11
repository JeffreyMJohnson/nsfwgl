#pragma once
#include "../nsfwgl/nsfw.h"
#include "glm\glm.hpp"
#include "glm\ext.hpp"
#include <vector>
#include <string>

struct Particle
{
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec4 color;
	float size;

	float lifeTime;
	float lifeSpan;
};

/*
struct ParticleVertex
{
	glm::vec4 position;
	glm::vec4 color;
};
*/

class ParticleEmitter
{
public:

	std::vector<Particle> mParticles;
	unsigned int mFirstDead;
	unsigned int mMaxParticles;
	std::string mName;

	nsfw::Asset<nsfw::ASSET::VAO> mesh;
	nsfw::Asset<nsfw::ASSET::SIZE> tris;

	nsfw::Asset<nsfw::ASSET::TEXTURE> diffuse;


	glm::vec3 mPosition;

	float mEmitTimer;
	float mEmitRate;

	float mLifeSpanMin;
	float mLifeSpanMax;

	float mVelocityMin;
	float mVelocityMax;

	float mStartSize;
	float mEndSize;

	glm::vec4 mStartColor;
	glm::vec4 mEndColor;

	ParticleEmitter()
	{
		mFirstDead = 0;
		mMaxParticles = 0;
		mPosition = glm::vec3(0);
	}
	~ParticleEmitter() {}

	void Init(unsigned int maxParticles,
		unsigned int emitRate,
		float lifeTimeMin, float lifeTimeMax,
		float velocityMin, float velocityMax,
		float startSize, float endSize,
		const glm::vec4& startColor,
		const glm::vec4 endColor)
	{
		//set up timer
		mEmitTimer = 0;
		mEmitRate = 1.0f / emitRate;

		mStartColor = startColor;
		mEndColor = endColor;
		mStartSize = startSize;
		mEndSize = endSize;
		mVelocityMin = velocityMin;
		mVelocityMax = velocityMax;
		mLifeSpanMin = lifeTimeMin;
		mLifeSpanMax = lifeTimeMax;
		mMaxParticles = maxParticles;

		mParticles = std::vector<Particle>(mMaxParticles, Particle());
		mFirstDead = 0;
	}

	void Emit()
	{
		if (mFirstDead >= mMaxParticles)
		{
			return;
		}
		//resurrect it
		Particle* newParticle = &mParticles[mFirstDead++];

		//start position is with the emitter hotdog
		newParticle->position = mPosition;

		//set velocity to random value
		float velocity = (rand() / (float)RAND_MAX) * (mVelocityMax - mVelocityMin) + mVelocityMin;
		newParticle->velocity.x = (rand() / (float)RAND_MAX) * 2 - 1;
		newParticle->velocity.y = (rand() / (float)RAND_MAX) * 2 - 1;
		newParticle->velocity.z = (rand() / (float)RAND_MAX) * 2 - 1;
		newParticle->velocity = glm::normalize(newParticle->velocity) * velocity;

		//set lifespan randomized
		//newParticle->lifeSpan = 5;
		newParticle->lifeTime = 0;
		newParticle->lifeSpan = (rand() / (float)RAND_MAX) * (mLifeSpanMax - mLifeSpanMin) + mLifeSpanMin;

		//set starting size
		newParticle->size = mStartSize;

		//set start color
		newParticle->color = mStartColor;

	}

	void Update(float deltaTime, const glm::mat4& cameraTransform)
	{
		using glm::vec3;
		using glm::vec4;

		//spawn particles
		mEmitTimer += deltaTime;
		while (mEmitTimer > mEmitRate)
		{
			Emit();
			mEmitTimer -= mEmitRate;
		}

		//update particles
		for (unsigned int i = 0; i < mFirstDead; i++)
		{
			Particle* particle = &mParticles[i];

			particle->lifeTime += deltaTime;
			if (particle->lifeTime >= particle->lifeSpan)
			{
				//swap last alive with this one
				Particle particle = mParticles[mFirstDead - 1];
				mFirstDead--;
			}
			else
			{
				//move particle
				particle->position += particle->velocity * deltaTime;

				//size particle
				float size = glm::mix(mStartSize, mEndSize, particle->lifeTime / particle->lifeSpan);

				//color particle
				particle->color = glm::mix(mStartColor, mEndColor, particle->lifeTime / particle->lifeSpan);
			}
		}
	}


};