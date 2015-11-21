#include "ParticleEmitter.h"

ParticleEmitter::ParticleEmitter()
	:mFirstDead(0),
	mMaxParticles(0),
	mTransform(
		1, 0, 0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1)
{}

ParticleEmitter::~ParticleEmitter()
{
}

void ParticleEmitter::Init(unsigned int maxParticles, unsigned int emitRate, float lifeTimeMin, float lifeTimeMax, float velocityMin, float velocityMax, float startSize, float endSize, const glm::vec4 & startColor, const glm::vec4 endColor)
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

void ParticleEmitter::Emit()
{
	// only emit if there is a dead particle to use
	if (mFirstDead >= mMaxParticles)
		return;
	// resurrect the first dead particle
	Particle particle = mParticles[mFirstDead++];
	// assign its starting position
	particle.transform = mTransform;
	// randomise its lifeSpan
	particle.lifeTime = 0;
	particle.lifeSpan = (rand() / (float)RAND_MAX) * (mLifeSpanMax - mLifeSpanMin) + mLifeSpanMin;
	// set starting size and colour
	particle.color = mStartColor;
	particle.transform = glm::scale(particle.transform, glm::vec3(mStartSize,mStartSize,1));
	// randomise velocity direction and strength
	float velocity = (rand() / (float)RAND_MAX) * (mVelocityMax - mVelocityMin) + mVelocityMin;
	particle.velocity.x = (rand() / (float)RAND_MAX) * 2 - 1;
	particle.velocity.y = (rand() / (float)RAND_MAX) * 2 - 1;
	particle.velocity.z = (rand() / (float)RAND_MAX) * 2 - 1;
	particle.velocity = glm::normalize(particle.velocity) *	velocity;
}

void ParticleEmitter::Update(float deltaTime, const glm::mat4 & cameraTransform)
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
			Particle particle = mParticles[i];

			particle.lifeTime += deltaTime;
			if (particle.lifeTime >= particle.lifeSpan)
			{
				//swap last alive with this one
				Particle particle = mParticles[mFirstDead - 1];
				mFirstDead--;
			}
			else
			{
				//move particle
				glm::translate(particle.transform, particle.velocity * deltaTime);

				//size particle
				float size = glm::mix(mStartSize, mEndSize, particle.lifeTime / particle.lifeSpan);
				glm::scale(particle.transform, glm::vec3(size, size, 1));

				//color particle
				particle.color = glm::mix(mStartColor, mEndColor, particle.lifeTime / particle.lifeSpan);
			}
		}
	//}
}
