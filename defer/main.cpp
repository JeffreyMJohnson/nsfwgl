
#include "DefApp.h"

#include "Geometry.h"
#include "Light.h"
#include "Camera.h"
#include "Keyboard.h"

#include "GPass.h"
#include "CPass.h"
#include "LPassD.h"



using namespace nsfw;

int main()
{
	DeferredApplication d;

	d.init();
	d.play();
	d.term();

	//system("pause");
}


void DeferredApplication::onInit()
{
	auto &w = nsfw::Window::instance();
	auto &a = nsfw::Assets::instance();

	// Load Shaders
	a.loadShader("GeometryPassPhong", "./shaders/Gpass_vert.glsl", "./shaders/Gpass_frag.glsl");
	a.loadShader("LightPassDirectional", "./shaders/Light_pass_directional_vert.glsl", "./shaders/Light_pass_directional_frag.glsl");
	//a.loadShader("LightPassPoint", "/path/to/lpass/Point/vertex", "/path/to/lpass/Point/fragment");
	a.loadShader("CompPass", "./shaders/Cpass_vert.glsl", "./shaders/Cpass_frag.glsl");

	m_camera = new Camera;
	m_camera->StartupPerspective(45, (float)w.getWidth() / w.getHeight(), .1f, 1000.0f);
	m_camera->SetView(glm::vec3(0, 2, 5), glm::vec3(0, 2, 0), glm::vec3(0, 1, 0));

	Keyboard::Init();

	// Setup FBOs
	const char *gpassTextureNames[] = { "GPassAlbedo","GPassPosition","GPassNormal","GPassDepth" };
	const unsigned gpassDepths[] = { GL_RGB8,GL_RGB32F,GL_RGB32F,GL_DEPTH_COMPONENT }; // GL_RGB8, GL_RGB32, GL_RGB32, GL_DEPTH_COMPONENT
	a.makeFBO("GeometryPass", w.getWidth(), w.getHeight(), 4, gpassTextureNames, gpassDepths);

	const char *lpassTextureNames[] = { "LPassColor" };
	const unsigned lpassDepths[] = { GL_RGB8 }; // GL_RGB8
	a.makeFBO("LightPass", w.getWidth(), w.getHeight(), 1, lpassTextureNames, lpassDepths);

	// Load any other textures and geometry we want to use
	a.loadFBX("Soulspear", "./resources/models/soulspear/soulspear.fbx");

}

void DeferredApplication::onPlay()
{
	m_light = new LightD;
	m_soulspear = new Geometry;

	m_light->color = glm::vec3(1, 1, 1);
	m_light->direction = glm::normalize(glm::vec3(0,1,.25f));

	m_soulspear->mesh = "SoulSpear_Low:SoulSpear_Low1";
	m_soulspear->tris = "SoulSpear_Low:SoulSpear_Low1";
	m_soulspear->diffuse = "soulspear_diffuse.tga";	// loadFBX will need to name every handle it creates,
	m_soulspear->specPower = 40.0f;
	m_soulspear->transform = mat4(1);

	m_geometryPass = new GPass("GeometryPassPhong", "GeometryPass");
	m_directionalLightPass = new LPassD("LightPassDirectional", "LightPass");
	m_compositePass = new CPass("CompPass", "Screen"); // Screen is defined in nsfw::Assets::init()
}

void DeferredApplication::onStep()
{
	float moveSpeed = 10;

	m_light->update();
	m_camera->Update(nsfw::Window::instance().getTime());
	UpdateFlyCamControls(nsfw::Window::instance().GetDeltaTime(), moveSpeed);
	m_soulspear->update();

	//TODO_D("Draw all of our renderpasses!");
	m_geometryPass->prep();
	m_geometryPass->draw(*m_camera, *m_soulspear);
	m_geometryPass->post();

	m_directionalLightPass->prep();
	m_directionalLightPass->draw(*m_camera, *m_light);
	m_directionalLightPass->post();

	m_compositePass->prep();
	m_compositePass->draw();
	m_compositePass->post();
}

void DeferredApplication::onTerm()
{
	delete m_camera;
	delete m_light;
	delete m_soulspear;

	delete m_compositePass;
	delete m_geometryPass;
	delete m_directionalLightPass;
}

void DeferredApplication::UpdateFlyCamControls(float deltaTime, float moveSpeed)
{
	if (Keyboard::IsKeyPressed(Keyboard::KEY_W) || Keyboard::IsKeyRepeat(Keyboard::KEY_W))
	{
		m_camera->Move(moveSpeed * deltaTime);
	}
	if (Keyboard::IsKeyPressed(Keyboard::KEY_X) || Keyboard::IsKeyRepeat(Keyboard::KEY_X))
	{
		m_camera->Move(-moveSpeed * deltaTime);
	}
	if (Keyboard::IsKeyPressed(Keyboard::KEY_A) || Keyboard::IsKeyRepeat(Keyboard::KEY_A))
	{
		m_camera->Slide(-moveSpeed * deltaTime, 0);
	}
	if (Keyboard::IsKeyPressed(Keyboard::KEY_D) || Keyboard::IsKeyRepeat(Keyboard::KEY_D))
	{
		m_camera->Slide(moveSpeed * deltaTime, 0);
	}
	if (Keyboard::IsKeyPressed(Keyboard::KEY_E) || Keyboard::IsKeyRepeat(Keyboard::KEY_E))
	{
		m_camera->Slide(0, moveSpeed * deltaTime);
	}
	if (Keyboard::IsKeyPressed(Keyboard::KEY_C) || Keyboard::IsKeyRepeat(Keyboard::KEY_C))
	{
		m_camera->Slide(0, -moveSpeed * deltaTime);
	}
}
