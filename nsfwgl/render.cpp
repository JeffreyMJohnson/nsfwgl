#include "nsfw.h"


// Each Case should set up a uniform using the pass parameters
bool nsfw::RenderPass::setUniform(const char *name, nsfw::UNIFORM::TYPE type, const void *value, unsigned count, bool normalize) 
{
	GLint location = glGetUniformLocation(*shader, name);
	if (location == -1)
	{
		printf("Error locating uniform %s in shader.\n", name);
		return false;
	}
	switch (type)
	{
	case nsfw::UNIFORM::FLO1:
		glUniform1f(location, *(GLfloat*)value);
		break;
	case nsfw::UNIFORM::FLO3: 
		glUniform3fv(location, 1, (GLfloat*)value);
		break;
	case nsfw::UNIFORM::FLO4: 
		glUniform4fv(location, 1, (GLfloat*)value);
		break;
	case nsfw::UNIFORM::MAT4: 
		glUniformMatrix4fv(location, 1, false, (const GLfloat*)value);
		break;
	case nsfw::UNIFORM::INT1:
		glUniform1i(location, *(GLint*)value);
		break;
	case nsfw::UNIFORM::TEX2: 
		glUniform1i(location, count);
		glActiveTexture(GL_TEXTURE0 + count);
		glBindTexture(GL_TEXTURE_2D,  *(GL_HANDLE*)value);
		break;
	default:				  
		std::cout << "Error setting uniform value, Bad Type.\n";
		return false;
	}

	return true;
}

// set GL state settings and globally accessible uniforms! Should be called before rendering occurs!
void nsfw::RenderPass::prep()
{
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glEnable(GL_DEPTH_TEST);
}

// reset the GL state- undo what we did just in case.
void nsfw::RenderPass::post() 
{ 
	//glDisable(GL_DEPTH_TEST);
}