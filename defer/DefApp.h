#pragma once

#include "../nsfwgl/nsfw.h"

class Camera;
struct Geometry;
struct LightD;
struct LightP;
class ParticleEmitter;

class GPass;
class LPassD;
class CPass;
class LPassP;
class ShadowPass;

class DeferredApplication : public nsfw::Application
{
	Camera* mCamera;
	Geometry* mSoulspear;
	Geometry* mSoulspear2;
	Geometry* mBunny;
	Geometry* mFloor;
	LightD* mLight;
	LightP* mPointLight;
	ParticleEmitter* mEmitter;

	CPass* mCompositePass;
	GPass* mGeometryPass;
	ShadowPass* mShadowPass;
	LPassD* mDirectionalLightPass;
	LPassP* mPointLightPass;

	Geometry* mTestCube;

	//for debug - sending a single texture straight to the
	//cpass for drawing to screen.
	nsfw::Asset<nsfw::ASSET::TEXTURE> mDebugTexture;

	void UpdateFlyCamControls(float deltaTime, float moveSpeed);

public:
	void onInit();
	void onStep();
	void onTerm();
	void onPlay();
};