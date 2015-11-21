
#include "DefApp.h"

#include "Geometry.h"
#include "Light.h"
#include "Camera.h"
#include "Keyboard.h"

#include "GPass.h"
#include "CPass.h"
#include "LPassD.h"
#include "LPassP.h"
#include "ShadowPass.h"



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
	a.LoadShader("GeometryPassPhong", "./shaders/Gpass_vert.glsl", "./shaders/Gpass_frag.glsl");
	a.LoadShader("ShadowPass", "./shaders/ShadowPass_vert.glsl", "./shaders/ShadowPass_frag.glsl");
	a.LoadShader("LightPassDirectional", "./shaders/Light_pass_directional_vert.glsl", "./shaders/Light_pass_directional_frag.glsl");
	a.LoadShader("LightPassPoint", "./shaders/light_pass_point_vert.glsl", "./shaders/light_pass_point_frag.glsl");
	a.LoadShader("CompPass", "./shaders/Cpass_vert.glsl", "./shaders/Cpass_frag.glsl");

	mCamera = new Camera;
	mCamera->StartupPerspective(45, (float)w.getWidth() / w.getHeight(), .1f, 1000.0f);
	mCamera->SetView(glm::vec3(0, 2, 10), glm::vec3(0, 2, 0), glm::vec3(0, 1, 0));

	Keyboard::Init();

	// Setup FBOs
	const char *gpassTextureNames[] = { "GPassAlbedo","GPassPosition","GPassNormal","GPassDepth" };
	const unsigned gpassDepths[] = { GL_RGB8,GL_RGB32F,GL_RGB32F,GL_DEPTH_COMPONENT }; // GL_RGB8, GL_RGB32, GL_RGB32, GL_DEPTH_COMPONENT
	a.MakeFBO("GeometryPass", w.getWidth(), w.getHeight(), 4, gpassTextureNames, gpassDepths);

	const char *shadowPassTextureNames[] = { "ShadowMap" };
	const unsigned shadowPassDepths[] = { GL_DEPTH_COMPONENT };
	a.MakeFBO("ShadowPass", w.getWidth(), w.getHeight(), 1, shadowPassTextureNames, shadowPassDepths);

	const char *lpassTextureNames[] = { "LPassColor" };
	const unsigned lpassDepths[] = { GL_RGB8 }; // GL_RGB8
	a.MakeFBO("LightPass", w.getWidth(), w.getHeight(), 1, lpassTextureNames, lpassDepths);

	// Load any other textures and geometry we want to use
	a.LoadFBX("Soulspear", "./resources/models/soulspear/soulspear.fbx");
	//a.loadOBJ("Bunny", "./resources/models/bunny/bunny.obj");


}

void DeferredApplication::onPlay()
{
	mLight = new LightD;
	mPointLight = new LightP;
	mSoulspear = new Geometry;
	mSoulspear2 = new Geometry;
	mBunny = new Geometry;
	mFloor = new Geometry;
	mEmitter = new ParticleEmitter;

	//directional light
	mLight->color = glm::vec3(1, 1, 1);
	mLight->direction = glm::normalize(glm::vec3(0,.75f, 1));
	mLight->ambientIntensity = 1;
	mLight->diffuseIntensity = 1;
	mLight->projection = glm::ortho<float>(-20, 20, -20, 20, -20, 20);
	mLight->view = glm::lookAt(mLight->direction, glm::vec3(0), glm::vec3(0, 1, 0));

	//point light
	mPointLight->color = glm::vec3(1, 1, 0);
	mPointLight->position = glm::vec4(0, 2, 2, 1);
	mPointLight->attenuation.kC = 0;

	mSoulspear->mesh = "SoulSpear_Low:SoulSpear_Low1";
	mSoulspear->tris = "SoulSpear_Low:SoulSpear_Low1";
	mSoulspear->diffuse = "soulspear_diffuse.tga";
	mSoulspear->specPower = 40.0f;
	mSoulspear->transform = mat4(1);

	mFloor->mesh = "Quad";
	mFloor->tris = "Quad";
	mFloor->transform = glm::rotate(90.0f, glm::vec3(1, 0, 0)) * glm::scale(glm::vec3(10, 10, 1));

	mEmitter->mesh = "Quad";
	mEmitter->tris = "Quad";
	mEmitter->mPosition = glm::vec3(1, 1, 0);
	mEmitter->Init(
		10,//max particles
		10,//emit rate
		2, 5,//lifetime min/max
		1, 1,//velocity min/max
		1, 1,//size start/end
		glm::vec4(1), glm::vec4(1));//color start/end


	mSoulspear2->mesh = "SoulSpear_Low:SoulSpear_Low1";
	mSoulspear2->tris = "SoulSpear_Low:SoulSpear_Low1";
	mSoulspear2->diffuse = "soulspear_diffuse.tga";
	mSoulspear2->specPower = 128.0f;
	mSoulspear2->transform = translate(-5, 0,0);

	mBunny->mesh = "Bunny";
	mBunny->tris = "Bunny";
	mBunny->specPower = 128.f;


	mGeometryPass = new GPass("GeometryPassPhong", "GeometryPass");
	mDirectionalLightPass = new LPassD("LightPassDirectional", "LightPass");
	mShadowPass = new ShadowPass("ShadowPass", "ShadowPass");
	mPointLightPass = new LPassP("LightPassPoint", "LightPass");
	mCompositePass = new CPass("CompPass", "Screen"); // Screen is defined in nsfw::Assets::init()
}

void DeferredApplication::onStep()
{
	float moveSpeed = 10;
	float deltaTime = nsfw::Window::instance().GetDeltaTime();

	mLight->update(deltaTime);
	mPointLight->Update(deltaTime);
	mCamera->Update(nsfw::Window::instance().getTime());
	UpdateFlyCamControls(deltaTime, moveSpeed);
	mSoulspear->update();
	mEmitter->Update(deltaTime, mCamera->GetWorldTransform());
	//mBunny->update();

	mGeometryPass->prep();

	//mGeometryPass->draw(*mCamera, *mSoulspear);
	//mGeometryPass->draw(*mCamera, *mSoulspear2);
	mGeometryPass->draw(*mCamera, *mFloor);
	mGeometryPass->Draw(*mCamera, *mEmitter);
	//mGeometryPass->draw(*mCamera, *mBunny);

	mGeometryPass->post();

	mShadowPass->prep();
	mShadowPass->draw(*mLight, *mSoulspear);
	mShadowPass->draw(*mLight, *mSoulspear2);
	mShadowPass->draw(*mLight, *mFloor);
	mShadowPass->post();

	mDirectionalLightPass->prep();
	mDirectionalLightPass->draw(*mCamera, *mLight);
	mDirectionalLightPass->post();

	mPointLightPass->prep();
	mPointLightPass->draw(*mCamera, *mPointLight);
	mPointLightPass->post();

	mCompositePass->prep();

	/*
	DEBUG
	to send single texture to the screen: 
	comment out -> mCompositePass->draw(); 
	set mDebugTexture to texture -> "name of texture asset"; 
	call mCompositePass->DrawDebugTexture(mDebugTexture);
	*/
	//mCompositePass->draw();
	mDebugTexture = "GPassAlbedo";
	mCompositePass->DrawDebugTexture(mDebugTexture);

	mCompositePass->post();
}

void DeferredApplication::onTerm()
{
	delete mCamera;
	delete mLight;
	delete mPointLight;
	delete mSoulspear;
	delete mSoulspear2;
	delete mBunny;
	delete mFloor;
	delete mEmitter;

	delete mCompositePass;
	delete mGeometryPass;
	delete mShadowPass;
	delete mDirectionalLightPass;
}

void DeferredApplication::UpdateFlyCamControls(float deltaTime, float moveSpeed)
{
	if (Keyboard::IsKeyPressed(Keyboard::KEY_W) || Keyboard::IsKeyRepeat(Keyboard::KEY_W))
	{
		mCamera->Move(moveSpeed * deltaTime);
	}
	if (Keyboard::IsKeyPressed(Keyboard::KEY_X) || Keyboard::IsKeyRepeat(Keyboard::KEY_X))
	{
		mCamera->Move(-moveSpeed * deltaTime);
	}
	if (Keyboard::IsKeyPressed(Keyboard::KEY_A) || Keyboard::IsKeyRepeat(Keyboard::KEY_A))
	{
		mCamera->Slide(-moveSpeed * deltaTime, 0);
	}
	if (Keyboard::IsKeyPressed(Keyboard::KEY_D) || Keyboard::IsKeyRepeat(Keyboard::KEY_D))
	{
		mCamera->Slide(moveSpeed * deltaTime, 0);
	}
	if (Keyboard::IsKeyPressed(Keyboard::KEY_E) || Keyboard::IsKeyRepeat(Keyboard::KEY_E))
	{
		mCamera->Slide(0, moveSpeed * deltaTime);
	}
	if (Keyboard::IsKeyPressed(Keyboard::KEY_C) || Keyboard::IsKeyRepeat(Keyboard::KEY_C))
	{
		mCamera->Slide(0, -moveSpeed * deltaTime);
	}
}
