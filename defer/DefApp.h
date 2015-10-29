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

class DeferredApplication : public nsfw::Application
{
	Camera   *m_camera;
	Geometry *m_soulspear;
	Geometry *m_soulspear2;
	Geometry *bunny;
	LightD	 *m_light;
	LightP *mPointLight;


	CPass   *m_compositePass;
	GPass   *m_geometryPass;
	LPassD  *m_directionalLightPass;
	LPassP  *mPointLightPass;

	Geometry* mTestCube;

	void UpdateFlyCamControls(float deltaTime, float moveSpeed);

public:
	void onInit();
	void onStep();
	void onTerm();
	void onPlay();
};