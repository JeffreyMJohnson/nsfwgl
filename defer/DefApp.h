#pragma once

#include "../nsfwgl/nsfw.h"

class Camera;
struct Geometry;
struct LightD;

class GPass;
class LPassD;
class CPass;

class DeferredApplication : public nsfw::Application
{
	Camera   *m_camera;
	Geometry *m_soulspear;
	LightD	 *m_light;

	CPass   *m_compositePass;
	GPass   *m_geometryPass;
	LPassD  *m_directionalLightPass;

	Geometry* mTestCube;

	void UpdateFlyCamControls(float deltaTime, float moveSpeed);

public:
	void onInit();
	void onStep();
	void onTerm();
	void onPlay();
};