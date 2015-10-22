#pragma once
/****************
GLFW Wrapper with BASIC interface.

Extend functionality as you see fit.

This class should ONLY use GLFW!

No OpenGL!!!
*****************/

#include <glm/glm.hpp>

namespace nsfw
{
	class Timer
	{
		float mTotalTime = 0;
		float mDeltaTime = 0;

	public:
		void Update()
		{
			float temp = mTotalTime;
			mTotalTime = glfwGetTime();
			mDeltaTime = mTotalTime - temp;
		}
		float GetTotalTime() { return mTotalTime; }
		float GetDeltaTime() { return mDeltaTime; }
	};

	class Window
	{
		GLFWwindow* window;
		unsigned width, height;		// keep track of width height- needed for glViewport FBO integration
		Timer mTimer;

		Window() :window(nullptr), width(0), height(0) {}
	public:
		static Window &instance() { static Window w; return w; }

		//create the context!
		void init(unsigned width, unsigned height);

		// swapBuffers and pollEvents!
		void step();

		// kill the context!
		void term();

		// just wrap what GLFW already does
		float getTime();
		float GetDeltaTime();
		bool getKey(unsigned k) const;
		bool getShouldClose() const;
		unsigned  getWidth() const;
		unsigned  getHeight() const;
		glm::mat4 getTexelAdjustmentMatrix() const;
	};
}