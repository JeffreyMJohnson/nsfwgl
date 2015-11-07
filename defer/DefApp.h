#pragma once

#include "../nsfwgl/nsfw.h"

class Camera;
struct Geometry;
struct LightD;
struct LightP;

class GPass;
class LPassD;
class CPass;
class LPassP;
class ShadowPass;

class DeferredApplication : public nsfw::Application
{
	Camera   *m_camera;
	Geometry *m_soulspear;
	Geometry *m_soulspear2;
	Geometry *bunny;
	Geometry *floor;
	LightD	 *m_light;
	LightP *mPointLight;


	CPass   *m_compositePass;
	GPass   *m_geometryPass;
	ShadowPass *mShadowPass;
	LPassD  *m_directionalLightPass;
	LPassP  *mPointLightPass;

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