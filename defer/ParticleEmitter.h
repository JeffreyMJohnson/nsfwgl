#pragma once
#include "../nsfwgl/nsfw.h"
#include "glm/glm.hpp"
#include "glm\ext.hpp"
#include <vector>


struct Particle
{
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec4 color;
	float size;
	float lifeTime;
	float lifeSpan;
};

struct ParticleVertex
{
	glm::vec4 position;
	glm::vec4 color;
};

class ParticleEmitter
{
public:

	std::vector<Particle> mParticles;
	std::vector<ParticleVertex> mVertexData;
	unsigned int mFirstDead;
	unsigned int mMaxParticles;
	unsigned int mVAO, mVBO, mIBO;
	uint mShader = 0;

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

	ParticleEmitter();
	virtual ~ParticleEmitter();

	void Init(unsigned int maxParticles,
		unsigned int emitRate,
		float lifeTimeMin, float lifeTimeMax,
		float velocityMin, float velocityMax,
		float startSize, float endSize,
		const glm::vec4& startColor,
		const glm::vec4 endColor);

	void Emit();

	void Update(float deltaTime, const glm::mat4& cameraTransform);

	void Draw();


};