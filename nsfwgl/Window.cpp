
#include "nsfw.h"

void nsfw::Window::init(unsigned width, unsigned height)
{
	bool success = glfwInit();
	if (!success)
	{
		std::cerr << "Error initializing GLFW.\n";
		assert(false);
		return;
	}
	this->width = width;
	this->height = height;
	window = glfwCreateWindow(this->width, this->height, "NSFWGL Window" , nullptr, nullptr);
	if (window == nullptr)
	{
		std::cerr << "Error creating context window\n";
		assert(false);
		return;
	}

	//context must be set before callingf ogl_loadfunctions
	glfwMakeContextCurrent(window);

	if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
		std::cerr << "Error loading ogl.\n";
		delete window;
		assert(false);
		return;
	}

}

void nsfw::Window::step()
{
	//TODO_D("GLFW poll events and swap buffers is all that should really be here! No GL!");
	glfwPollEvents();
	glfwSwapBuffers(window);
}

void nsfw::Window::term()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

float nsfw::Window::getTime() const
{
	return glfwGetTime();
}

bool nsfw::Window::getKey(unsigned k) const
{
	return glfwGetKey(window, k);
}

bool nsfw::Window::getShouldClose() const
{
	return glfwWindowShouldClose(window);
}

unsigned nsfw::Window::getWidth() const
{
	return width;
}

unsigned nsfw::Window::getHeight() const
{
	return height;
}

glm::mat4 nsfw::Window::getTexelAdjustmentMatrix() const
{
	TODO_D("Implemented, not tested.");

	glm::vec3 texelSize = 1.0f/glm::vec3(width,height,0);
	glm::vec3 halfTexel = texelSize * 0.5f; // bottom left

	// scale up to the appropriate size, then shift it to line up w/center of texels
	return glm::translate(halfTexel) * glm::scale(glm::vec3(1, 1, 1) - texelSize);	
}