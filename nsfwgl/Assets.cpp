#include "nsfw.h"
//#include <array>

using namespace nsfw::ASSET;

const char *nsfw::TYPE_NAMES[eSIZE + 1] = { "NONE","vao","ibo","vbo","tri-size","fbo","rbo","texture","shader","SIZE" };

#ifdef _DEBUG
#define ASSET_LOG(type) do { std::cerr << "Trying to create key: <" << TYPE_NAMES[type] << ">" << name << std::endl; } while(0)
#else 
#define ASSET_LOG(type) do {} while (0)
#endif


nsfw::GL_HANDLE nsfw::Assets::getVERIFIED(const AssetKey &key) const
{
#ifdef _DEBUG
	if (!handles.count(key))
	{
		std::cerr << "Asset Key not found: <" << TYPE_NAMES[key.first] << ">" << key.second << std::endl;
		assert(false);
		return 0;
	}
#endif
	return handles.at(key);
}

bool nsfw::Assets::setINTERNAL(ASSET::GL_HANDLE_TYPE t, const char *name, GL_HANDLE handle)
{
	AssetKey key(t, name);
#ifdef _DEBUG
	if (handles.count(key))
	{
		std::cerr << "Asset Key already exists: <" << TYPE_NAMES[key.first] << ">" << key.second << " ignoring." << std::endl;
		assert(false);
		return false;
	}
	else std::cerr << "Asset Key successfully created: <" << TYPE_NAMES[key.first] << ">" << key.second << std::endl;
#endif
	handles[key] = handle;
	return true;
}

unsigned int nsfw::Assets::LoadSubShader(unsigned int shaderType, const char * path)
{
	std::ifstream stream(path);
	std::string contents = std::string(std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>());
	if (contents.length() == 0)
	{
		std::cerr << "Error loading shader file " << path << "\ntext:\n" << contents << std::endl;
		assert(false);
		return 0;
	}

	char* code = new char[contents.length() + 1];
	strncpy_s(code, contents.length() + 1, contents.c_str(), contents.length());
	unsigned int shader = glCreateShader(shaderType);

	glShaderSource(shader, 1, &code, 0);
	GLint success = GL_FALSE;
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		int length = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
		char* log = new char[length];
		glGetShaderInfoLog(shader, length, 0, log);
		std::cout << "Error compiling shader.\n" << log << std::endl;
		delete[] log;
		assert(false);
	}

	delete[] code;

	return shader;
}


bool nsfw::Assets::makeVAO(const char * name, const struct Vertex *verts, unsigned vsize, const unsigned * tris, unsigned tsize)
{
	//TODO_D("Should generate VBO, IBO, VAO, and SIZE using the parameters, storing them in the 'handles' map.\nThis is where vertex attributes are set!");
	GLuint vao, vbo, ibo;
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ibo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glGetError();
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vsize, verts, GL_STATIC_DRAW);
	CheckGLError();

	//may have reset the bound buffer here.. may need to move after vertexattribute calls
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	glGetError();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * tsize, tris, GL_STATIC_DRAW);
	CheckGLError();

	/*
	Vertex struct layout
		glm::vec4 position;
		glm::vec4 normal;
		glm::vec4 tangent;
		glm::vec2 texCoord;
	*/
	//set up attribs
	glEnableVertexAttribArray(0);//position
	glEnableVertexAttribArray(1);//normal
	glEnableVertexAttribArray(2);//tangent
	glEnableVertexAttribArray(3);//UV coord

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_TRUE, sizeof(Vertex), (void*)(sizeof(glm::vec4) * 1));
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec4) * 2));
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec4) * 3));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	//Asset<TEXTURE> diffuseMap = "JohnDiffuseMap";
	setINTERNAL(VAO, name, vao);
	setINTERNAL(VBO, name, vbo);
	setINTERNAL(IBO, name, ibo);
	setINTERNAL(GL_HANDLE_TYPE::SIZE, name, tsize);
	return true;
}

bool nsfw::Assets::makeFBO(const char * name, unsigned w, unsigned h, unsigned nTextures, const char * names[], const unsigned depths[])
{
	ASSET_LOG(GL_HANDLE_TYPE::FBO);
	//TODO_D("Create an FBO! Array parameters are for the render targets, which this function should also generate!\n
	//use makeTexture.\n
	//NOTE THAT THERE IS NO FUNCTION SETUP FOR MAKING RENDER BUFFER OBJECTS.");

	// setup framebuffer
	GLuint fbo;
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	std::vector<GLenum> drawBuffers;
	int colorAttachmentCount = 0;
	for (int i = 0; i < nTextures; i++)
	{
		/*
		std::string name = names[i];
		int depth = depths[i];

		if (depths[i] == GL_DEPTH_COMPONENT)
		{
			makeRBO(names[i], w, h, depth);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, get(RBO, names[i]));

		}
		else
		{
			makeTexture(names[i], w, h, depths[i]);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, get(TEXTURE, names[i]), 0);
			drawBuffers.push_back(GL_COLOR_ATTACHMENT0 + i);
		}
		*/
		makeTexture(names[i], w, h, depths[i]);

		GLenum attachment = (depths[i] == GL_DEPTH_COMPONENT) ? GL_DEPTH_ATTACHMENT : (GL_COLOR_ATTACHMENT0 + colorAttachmentCount);
		glFramebufferTexture(GL_FRAMEBUFFER, attachment, get(TEXTURE, names[i]), 0);
		if (attachment != GL_DEPTH_ATTACHMENT)
		{
			drawBuffers.push_back(GL_COLOR_ATTACHMENT0 + colorAttachmentCount++);
		}
	}
	glDrawBuffers(drawBuffers.size(), drawBuffers.data());

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		bool incompleteAttachment = status == GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT;
		bool invalidEnum = status == GL_INVALID_ENUM;
		bool invalidValue = status == GL_INVALID_VALUE;
		printf("Framebuffer Error!\n");
		assert(false);
		return false;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	setINTERNAL(FBO, name, fbo);
	return true;
}

bool nsfw::Assets::makeRBO(const char * name, unsigned w, unsigned h, unsigned depth, const char * pixels)
{
	GLuint rbo;
	glGetError();
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, depth, w, h);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	CheckGLError();
	setINTERNAL(RBO, name, rbo);
	return true;
}

bool nsfw::Assets::makeTexture(const char * name, unsigned w, unsigned h, unsigned depth, const char *pixels)
{
	glGetError();
	ASSET_LOG(GL_HANDLE_TYPE::TEXTURE);
	//TODO_D("Allocate a texture using the given space/dimensions. Should work if 'pixels' is null, so that you can use this same function with makeFBO\n note that Dept will use a GL value.");
	GLuint tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	if (nullptr == pixels && depth != GL_DEPTH_COMPONENT)
	{
		GLenum status = glGetError();
		assert(status == GL_NO_ERROR);

		glTexStorage2D(GL_TEXTURE_2D, 1, depth, w, h);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		assert(status == GL_NO_ERROR);
	}
	else   // otherwise, we're creating a normal texture
	{
		// Does this really work?
		glTexImage2D(GL_TEXTURE_2D, 0, depth, w, h, 0, depth, GL_UNSIGNED_BYTE, pixels);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	/*
	if (pixels)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, depth, w, h, 0, depth, GL_UNSIGNED_BYTE, pixels);
	}
	else
	{
		glTexStorage2D(GL_TEXTURE_2D, 1, depth, w, h);
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	*/
	CheckGLError();

	glBindTexture(GL_TEXTURE_2D, 0);
	setINTERNAL(TEXTURE, name, tex);
	return true;
}

bool nsfw::Assets::loadTexture(const char * name, const char * path)
{
	//TODO_D("This should load a texture from a file, using makeTexture to perform the allocation.\nUse STBI, and make sure you switch the format STBI provides to match what openGL needs!");
	int imageWidth = 0, imageHeight = 0, imageFormat = 0;
	const char* data = (const char*)stbi_load(path, &imageWidth, &imageHeight, &imageFormat, STBI_default);

	switch (imageFormat)
	{
	case 1: imageFormat = GL_RED; break;
	case 2: imageFormat = GL_RG; break;
	case 3: imageFormat = GL_RGB; break;
	case 4: imageFormat = GL_RGBA; break;
	}

	if (data == nullptr)
	{
		std::cout << "error loading texture.\n" << stbi_failure_reason();
		assert(false);
		return false;
	}

	makeTexture(name, imageWidth, imageHeight, imageFormat, data);
	stbi_image_free((void*)data);

	return true;
}

bool nsfw::Assets::loadShader(const char * name, const char * vpath, const char * fpath)
{
	//ASSET_LOG(GL_HANDLE_TYPE::SHADER);
	//TODO_D("Load shader from a file.");
	unsigned int vertex = LoadSubShader(GL_VERTEX_SHADER, vpath);
	if (vertex == 0)
	{
		return false;
	}
	unsigned int fragment = LoadSubShader(GL_FRAGMENT_SHADER, fpath);
	if (fragment == 0)
	{
		return false;
	}

	int success = GL_FALSE;
	GLuint	shader = glCreateProgram();
	glAttachShader(shader, vertex);
	glAttachShader(shader, fragment);
	glLinkProgram(shader);
	glDeleteShader(vertex);
	glDeleteShader(fragment);

	glGetProgramiv(shader, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		int length = 0;
		glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &length);
		char* log = new char[length];
		glGetProgramInfoLog(shader, length, 0, log);
		std::cout << "Error linking shader program.\n" << log << std::endl;
		delete[] log;
		assert(false);
		return false;
	}
	setINTERNAL(SHADER, name, shader);
	return true;
}

bool nsfw::Assets::loadFBX(const char * name, const char * path)
{
	//name/meshName
	//name/textureName
	//TODO_D("FBX file-loading support needed.\nThis function should call loadTexture and makeVAO internally.\nFBX meshes each have their own name, 
	//you may use this to name the meshes as they come in.\nMAKE SURE YOU SUPPORT THE DIFFERENCE BETWEEN FBXVERTEX AND YOUR VERTEX STRUCT!\n");

	FBXFile file;
	std::vector<Vertex> vertices;
	std::vector<unsigned> indices;
	bool success = file.load(path, FBXFile::UNITS_METER, true, false, false);
	if (!success)
	{
		std::cout << "Error loading FBX file:\n";
		assert(false);
		return false;
	}

	//load meshes
	assert(file.getMeshCount() > 0);
	for (int meshIndex = 0; meshIndex < file.getMeshCount(); meshIndex++)
	{
		FBXMeshNode* mesh = file.getMeshByIndex(meshIndex);

		for (int verticesIndex = 0; verticesIndex < mesh->m_vertices.size(); verticesIndex++)
		{
			auto xVert = mesh->m_vertices[verticesIndex];
			Vertex v;
			v.position = xVert.position;
			v.normal = xVert.normal;
			v.texCoord = xVert.texCoord1;
			vertices.push_back(v);
		}
		indices = mesh->m_indices;

		makeVAO(mesh->m_name.c_str(), vertices.data(), vertices.size(), indices.data(), indices.size());
	}

	//load textures
	for (int i = 0; i < file.getTextureCount(); i++)
	{
		FBXTexture* tex = file.getTextureByIndex(i);
		loadTexture(tex->name.c_str(), tex->path.c_str());
	}
	file.unload();
	return true;
}

bool nsfw::Assets::loadOBJ(const char * name, const char * path)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string err = tinyobj::LoadObj(shapes, materials, path);
	if (err.length() != 0)
	{
		std::cout << "Error loading OBJ file:\n" << err << std::endl;
		assert(false);
		return false;
	}
	assert(shapes.size() > 0);
	//load meshes
	for (int i = 0; i < shapes.size(); i++)
	{
		auto shape = shapes[i];
		auto mesh = shape.mesh;
		unsigned int posIndex = 0;
		unsigned int normalIndex = 0;
		unsigned int UVIndex = 0;
		bool hasNormals = mesh.normals.size() == mesh.positions.size();
		bool hasUVs = mesh.texcoords.size() == mesh.positions.size();
		//obj has vectors of floats, my struct and shaders uses glm vecs so need to build myself
		for (unsigned int vertexCount = 0; posIndex < mesh.positions.size(); vertexCount++)
		{
			Vertex vertex;
			float x = mesh.positions[posIndex++];
			float y = mesh.positions[posIndex++];
			float z = mesh.positions[posIndex++];
			vertex.position = vec4(x, y, z, 1);
			if (hasNormals)
			{
				x = mesh.normals[normalIndex++];
				y = mesh.normals[normalIndex++];
				z = mesh.normals[normalIndex++];
				vertex.normal = vec4(x, y, z, 1);
			}
			if (hasUVs)
			{
				x = mesh.texcoords[UVIndex++];
				y = mesh.texcoords[UVIndex++];
				vertex.texCoord = vec2(x, y);
			}
			vertices.push_back(vertex);
		}
		indices = mesh.indices;
		makeVAO(shape.name.c_str(), vertices.data(), vertices.size(), indices.data(), indices.size());
	}

}

void nsfw::Assets::init()
{
	//TODO_D("Load up some default assets here if you want.");

	setINTERNAL(FBO, "Screen", 0);

	makeVAO("Cube", CubeVerts, 24, CubeTris, 36);
	makeVAO("Quad", QuadVerts, 4, QuadTris, 6);
	/*
	char w[] = { 255,255,255,255 };
	makeTexture("White", 1, 1, GL_RGB8, w);
	*/

}

void nsfw::Assets::term()
{
	for each(std::pair<AssetKey, unsigned> k in handles)
	{
		switch (k.first.first)
		{
		case VBO:
			glDeleteBuffers(1, &k.second);
			break;
		case IBO:
			glDeleteBuffers(1, &k.second);
			break;
		case VAO:
			glDeleteVertexArrays(1, &k.second);
			break;
		case SHADER:
			glDeleteProgram(k.second);
			break;
		case TEXTURE:
			glDeleteTextures(1, &k.second);
			break;
		case RBO:
			glDeleteRenderbuffers(1, &k.second);
			break;
		case FBO:
			glDeleteFramebuffers(1, &k.second);
			break;
		}
	}
	handles.clear();
}

void nsfw::Assets::CheckGLError()
{
	std::string error;
	switch (glGetError())
	{
	case GL_NO_ERROR:
		error = "GL_NO_ERROR";
		break;
	case GL_INVALID_ENUM:
		error = "GL_INVALID_ENUM";
		break;
	case GL_INVALID_VALUE:
		error = "GL_INVALID_VALUE";
		break;
	case GL_INVALID_OPERATION:
		error = "GL_INVALID_OPERATION";
		break;
	case GL_INVALID_FRAMEBUFFER_OPERATION:
		error = "GL_INVALID_FRAMEBUFFER_OPERATION";
		break;
	case GL_OUT_OF_MEMORY:
		error = "GL_OUT_OF_MEMORY";
		break;
	case GL_STACK_UNDERFLOW:
		error = "GL_STACK_UNDERFLOW";
		break;
	case GL_STACK_OVERFLOW:
		error = "GL_STACK_OVERFLOW";
		break;
	}
	if (error != "GL_NO_ERROR")
	{
		std::cerr << printf("GL error found: %s\n", error);
		assert(false);
	}
}


