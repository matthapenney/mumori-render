#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "vertexarray.h"
#include "indexbuffer.h"
#include "shader.h"

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

void GLCLearError();
bool GLLogCall(const char* function, const char* file, int line);

class Renderer
{
public:
	void Draw(const VertexArray& VA, /*const IndexBuffer& IB,*/ const Shader& shader) const;


};