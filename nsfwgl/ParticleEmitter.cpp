#include "ParticleEmitter.h"

ParticleEmitter::ParticleEmitter()
	:mFirstDead(0),
	mMaxParticles(0),
	mPosition(0, 0, 0),
	mVAO(0), mVBO(0), mIBO(0)
{}

ParticleEmitter::~ParticleEmitter()
{
}

void ParticleEmitter::Init(std::string name, unsigned int maxParticles, unsigned int emitRate, float lifeTimeMin, float lifeTimeMax, float velocityMin, float velocityMax, float startSize, float endSize, const glm::vec4 & startColor, const glm::vec4 endColor)
{
	using namespace nsfw;
	using namespace ASSET;

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
	//mVertexData = std::vector<ParticleVertex>(mMaxParticles * 4, ParticleVertex());
	mVertexData = std::vector<nsfw::Vertex>(mMaxParticles * 4, nsfw::Vertex());

	std::vector<unsigned int> indexData(mMaxParticles * 6, 0);
	for (unsigned int i = 0; i < mMaxParticles; ++i)
	{
		indexData[i * 6 + 0] = i * 4 + 0;
		indexData[i * 6 + 1] = i * 4 + 1;
		indexData[i * 6 + 2] = i * 4 + 2;
		indexData[i * 6 + 3] = i * 4 + 0;
		indexData[i * 6 + 4] = i * 4 + 2;
		indexData[i * 6 + 5] = i * 4 + 3;
	}

	Assets::instance().MakeVAO(name.c_str(), nullptr, mMaxParticles * 4, indexData.data(), indexData.size());

	/*
	glGenVertexArrays(1, &mVAO);
	glBindVertexArray(mVAO);
	glGenBuffers(1, &mVBO);
	glGenBuffers(1, &mIBO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, mMaxParticles * 4 * sizeof(ParticleVertex), (void*)mVertexData.data(), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mMaxParticles * 6 * sizeof(unsigned int), (void*)indexData.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0); // position
	glEnableVertexAttribArray(1); // colour
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(ParticleVertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(ParticleVertex), (void*)(sizeof(glm::vec4)));
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	*/
}

void ParticleEmitter::Emit()
{
	// only emit if there is a dead particle to use
	if (mFirstDead >= mMaxParticles)
		return;
	// resurrect the first dead particle
	Particle particle = mParticles[mFirstDead++];
	// assign its starting position
	particle.position = mPosition;
	// randomise its lifeSpan
	particle.lifeTime = 0;
	particle.lifeSpan = (rand() / (float)RAND_MAX) * (mLifeSpanMax - mLifeSpanMin) + mLifeSpanMin;
	// set starting size and colour
	particle.color = mStartColor;
	particle.size = mStartSize;
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
		unsigned int quad = 0;
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
				particle.position += particle.velocity * deltaTime;

				//size particle
				particle.size = glm::mix(mStartSize, mEndSize, particle.lifeTime / particle.lifeSpan);

				//color particle
				particle.color = glm::mix(mStartColor, mEndColor, particle.lifeTime / particle.lifeSpan);

				// make a quad the correct size and color
				float halfSize = particle.size * 0.5f;
				mVertexData[quad * 4 + 0].position = vec4(halfSize, halfSize, 0, 1);
				mVertexData[quad * 4 + 0].color = particle.color;
				mVertexData[quad * 4 + 1].position = vec4(-halfSize, halfSize, 0, 1);
				mVertexData[quad * 4 + 1].color = particle.color;
				mVertexData[quad * 4 + 2].position = vec4(-halfSize, -halfSize, 0, 1);
				mVertexData[quad * 4 + 2].color = particle.color;
				mVertexData[quad * 4 + 3].position = vec4(halfSize, -halfSize, 0, 1);
				mVertexData[quad * 4 + 3].color = particle.color;
				// create billboard transform
				vec3 zAxis = glm::normalize(vec3(cameraTransform[3]) - particle.position);
				vec3 xAxis = glm::cross(vec3(cameraTransform[1]), zAxis);
				vec3 yAxis = glm::cross(zAxis, xAxis);
				glm::mat4 billboard(vec4(xAxis, 0),
					vec4(yAxis, 0),
					vec4(zAxis, 0),
					vec4(0, 0, 0, 1));
				mVertexData[quad * 4 + 0].position = billboard * mVertexData[quad * 4 + 0].position +
					vec4(particle.position, 0);
				mVertexData[quad * 4 + 1].position = billboard * mVertexData[quad * 4 + 1].position +
					vec4(particle.position, 0);
				mVertexData[quad * 4 + 2].position = billboard * mVertexData[quad * 4 + 2].position +
					vec4(particle.position, 0);
				mVertexData[quad * 4 + 3].position = billboard * mVertexData[quad * 4 + 3].position +
					vec4(particle.position, 0);
				++quad;
			}
		}
	}
}

void ParticleEmitter::Draw()
{
	glUseProgram(mShader);
	// sync the particle vertex buffer
	// based on how many alive particles there are
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, mFirstDead * 4 * sizeof(ParticleVertex), mVertexData.data());
	// draw particles
	glBindVertexArray(mVAO);
	glDrawElements(GL_TRIANGLES, mFirstDead * 6, GL_UNSIGNED_INT, 0);
}
